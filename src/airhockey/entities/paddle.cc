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

static const double kPaddleRadii[] = {40, 65, 92.5};
static const int kPaddle2X = Rink::CenterX();
static const int kPaddle2Y = 130;
static const int kPaddle1X = kPaddle2X;
static const int kPaddle1Y = (Rink::TotalHeight() - kPaddle2Y - 1);
static const double kPaddleMass = 100;
static const double kPaddleFriction = 0.1;
static const double kPaddleAIFriction = 0.999;
static const int kPaddleAIInitialPauseTicks = 30;

Paddle::Paddle(sp<GameEngine> game_engine, PlayerId player_id, PaddleSize size,
               bool player_controlled, ComputerAI ai_level, vector<sp<Puck> > &pucks)
    : RoundThing(game_engine),
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
        sprite_.set_texture(ResourceLoader::Instance().TextureWithName("paddle_1_small"));
        break;
      case kPaddleSizeMedium:
        sprite_.set_texture(ResourceLoader::Instance().TextureWithName("paddle_1_medium"));
        break;
      case kPaddleSizeLarge:
        sprite_.set_texture(ResourceLoader::Instance().TextureWithName("paddle_1_large"));
        break;
    }
  } else {
    switch (size) {
      case kPaddleSizeSmall:
        sprite_.set_texture(ResourceLoader::Instance().TextureWithName("paddle_2_small"));
        break;
      case kPaddleSizeMedium:
        sprite_.set_texture(ResourceLoader::Instance().TextureWithName("paddle_2_medium"));
        break;
      case kPaddleSizeLarge:
        sprite_.set_texture(ResourceLoader::Instance().TextureWithName("paddle_2_large"));
        break;
    }
  }

  radius_ = kPaddleRadii[size];
  mass_ = kPaddleMass;
  friction_ = player_controlled_ ? kPaddleFriction : kPaddleAIFriction;
}

void Paddle::SetInitialPositionForPlayer(PlayerId playerId) {
  switch (player_id_) {
    case kPlayerId1: {
      x_ = kPaddle1X;
      y_ = kPaddle1Y;
      break;
    }
    case kPlayerId2: {
      x_ = kPaddle2X;
      y_ = kPaddle2Y;
      break;
    }
  }
}

void Paddle::KeepInPlayerBounds() {
  switch (player_id_) {
    case kPlayerId1: {
      if (y_ + radius_ > Rink::BottomY()) {
        y_ = Rink::BottomY() - radius_;
        vy_ = 0;
      } else if (y_ - radius_ < Rink::CenterY() + 1) {
        y_ = Rink::CenterY() + 1 + radius_;
        vy_ = 0;
      }
      break;
    }
    case kPlayerId2: {
      if (y_ - radius_ < Rink::TopY()) {
        y_ = Rink::TopY() + radius_;
        vy_ = 0;
      } else if (y_ + radius_ > Rink::CenterY() - 1) {
        y_ = Rink::CenterY() - 1 - radius_;
        vy_ = 0;
      }
      break;
    }
  }
  if (x_ - radius_ < Rink::LeftX()) {
    x_ = Rink::LeftX() + radius_;
    vx_ = 0;
  } else if (x_ + radius_ > Rink::RightX()) {
    x_ = Rink::RightX() - radius_;
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

void Paddle::Render() {
  sprite_.Draw(game_point_make(x_ - sprite_.content_size().width/2,
                               y_ - sprite_.content_size().height/2),
               0, (is_grabbed() || !player_controlled_ ? 1.0 : 0.5), 1);
}


// RoundThing

void Paddle::DidBounceOff(ViewEntity *other, double total_velocity) {
  if (typeid(*other) == typeid(Puck)) {
    ((Puck *)other)->DidBounceOff(this, total_velocity);
  }
}

bool Paddle::ContainsTouch(Touch *touch) {
  GamePoint p = touch->location();
  if (p.x < 0 || p.x >= Rink::TotalWidth()) {
    return false;
  }
  switch (player_id_) {
    case kPlayerId1:
      return p.y >= Rink::CenterY() && p.y < Rink::BottomY() && p.x >= Rink::LeftX() &&
          p.x < Rink::RightX();
      break;
    case kPlayerId2:
      return p.y < Rink::CenterY() && p.y >= Rink::TopY() && p.x >= Rink::LeftX() &&
          p.x < Rink::RightX();
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
    if (puck->y() - puck->radius() > Rink::CenterY()) {
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

  if (!target_away_from_corner_ && target && target->y() <= Rink::TopY() + radius_ &&
      fabs(target->vx()) < 5 && fabs(target->vy()) < 5) {
    if (target->x() < Rink::CenterX()) {
      target_left_corner_ = true;
    } else {
      target_right_corner_ = true;
    }
  }

  if (target_left_corner_) {
    targetX = Rink::LeftX() + radius_;
    targetY = Rink::TopY() + radius_;
    if (Overlaps(target)) {
      target_left_corner_ = false;
      target_away_from_corner_ = true;
    }
  } else if (target_right_corner_) {
    targetX = Rink::RightX() - radius_;
    targetY = Rink::TopY() + radius_;
    if (Overlaps(target)) {
      target_right_corner_ = false;
      target_away_from_corner_ = true;
    }
  } else if (target_away_from_corner_) {
    targetX = Rink::CenterX();
    targetY = Rink::TopY() + radius_;
    if (x_ >= Rink::CenterX() - 5 && x_ <= Rink::CenterX() + 5) {
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
    targetX = kPaddle2X;
    targetY = kPaddle2Y;
  } else if (ai_level_ == kComputerAIGood) {
    targetX = kPaddle2X;
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
