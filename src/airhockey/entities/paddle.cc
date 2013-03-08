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

using std::vector;

Paddle::Paddle(sp<GameEngine> game_engine, int player_id, PaddleSize size, bool player_controlled,
               ComputerAI ai_level, vector<sp<Puck> > &pucks)
    : RoundThing(game_engine),
      player_id_(player_id),
      player_controlled_(player_controlled),
      ready_to_play_(true),
      ai_level_(ai_level),
      pucks_(pucks),
      target_left_corner_(false),
      target_right_corner_(false),
      target_away_from_corner_(false) {
  if (player_id == PLAYER_1) {
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

  radius_ = PADDLE_RADIUS[size];
  mass_ = PADDLE_MASS;
  friction_ = player_controlled_ ? PADDLE_FRICTION : PADDLE_AI_FRICTION;
}


void Paddle::SetInitialPositionForPlayer(int playerId) {
  switch (player_id_) {
    case PLAYER_1: {
      x_ = PADDLE_1_X;
      y_ = PADDLE_1_Y;
      break;
    }
    case PLAYER_2: {
      x_ = PADDLE_2_X;
      y_ = PADDLE_2_Y;
      break;
    }
  }
}

void Paddle::KeepInPlayerBounds() {
  switch (player_id_) {
    case PLAYER_1: {
      if (y_ + radius_ > RINK_BOTTOM_Y) {
        y_ = RINK_BOTTOM_Y - radius_;
        vy_ = 0;
      } else if (y_ - radius_ < SCREEN_HEIGHT/2+1) {
        y_ = SCREEN_HEIGHT/2+1 + radius_;
        vy_ = 0;
      }
      break;
    }
    case PLAYER_2: {
      if (y_ - radius_ < RINK_TOP_Y) {
        y_ = RINK_TOP_Y + radius_;
        vy_ = 0;
      } else if (y_ + radius_ > SCREEN_HEIGHT/2-1) {
        y_ = SCREEN_HEIGHT/2-1 - radius_;
        vy_ = 0;
      }
      break;
    }
  }
  if (x_ - radius_ < RINK_LEFT_X) {
    x_ = RINK_LEFT_X + radius_;
    vx_ = 0;
  } else if (x_ + radius_ > RINK_RIGHT_X) {
    x_ = RINK_RIGHT_X - radius_;
    vx_ = 0;
  }
}

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

  if (!target_away_from_corner_ && target && target->y() <= RINK_TOP_Y + radius_ &&
      fabs(target->vx()) < 5 && fabs(target->vy()) < 5) {
    if (target->x() < SCREEN_WIDTH / 2) {
      target_left_corner_ = true;
    } else {
      target_right_corner_ = true;
    }
  }

  if (target_left_corner_) {
    targetX = RINK_LEFT_X + radius_;
    targetY = RINK_TOP_Y + radius_;
    if (Overlaps(target)) {
      target_left_corner_ = false;
      target_away_from_corner_ = true;
    }
  } else if (target_right_corner_) {
    targetX = RINK_RIGHT_X - radius_;
    targetY = RINK_TOP_Y + radius_;
    if (Overlaps(target)) {
      target_right_corner_ = false;
      target_away_from_corner_ = true;
    }
  } else if (target_away_from_corner_) {
    targetX = SCREEN_WIDTH / 2;
    targetY = RINK_TOP_Y + radius_;
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
    targetX = PADDLE_2_X;
    targetY = PADDLE_2_Y;
  } else if (ai_level_ == kComputerAIGood) {
    targetX = PADDLE_2_X;
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
    case PLAYER_1:
      return p.y >= SCREEN_HEIGHT / 2 && p.y < RINK_BOTTOM_Y && p.x >= RINK_LEFT_X &&
          p.x < RINK_RIGHT_X &&
          (p.x < SCREEN_WIDTH - PAUSE_BUTTON_SIZE || p.y < SCREEN_HEIGHT - PAUSE_BUTTON_SIZE);
      break;
    case PLAYER_2:
      return p.y < SCREEN_HEIGHT / 2 && p.y >= RINK_TOP_Y && p.x >= RINK_LEFT_X &&
          p.x < RINK_RIGHT_X &&
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
