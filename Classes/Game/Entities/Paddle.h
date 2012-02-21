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
  int        _playerId;
  BOOL       _playerControlled;
  ComputerAI _aiLevel;
  NSArray*   _pucks;
  Paddle*    _otherPaddle;
  BOOL       _targetLeftCorner;
  BOOL       _targetRightCorner;
  BOOL       _targetAwayFromCorner;
}

- (id) initWithPlayer:(int)playerId size:(PaddleSize)size playerControlled:(BOOL)playerControlled aiLevel:(ComputerAI)aiLevel;
- (void) setInitialPositionForPlayer:(int)playerId;
- (void) keepInPlayerBounds;

@property (retain) NSArray* pucks;
@property (retain) Paddle*  otherPaddle;

@end
