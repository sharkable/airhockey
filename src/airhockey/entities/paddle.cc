//
//  paddle.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2010-04-12.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "airhockey/entities/paddle.h"

#include <cmath>

#include "gameengine/coordinate_types.h"
#include "gameengine/game_engine.h"
#include "gameengine/resource_loader.h"

#include "airhockey/entities/puck.h"
#include "airhockey/entities/rink.h"

using std::vector;

static const double kPaddleRadii[] = {45, 65, 92.5};
static const double kPaddleY = 130;
static const double kPaddleMass = 100;
static const double kPaddleFriction = 0.1;
static const double kPaddleAIFriction = 0.999;
static const int kPaddleAIInitialPauseTicks = 30;

Paddle::Paddle(GameEngine *game_engine, Rink &rink, PlayerId player_id, PaddleSize size,
               bool player_controlled, ComputerAI ai_level, vector<sp<Puck> > &pucks)
    : RoundThing(game_engine),
      rink_(rink),
      player_id_(player_id),
      player_controlled_(player_controlled),
      ready_to_play_(true),
      ai_level_(ai_level),
      pucks_(pucks),
      target_left_corner_(false),
      target_right_corner_(false),
      target_away_from_corner_(false),
      ai_initial_pause_ticks_(0) {
  if (player_id == kPlayerId1) {
    switch (size) {
      case kPaddleSizeSmall:
        sprite_.set_texture(game_engine->resource_loader().TextureWithName("paddle_1_small"));
        break;
      case kPaddleSizeMedium:
        sprite_.set_texture(game_engine->resource_loader().TextureWithName("paddle_1_medium"));
        break;
      case kPaddleSizeLarge:
        sprite_.set_texture(game_engine->resource_loader().TextureWithName("paddle_1_large"));
        break;
    }
  } else {
    switch (size) {
      case kPaddleSizeSmall:
        sprite_.set_texture(game_engine->resource_loader().TextureWithName("paddle_2_small"));
        break;
      case kPaddleSizeMedium:
        sprite_.set_texture(game_engine->resource_loader().TextureWithName("paddle_2_medium"));
        break;
      case kPaddleSizeLarge:
        sprite_.set_texture(game_engine->resource_loader().TextureWithName("paddle_2_large"));
        break;
    }
  }

  radius_ = kPaddleRadii[size];
  mass_ = kPaddleMass;
  friction_ = player_controlled_ ? kPaddleFriction : kPaddleAIFriction;
}

void Paddle::SetInitialPosition() {
  x_ = StartingX();
  y_ = StartingY();
}

void Paddle::KeepInPlayerBounds() {
  switch (player_id_) {
    case kPlayerId1: {
      if (y_ + radius_ > rink_.BottomY()) {
        y_ = rink_.BottomY() - radius_;
        vy_ = 0;
      } else if (y_ - radius_ < rink_.CenterY() + 1) {
        y_ = rink_.CenterY() + 1 + radius_;
        vy_ = 0;
      }
      break;
    }
    case kPlayerId2: {
      if (y_ - radius_ < rink_.TopY()) {
        y_ = rink_.TopY() + radius_;
        vy_ = 0;
      } else if (y_ + radius_ > rink_.CenterY() - 1) {
        y_ = rink_.CenterY() - 1 - radius_;
        vy_ = 0;
      }
      break;
    }
  }
  if (x_ - radius_ < rink_.LeftX()) {
    x_ = rink_.LeftX() + radius_;
    vx_ = 0;
  } else if (x_ + radius_ > rink_.RightX()) {
    x_ = rink_.RightX() - radius_;
    vx_ = 0;
  }
}

void Paddle::SetReadyToPlay(bool ready) {
  ready_to_play_ = ready;
  if (!ready) {
    grabbed_ = false;
  }
  ai_initial_pause_ticks_ = kPaddleAIInitialPauseTicks;
}


// ViewEntity

void Paddle::Update() {
  // Computer AI
  if (!player_controlled_ && ready_to_play_) {
    if (ai_initial_pause_ticks_) {
      ai_initial_pause_ticks_--;
      return;
    }
    RunAITick();
  }
  RoundThing::Update();
}

void Paddle::Render(GamePoint offset) {
  sprite_.Draw(game_point_make(x_ - sprite_.content_size().width / 2 + offset.x,
                               y_ - sprite_.content_size().height / 2 + offset.y),
               0, (is_grabbed() || !player_controlled_ ? 1.0 : 0.5), 1);
}


// RoundThing

void Paddle::DidBounceOff(ViewEntity *other, double total_velocity) {
  if (typeid(*other) == typeid(Puck)) {
    ((Puck *)other)->DidBounceOff(this, total_velocity);
  }
}

