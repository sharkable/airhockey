//
//  GameEngine.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Stack.h"
#import "Touch.h"
#import "EngineState.h"

#define MAX_TOUCHES 20

extern void init_genrand(unsigned long s);
extern long genrand_int31(void);

@interface GameEngine : NSObject {
 @private
  Stack *states_;
  Touch *touchesBegan_[MAX_TOUCHES];
  int numTouchesBegan_;
  Touch *touchesMoved_[MAX_TOUCHES];
  int numTouchesMoved_;
  Touch *touchesEnded_[MAX_TOUCHES];
  int numTouchesEnded_;
  BOOL popOnNext_;
  BOOL replaceOnNext_;
  EngineState *nextState_;
}

+ (GameEngine *)instance;
- (void)render;
- (void)update;
- (void)pushState:(EngineState *)state;
- (void)popState;
- (void)replaceTopState:(EngineState *)state;

- (void)setTouchesBegan:(NSSet *)touchesBegan;
- (void)setTouchesMoved:(NSSet *)touchesMoved;
- (void)setTouchesEnded:(NSSet *)touchesEnded;
- (void)clearTouches;

@end
