//
//  Paddle.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-12.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "Paddle.h"
#import "Puck.h"
#import "GameEngine.h"

Paddle::Paddle(int playerId, PaddleSize size, bool playerControlled, ComputerAI aiLevel) {
  playerId_ = playerId;
  playerControlled_ = playerControlled;
  aiLevel_ = aiLevel;

  if (playerId == PLAYER_1) {
    switch (size) {
      case psSmall:
        texture_ = [[ResourceLoader instance] getTextureWithName:@"paddle_1_small"];
        break;
      case psMedium:
        texture_ = [[ResourceLoader instance] getTextureWithName:@"paddle_1_medium"];
        break;
      case psLarge:
        texture_ = [[ResourceLoader instance] getTextureWithName:@"paddle_1_large"];
        break;
    }
  } else {
    switch (size) {
      case psSmall:
        texture_ = [[ResourceLoader instance] getTextureWithName:@"paddle_2_small"];
        break;
      case psMedium:
        texture_ = [[ResourceLoader instance] getTextureWithName:@"paddle_2_medium"];
        break;
      case psLarge:
        texture_ = [[ResourceLoader instance] getTextureWithName:@"paddle_2_large"];
        break;
    }    
  }
  [texture_ retain];
  
  radius_ = PADDLE_RADIUS[size];
  mass_ = PADDLE_MASS;
  friction_ = playerControlled_ ? PADDLE_FRICTION : PADDLE_AI_FRICTION;
}

Paddle::~Paddle() {
}

void Paddle::setInitialPositionForPlayer(int playerId) {
  switch (playerId_) {
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

void Paddle::keepInPlayerBounds() {
  switch (playerId_) {
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

void Paddle::update() {
  // Computer AI
  if (!playerControlled_) {
    double speed = 0;
    switch (aiLevel_) {
      case caiBad:
        speed = 1;
        break;
      case caiGood:
        speed = 1.5;
        break;
      case caiExcellent:
        speed = 2;
        break;
      case caiAmazing:
        speed = 4.5;
        break;
    }
    
    // Find the puck that will reach the paddle first.
    Puck* target = nil;    
    double bestTime;
    
    for (int i = 0; i < pucks_.size(); i++) {
    Puck *puck = pucks_[i];
      if (!puck->isActive()) {
        continue;
      }
      if (puck->getVY() > 4) {
        continue;
      }
      double timeToReach = 999999;
      if (puck->getVY() < 0) {
        timeToReach = fabs((y_ - puck->getY()) / puck->getVY());
      }
      if (puck->getY() - puck->getRadius() > SCREEN_HEIGHT/2) {
        continue;
      }
      if (target == nil || timeToReach < bestTime || (timeToReach == bestTime && puck->getY() < target->getY())) {
        target = puck;
        bestTime = timeToReach;
      }
    }
    
    if (!target) {
      targetLeftCorner_ = targetRightCorner_ = targetAwayFromCorner_ = NO;
    }
    
    double targetX;
    double targetY;
    
    if (!targetAwayFromCorner_ && target && target->getY() <= RINK_TOP_Y + radius_ && fabs(target->getVX()) < 5 && fabs(target->getVY()) < 5) {
      if (target->getX() < SCREEN_WIDTH / 2) {
        targetLeftCorner_ = YES;
      } else {
        targetRightCorner_ = YES;
      }
    }
    
    if (targetLeftCorner_) {
      targetX = RINK_LEFT_X + radius_;
      targetY = RINK_TOP_Y + radius_;
      if (overlaps(target)) {
        targetLeftCorner_ = NO;
        targetAwayFromCorner_ = YES;
      }
    } else if (targetRightCorner_) {
      targetX = RINK_RIGHT_X - radius_;
      targetY = RINK_TOP_Y + radius_;
      if (overlaps(target)) {
        targetRightCorner_ = NO;
        targetAwayFromCorner_ = YES;
      }
    } else if (targetAwayFromCorner_) {
      targetX = SCREEN_WIDTH / 2;
      targetY = RINK_TOP_Y + radius_;
      if (x_ >= SCREEN_WIDTH / 2 - 5 && x_ <= SCREEN_WIDTH / 2 + 5) {
        targetAwayFromCorner_ = NO;
      }
    } else if (target) {
      if (target->getY() > y_) {
        targetX = target->getX();
        targetY = target->getY() - target->getRadius();
      } else {
        targetY = target->getY() - target->getRadius() - radius_ - 20;
        if (target->getX() < x_) {
          targetX = target->getX() + target->getRadius() + radius_ + 20;
        } else {
          targetX = target->getX() - target->getRadius() - radius_ - 20;
        }
      }
    } else if (aiLevel_ >= caiExcellent) {
      targetX = PADDLE_2_X;
      targetY = PADDLE_2_Y;
    } else if (aiLevel_ == caiGood) {
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
  RoundThing::update();
}

void Paddle::render() {
  [texture_ drawAtPoint:CGPointMake(x_ - texture_.contentSize.width/2, y_ - texture_.contentSize.height/2)
          alpha:(isGrabbed() || !playerControlled_ ? 1.0 : 0.5)
           zoom:1
          angle:0
            z:0];
}

bool Paddle::isGrabbable() {
  return playerControlled_;
}

bool Paddle::containsTouch(Touch *touch) {
  CGPoint p = touch->getLocation();
  if (p.x < 0 || p.x >= SCREEN_WIDTH) {
    return NO;
  }
  switch (playerId_) {
    case PLAYER_1:
      return p.y >= SCREEN_HEIGHT/2 && p.y < SCREEN_HEIGHT;
      break;
    case PLAYER_2:
      return p.y < SCREEN_HEIGHT/2 && p.y >= 0;
      break;
  }
  return NO;
}