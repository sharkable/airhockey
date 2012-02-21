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

@implementation Paddle

@synthesize pucks=pucks_, otherPaddle=otherPaddle_;

- (id) initWithPlayer:(int)playerId size:(PaddleSize)size playerControlled:(BOOL)playerControlled aiLevel:(ComputerAI)aiLevel {
  [super init];
  
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
  
  return self;
}

- (void) dealloc {
  [pucks_ release];
  [otherPaddle_ release];
  
  [super dealloc];
}

- (void) setInitialPositionForPlayer:(int)playerId {
  switch (playerId_) {
    case PLAYER_1: {
      _x = PADDLE_1_X;
      _y = PADDLE_1_Y;      
      break;
    }
    case PLAYER_2: {
      _x = PADDLE_2_X;
      _y = PADDLE_2_Y;      
      break;
    }
  }  
}

- (void) keepInPlayerBounds {
  switch (playerId_) {
    case PLAYER_1: {
      if (self.y + self.radius > RINK_BOTTOM_Y) {
        self.y = RINK_BOTTOM_Y - self.radius;
        self.vy = 0;
      } else if (self.y - self.radius < SCREEN_HEIGHT/2+1) {
        self.y = SCREEN_HEIGHT/2+1 + self.radius;
        self.vy = 0;
      }
      break;
    }
    case PLAYER_2: {
      if (self.y - self.radius < RINK_TOP_Y) {
        self.y = RINK_TOP_Y + self.radius;
        self.vy = 0;
      } else if (self.y + self.radius > SCREEN_HEIGHT/2-1) {
        self.y = SCREEN_HEIGHT/2-1 - self.radius;
        self.vy = 0;
      }
      break;
    }
  }
  if (self.x - self.radius < RINK_LEFT_X) {
    self.x = RINK_LEFT_X + self.radius;
    self.vx = 0;
  } else if (self.x + self.radius > RINK_RIGHT_X) {
    self.x = RINK_RIGHT_X - self.radius;
    self.vx = 0;
  }
}

- (void) update {
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
    
    for (Puck* puck in pucks_) {
      if (!puck.active) {
        continue;
      }
      if (puck.vy > 4) {
        continue;
      }
      double timeToReach = 999999;
      if (puck.vy < 0) {
        timeToReach = fabs((self.y - puck.y) / puck.vy);
      }
      if (puck.y - puck.radius > SCREEN_HEIGHT/2) {
        continue;
      }
      if (target == nil || timeToReach < bestTime || (timeToReach == bestTime && puck.y < target.y)) {
        target = puck;
        bestTime = timeToReach;
      }
    }
    
    if (!target) {
      targetLeftCorner_ = targetRightCorner_ = targetAwayFromCorner_ = NO;
    }
    
    double targetX;
    double targetY;
    
    if (!targetAwayFromCorner_ && target && target.y <= RINK_TOP_Y + self.radius && fabs(target.vx) < 5 && fabs(target.vy) < 5) {
      if (target.x < SCREEN_WIDTH / 2) {
        targetLeftCorner_ = YES;
      } else {
        targetRightCorner_ = YES;
      }
    }
    
    if (targetLeftCorner_) {
      targetX = RINK_LEFT_X + self.radius;
      targetY = RINK_TOP_Y + self.radius;
      if ([self overlaps:target]) {
        targetLeftCorner_ = NO;
        targetAwayFromCorner_ = YES;
      }
    } else if (targetRightCorner_) {
      targetX = RINK_RIGHT_X - self.radius;
      targetY = RINK_TOP_Y + self.radius;
      if ([self overlaps:target]) {
        targetRightCorner_ = NO;
        targetAwayFromCorner_ = YES;
      }
    } else if (targetAwayFromCorner_) {
      targetX = SCREEN_WIDTH / 2;
      targetY = RINK_TOP_Y + self.radius;
      if (self.x >= SCREEN_WIDTH / 2 - 5 && self.x <= SCREEN_WIDTH / 2 + 5) {
        targetAwayFromCorner_ = NO;
      }
    } else if (target) {
      if (target.y > self.y) {
        targetX = target.x;
        targetY = target.y - target.radius;
      } else {
        targetY = target.y - target.radius - self.radius - 20;
        if (target.x < self.x) {
          targetX = target.x + target.radius + self.radius + 20;
        } else {
          targetX = target.x - target.radius - self.radius - 20;
        }
      }
    } else if (aiLevel_ >= caiExcellent) {
      targetX = PADDLE_2_X;
      targetY = PADDLE_2_Y;
    } else if (aiLevel_ == caiGood) {
      targetX = PADDLE_2_X;
      targetY = self.y;    
    } else {
      targetX = self.x;
      targetY = self.y;
    }
    
    // Get direction of target.
    double dx = targetX - self.x;
    double dy = targetY - self.y;
    double adx = fabs(dx);
    double ady = fabs(dy);
    
    if (adx <= speed && ady <= speed) {
      self.vx = 0;
      self.vy = 0;
      self.x = targetX;
      self.y = targetY;
    } else {
      if (fabs(dx) < fabs(self.vx)) {
        self.vx = dx;
      } else if (fabs(dy) < fabs(self.vy)) {
        self.vy = dy;
      }
      
      double dL = sqrt(dx*dx + dy*dy);
      double nx = dx / dL;
      double ny = dy / dL;
      self.vx += speed * nx;
      self.vy += speed * ny;
    }
  }
  [super update];
}

- (void) render {
  [texture_ drawAtPoint:CGPointMake(_x - texture_.contentSize.width/2, _y - texture_.contentSize.height/2)
          alpha:(self.grabbed || !playerControlled_ ? 1.0 : 0.5)
           zoom:1
          angle:0
            z:0];
}

- (BOOL) grabbable {
  return playerControlled_;
}

- (BOOL) containsTouch:(Touch*)touch {
  CGPoint p = touch.location;
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

@end
