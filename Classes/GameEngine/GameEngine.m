//
//  GameEngine.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "const.h"
#import "GameEngine.h"

extern void init_genrand(unsigned long s);
extern long genrand_int31(void);

@implementation GameEngine

- (id)init {
  self = [super init];
  
  if (self) {
    init_genrand(time(NULL));
    
    states_ = [[Stack alloc] init];
    for (int i = 0; i < MAX_TOUCHES; i++) {
      touchesBegan_[i] = [[Touch alloc] init];
      touchesMoved_[i] = [[Touch alloc] init];
      touchesEnded_[i] = [[Touch alloc] init];
    }
  }
  
  return self;
}

- (void)dealloc {
  [states_ release];
  for (int i = 0; i < MAX_TOUCHES; i++) {
    [touchesBegan_[i] release];
    [touchesMoved_[i] release];
    [touchesEnded_[i] release];
  }
  
  [super dealloc];
}

- (void)render {
  for (int i = 0; i < states_.count; i++) {
    EngineState *state = [states_ objectAtIndex:i];
    [state render];
  }
}

- (void)update {
  if (popOnNext_) {
    [states_ pop];
    [[states_ top] stateIsShown];
    popOnNext_ = NO;
  } else if (replaceOnNext_) {
    [states_ pop];
    [states_ push:nextState_];
    [nextState_ release];
    [nextState_ stateIsShown];
    replaceOnNext_ = NO;
    nextState_ = nil;
  }
  
  // Process input.
  EngineState *topState = [states_ top];
  if (numTouchesBegan_ > 0) {
    [topState touchesBegan:touchesBegan_ numTouches:numTouchesBegan_];
    numTouchesBegan_ = 0;
  }
  if (numTouchesMoved_ > 0) {
    [topState touchesMoved:touchesMoved_ numTouches:numTouchesMoved_];
    numTouchesMoved_ = 0;
  }
  if (numTouchesEnded_ > 0) {
    [topState touchesEnded:touchesEnded_ numTouches:numTouchesEnded_];
    numTouchesEnded_ = 0;
  }
  
  // Update states.
  for (int i = 0; i < states_.count; i++) {
    EngineState *state = [states_ objectAtIndex:i];
    [state update];
  }
}

- (void) pushState:(EngineState *)state {
  state.gameEngine = self;
  [states_ push:state];
  [state stateIsShown];
}

- (void) popState {
  popOnNext_ = YES;
}

- (void) replaceTopState:(EngineState *)state {
  state.gameEngine = self;
  replaceOnNext_ = YES;
  if (nextState_ != state) {
    [nextState_ release];
    nextState_ = state;
    [nextState_ retain];
  }
}

- (void)setTouchesBegan:(NSSet *)touches {
  numTouchesBegan_ = touches.count;
  int i = 0;
  for (UITouch *touch in touches) {
    touchesBegan_[i].location = [touch locationInView:touch.view];
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
      CGPoint p = touchesBegan_[i].location;
      p.x *= 768.0 / 320.0;
      p.y = (p.y - (IS_FREE ? 53 : 26)) * (768.0 / 320.0);
      touchesBegan_[i].location = p;
    }
    touchesBegan_[i].identifier = touch;
    i++;
  }
}

- (void)setTouchesMoved:(NSSet *)touches {
  numTouchesMoved_ = touches.count;
  int i = 0;
  for (UITouch *touch in touches) {
    touchesMoved_[i].location = [touch locationInView:touch.view];
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
      CGPoint p = touchesMoved_[i].location;
      p.x *= 768.0 / 320.0;
      p.y = (p.y - (IS_FREE ? 53 : 26)) * (768.0 / 320.0);
      touchesMoved_[i].location = p;
    }
    touchesMoved_[i].identifier = touch;
    i++;
  }
}

- (void)setTouchesEnded:(NSSet *)touches {
  numTouchesEnded_ = touches.count;
  int i = 0;
  for (UITouch* touch in touches) {
    touchesEnded_[i].location = [touch locationInView:touch.view];
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
      CGPoint p = touchesEnded_[i].location;
      p.x *= 768.0 / 320.0;
      p.y = (p.y - (IS_FREE ? 53 : 26)) * (768.0 / 320.0);
      touchesEnded_[i].location = p;
    }
    touchesEnded_[i].identifier = touch;
    i++;
  }
}

- (void) clearTouches {
  for (int i = 0; i < states_.count; i++) {
    EngineState *state = [states_ objectAtIndex:i];
    [state clearTouches];
  }
}

@end
