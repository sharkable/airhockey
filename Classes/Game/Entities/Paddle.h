//
//  Paddle.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-12.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RoundThing.h"
#import "Texture2D.h"
#import "const.h"

@interface Paddle : RoundThing {
  int        playerId_;
  BOOL       playerControlled_;
  ComputerAI aiLevel_;
  NSArray*   pucks_;
  Paddle*    otherPaddle_;
  BOOL       targetLeftCorner_;
  BOOL       targetRightCorner_;
  BOOL       targetAwayFromCorner_;
}

- (id) initWithPlayer:(int)playerId size:(PaddleSize)size playerControlled:(BOOL)playerControlled aiLevel:(ComputerAI)aiLevel;
- (void) setInitialPositionForPlayer:(int)playerId;
- (void) keepInPlayerBounds;

@property (retain) NSArray* pucks;
@property (retain) Paddle*  otherPaddle;

@end