bool Paddle::ContainsPoint(GamePoint point) {
  if (point.x < 0 || point.x >= rink_.TotalWidth()) {
    return false;
  }
  switch (player_id_) {
    case kPlayerId1:
      return point.y >= rink_.CenterY() && point.y < rink_.BottomY() && point.x >= rink_.LeftX() &&
          point.x < rink_.RightX();
      break;
    case kPlayerId2:
      return point.y < rink_.CenterY() && point.y >= rink_.TopY() && point.x >= rink_.LeftX() &&
          point.x < rink_.RightX();
      break;
  }
  return false;
}

bool Paddle::IsGrabbable() {
  return player_controlled_;
}

bool Paddle::IsMovable() {
  return ready_to_play_;
}


// private

void Paddle::RunAITick() {
  double speed = 0;
  switch (ai_level_) {
    case kComputerAIBad:
      speed = 0.4;
      break;
    case kComputerAIGood:
      speed = 1.15;
      break;
    case kComputerAIExcellent:
      speed = 1.7;
      break;
    case kComputerAIAmazing:
      speed = 2.5;
      break;
  }

  // Find the puck that will reach the paddle first.
  Puck *target = NULL;
  double bestTime;

  for (int i = 0; i < pucks_.size(); i++) {
    Puck *puck = pucks_[i].get();
    if (!puck->is_active()) {
      continue;
    }
    if (puck->vy() > 4) {
      continue;
    }
    double timeToReach = 999999;
    if (puck->vy() < 0) {
      timeToReach = fabs((y_ - puck->y()) / puck->vy());
    }
    if (puck->y() - puck->radius() > rink_.CenterY()) {
      continue;
    }
    if (target == NULL || timeToReach < bestTime ||
        (timeToReach == bestTime && puck->y() < target->y())) {
      target = puck;
      bestTime = timeToReach;
    }
  }

  if (!target) {
    target_left_corner_ = target_right_corner_ = target_away_from_corner_ = false;
  }

  double targetX;
  double targetY;

  if (!target_away_from_corner_ && target && target->y() <= rink_.TopY() + radius_ &&
      fabs(target->vx()) < 5 && fabs(target->vy()) < 5) {
    if (target->x() < rink_.CenterX()) {
      target_left_corner_ = true;
    } else {
      target_right_corner_ = true;
    }
  }

  if (target_left_corner_) {
    targetX = rink_.LeftX() + radius_;
    targetY = rink_.TopY() + radius_;
    if (Overlaps(target)) {
      target_left_corner_ = false;
      target_away_from_corner_ = true;
    }
  } else if (target_right_corner_) {
    targetX = rink_.RightX() - radius_;
    targetY = rink_.TopY() + radius_;
    if (Overlaps(target)) {
      target_right_corner_ = false;
      target_away_from_corner_ = true;
    }
  } else if (target_away_from_corner_) {
    targetX = rink_.CenterX();
    targetY = rink_.TopY() + radius_;
    if (x_ >= rink_.CenterX() - 5 && x_ <= rink_.CenterX() + 5) {
      target_away_from_corner_ = false;
    }
  } else if (target) {
    if (target->y() > y_) {
      targetX = target->x();
      targetY = target->y() - target->radius();
    } else {
      targetY = target->y() - target->radius() - radius_ - 20;
      if (target->x() < x_) {
        targetX = target->x() + target->radius() + radius_ + 20;
      } else {
        targetX = target->x() - target->radius() - radius_ - 20;
      }
    }
  } else if (ai_level_ >= kComputerAIExcellent) {
    targetX = StartingX();
    targetY = StartingY();
  } else if (ai_level_ == kComputerAIGood) {
    targetX = StartingX();
    targetY = y_;
  } else {
    targetX = x_;
    targetY = y_;
  }

  // Get direction of target.
  double dx = targetX - x_;
  double dy = targetY - y_;
  double adx = fabs(dx);
  double ady = fabs(dy);

  if (adx <= speed && ady <= speed) {
    vx_ = 0;
    vy_ = 0;
    x_ = targetX;
    y_ = targetY;
  } else {
    if (fabs(dx) < fabs(vx_)) {
      vx_ = dx;
    } else if (fabs(dy) < fabs(vy_)) {
      vy_ = dy;
    }

    double dL = sqrt(dx*dx + dy*dy);
    double nx = dx / dL;
    double ny = dy / dL;
    vx_ += speed * nx;
    vy_ += speed * ny;
  }
}

double Paddle::StartingX() {
  return rink_.CenterX();
}

double Paddle::StartingY() {
  switch (player_id_) {
    case kPlayerId1:
      return rink_.TotalHeight() - kPaddleY - 1;
    case kPlayerId2:
      return kPaddleY;
  }
}
