//
//  Puck.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RoundThing.h"
#import "Texture2D.h"
#import "Paddle.h"

@interface Puck : RoundThing {
  int fadeTicksLeft_;
  double alpha_;
}

- (void) placeForPlayer:(int)playerId roundThings:(NSArray*)roundThings center:(BOOL)center;
- (void) fadeIn;

@end
