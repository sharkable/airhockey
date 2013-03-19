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
static const int kPaddle2X = (SCREEN_WIDTH / 2);
static const int kPaddle2Y = 130;
static const int kPaddle1X = kPaddle2X;
static const int kPaddle1Y = (SCREEN_HEIGHT - kPaddle2Y - 1);
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
      if (y_ + radius_ > kRinkBottomY) {
        y_ = kRinkBottomY - radius_;
        vy_ = 0;
      } else if (y_ - radius_ < SCREEN_HEIGHT/2+1) {
        y_ = SCREEN_HEIGHT/2+1 + radius_;
        vy_ = 0;
      }
      break;
    }
    case kPlayerId2: {
      if (y_ - radius_ < kRinkTopY) {
        y_ = kRinkTopY + radius_;
        vy_ = 0;
      } else if (y_ + radius_ > SCREEN_HEIGHT/2-1) {
        y_ = SCREEN_HEIGHT/2-1 - radius_;
        vy_ = 0;
      }
      break;
    }
  }
  if (x_ - radius_ < kRinkLeftX) {
    x_ = kRinkLeftX + radius_;
    vx_ = 0;
  } else if (x_ + radius_ > kRinkRightX) {
    x_ = kRinkRightX - radius_;
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
  if (!player_controlled_) {
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

// TODO ugh. Hack to prevent moving paddle on iPhone when touching the pause button.
#define PAUSE_BUTTON_SIZE (28 * 768.0 / 320.0)

bool Paddle::ContainsTouch(Touch *touch) {
  GamePoint p = touch->location();
  if (p.x < 0 || p.x >= SCREEN_WIDTH) {
    return false;
  }
  switch (player_id_) {
    case kPlayerId1:
      return p.y >= SCREEN_HEIGHT / 2 && p.y < kRinkBottomY && p.x >= kRinkLeftX &&
          p.x < kRinkRightX &&
          (p.x < SCREEN_WIDTH - PAUSE_BUTTON_SIZE || p.y < SCREEN_HEIGHT - PAUSE_BUTTON_SIZE);
      break;
    case kPlayerId2:
      return p.y < SCREEN_HEIGHT / 2 && p.y >= kRinkTopY && p.x >= kRinkLeftX &&
          p.x < kRinkRightX &&
          (p.x > PAUSE_BUTTON_SIZE || p.y >PAUSE_BUTTON_SIZE);
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


// Private

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
    if (puck->y() - puck->radius() > SCREEN_HEIGHT/2) {
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

  if (!target_away_from_corner_ && target && target->y() <= kRinkTopY + radius_ &&
      fabs(target->vx()) < 5 && fabs(target->vy()) < 5) {
    if (target->x() < SCREEN_WIDTH / 2) {
      target_left_corner_ = true;
    } else {
      target_right_corner_ = true;
    }
  }

  if (target_left_corner_) {
    targetX = kRinkLeftX + radius_;
    targetY = kRinkTopY + radius_;
    if (Overlaps(target)) {
      target_left_corner_ = false;
      target_away_from_corner_ = true;
    }
  } else if (target_right_corner_) {
    targetX = kRinkRightX - radius_;
    targetY = kRinkTopY + radius_;
    if (Overlaps(target)) {
      target_right_corner_ = false;
      target_away_from_corner_ = true;
    }
  } else if (target_away_from_corner_) {
    targetX = SCREEN_WIDTH / 2;
    targetY = kRinkTopY + radius_;
    if (x_ >= SCREEN_WIDTH / 2 - 5 && x_ <= SCREEN_WIDTH / 2 + 5) {
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
