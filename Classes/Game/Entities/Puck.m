//
//  Puck.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "Puck.h"
#import "const.h"

@implementation Puck

- (id) init {
  [super init];
  
  texture_ = [[ResourceLoader instance] getTextureWithName:@"puck"];;  
  radius_ = PUCK_RADIUS;
  mass_ = PUCK_MASS;
  friction_ = PUCK_FRICTION;
  alpha_ = 1;
  
  return self;
}

- (void) update {
  [super update];
  
  // Stop the puck from getting stuck in the goal.
  if (self.y < RINK_TOP_Y && fabs(self.vy) < PUCK_GOAL_MIN_DROP_SPEED) {
    self.vy = -PUCK_GOAL_MIN_DROP_SPEED;
  } else if (self.y > RINK_BOTTOM_Y && fabs(self.vy) < PUCK_GOAL_MIN_DROP_SPEED) {
    self.vy = PUCK_GOAL_MIN_DROP_SPEED;
  }
  
  if (fadeTicksLeft_ > 0) {
    fadeTicksLeft_--;
    alpha_ = ((double)PUCK_FADE_TICKS - fadeTicksLeft_) / PUCK_FADE_TICKS;
  }
}

- (void) render {
  if (self.active) {
    [texture_ drawAtPoint:CGPointMake(_x - texture_.contentSize.width/2, _y - texture_.contentSize.height/2)
            alpha:alpha_
             zoom:1
            angle:0
              z:0];
  }
}

- (void) placeForPlayer:(int)playerId roundThings:(NSArray*)roundThings center:(BOOL)center {
  double startX = SCREEN_WIDTH / 2;
  if (!center) {
    startX += PUCK_X_SEPARATION / 2;
  }
  _x = startX;
  _y = playerId == PLAYER_1 ? PLAYER_1_PUCK_Y : PLAYER_2_PUCK_Y;

  vx_ = 0;
  vy_ = 0;
  
  BOOL goLeft = YES;
  int offset = 1;
  BOOL overlapping;
  do {
    overlapping = NO;
    for (RoundThing* thing in roundThings) {
      if (thing != self && [self overlaps:thing]) {
        overlapping = YES;
        if (goLeft) {
          _x = startX - offset * PUCK_X_SEPARATION;
          goLeft = NO;
        } else {
          _x = startX + offset * PUCK_X_SEPARATION;
          offset++;
          goLeft = YES;
        }
        break;
      }
    }
  } while (overlapping);
}

- (void) fadeIn {
  fadeTicksLeft_ = PUCK_FADE_TICKS;
  alpha_ = 0;
}

@end
