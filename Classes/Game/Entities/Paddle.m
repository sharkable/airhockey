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

@synthesize pucks=_pucks, otherPaddle=_otherPaddle;

- (id) initWithPlayer:(int)playerId size:(PaddleSize)size playerControlled:(BOOL)playerControlled aiLevel:(ComputerAI)aiLevel {
  [super init];
  
  _playerId = playerId;
  _playerControlled = playerControlled;
  _aiLevel = aiLevel;

  if (playerId == PLAYER_1) {
    switch (size) {
      case psSmall:
        _texture = [[ResourceLoader instance] getTextureWithName:@"paddle_1_small"];
        break;
      case psMedium:
        _texture = [[ResourceLoader instance] getTextureWithName:@"paddle_1_medium"];
        break;
      case psLarge:
        _texture = [[ResourceLoader instance] getTextureWithName:@"paddle_1_large"];
        break;
    }
  } else {
    switch (size) {
      case psSmall:
        _texture = [[ResourceLoader instance] getTextureWithName:@"paddle_2_small"];
        break;
      case psMedium:
        _texture = [[ResourceLoader instance] getTextureWithName:@"paddle_2_medium"];
        break;
      case psLarge:
        _texture = [[ResourceLoader instance] getTextureWithName:@"paddle_2_large"];
        break;
    }    
  }
  [_texture retain];
  
  _radius = PADDLE_RADIUS[size];
  _mass = PADDLE_MASS;
  _friction = _playerControlled ? PADDLE_FRICTION : PADDLE_AI_FRICTION;
  
  return self;
}

- (void) dealloc {
  [_pucks release];
  [_otherPaddle release];
  
  [super dealloc];
}

- (void) setInitialPositionForPlayer:(int)playerId {
  switch (_playerId) {
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
  switch (_playerId) {
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
  if (!_playerControlled) {
    double speed = 0;
    switch (_aiLevel) {
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
    
    for (Puck* puck in _pucks) {
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
      _targetLeftCorner = _targetRightCorner = _targetAwayFromCorner = NO;
    }
    
    double targetX;
    double targetY;
    
    if (!_targetAwayFromCorner && target && target.y <= RINK_TOP_Y + self.radius && fabs(target.vx) < 5 && fabs(target.vy) < 5) {
      if (target.x < SCREEN_WIDTH / 2) {
        _targetLeftCorner = YES;
      } else {
        _targetRightCorner = YES;
      }
    }
    
    if (_targetLeftCorner) {
      targetX = RINK_LEFT_X + self.radius;
      targetY = RINK_TOP_Y + self.radius;
      if ([self overlaps:target]) {
        _targetLeftCorner = NO;
        _targetAwayFromCorner = YES;
      }
    } else if (_targetRightCorner) {
      targetX = RINK_RIGHT_X - self.radius;
      targetY = RINK_TOP_Y + self.radius;
      if ([self overlaps:target]) {
        _targetRightCorner = NO;
        _targetAwayFromCorner = YES;
      }
    } else if (_targetAwayFromCorner) {
      targetX = SCREEN_WIDTH / 2;
      targetY = RINK_TOP_Y + self.radius;
      if (self.x >= SCREEN_WIDTH / 2 - 5 && self.x <= SCREEN_WIDTH / 2 + 5) {
        _targetAwayFromCorner = NO;
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
    } else if (_aiLevel >= caiExcellent) {
      targetX = PADDLE_2_X;
      targetY = PADDLE_2_Y;
    } else if (_aiLevel == caiGood) {
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
  [_texture drawAtPoint:CGPointMake(_x - _texture.contentSize.width/2, _y - _texture.contentSize.height/2)
          alpha:(self.grabbed || !_playerControlled ? 1.0 : 0.5)
           zoom:1
          angle:0
            z:0];
}

- (BOOL) grabbable {
  return _playerControlled;
}

- (BOOL) containsTouch:(Touch*)touch {
  CGPoint p = touch.location;
  if (p.x < 0 || p.x >= SCREEN_WIDTH) {
    return NO;
  }
  switch (_playerId) {
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
