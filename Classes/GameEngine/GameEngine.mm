//
//  GameEngine.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "GameEngine.h"

#import "AdEngine.h"
#import "const.h"
#import "EAGLView.h"
#import "GameTimer.h"
#import "GameTouchWindow.h"

@interface GameEngine ()
- (void)update;
- (void)render;
@end

@implementation GameEngine {
 @private
  EAGLView *view_;
  GameTouchWindow *gameTouchWindow_;
  GameTimer *gameTimer_;
  AdEngine *adEngine_;
  
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

@synthesize adEngine = adEngine_;
@synthesize window = gameTouchWindow_;

- (id)init {
  self = [super init];
  
  if (self) {
    adEngine_ = [[AdEngine alloc] init];
    adEngine_.gameEngine = self;
    
    CGRect screenSize = [[UIScreen mainScreen] bounds];
    gameTouchWindow_ = [[GameTouchWindow alloc] initWithFrame:screenSize];
    gameTouchWindow_.gameEngine = self;
    [gameTouchWindow_ addSubview:self.view];
    [gameTouchWindow_ makeKeyAndVisible];
    
    gameTimer_ = [[GameTimer alloc] initWithTarget:self selector:@selector(update)];
    
    states_ = [[Stack alloc] init];
    for (int i = 0; i < MAX_TOUCHES; i++) {
      touchesBegan_[i] = new Touch();
      touchesMoved_[i] = new Touch();
      touchesEnded_[i] = new Touch();
    }
  }
  
  return self;
}

- (void)dealloc {
  [view_ release];
  [gameTouchWindow_ release];
  [gameTimer_ release];
  [adEngine_ release];
  
  [states_ release];
  for (int i = 0; i < MAX_TOUCHES; i++) {
    delete touchesBegan_[i];
    delete touchesMoved_[i];
    delete touchesEnded_[i];
  }
  [nextState_ release];
  
  [super dealloc];
}

- (void)pushState:(EngineState *)state {
  [states_ push:state];
  [state stateIsShown];
}

- (void)popState {
  popOnNext_ = YES;
}

- (void)replaceTopState:(EngineState *)state {
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
    touchesBegan_[i]->setLocation([touch locationInView:touch.view]);
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
      CGPoint p = touchesBegan_[i]->getLocation();
      p.x *= 768.0 / 320.0;
      p.y = (p.y - (IS_FREE ? 53 : 26)) * (768.0 / 320.0);
      touchesBegan_[i]->setLocation(p);
    }
    touchesBegan_[i]->setIdentifier(touch);
    i++;
  }
}

- (void)setTouchesMoved:(NSSet *)touches {
  numTouchesMoved_ = touches.count;
  int i = 0;
  for (UITouch *touch in touches) {
    touchesMoved_[i]->setLocation([touch locationInView:touch.view]);
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
      CGPoint p = touchesMoved_[i]->getLocation();
      p.x *= 768.0 / 320.0;
      p.y = (p.y - (IS_FREE ? 53 : 26)) * (768.0 / 320.0);
      touchesMoved_[i]->setLocation(p);
    }
    touchesMoved_[i]->setIdentifier(touch);
    i++;
  }
}

- (void)setTouchesEnded:(NSSet *)touches {
  numTouchesEnded_ = touches.count;
  int i = 0;
  for (UITouch* touch in touches) {
    touchesEnded_[i]->setLocation([touch locationInView:touch.view]);
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
      CGPoint p = touchesEnded_[i]->getLocation();
      p.x *= 768.0 / 320.0;
      p.y = (p.y - (IS_FREE ? 53 : 26)) * (768.0 / 320.0);
      touchesEnded_[i]->setLocation(p);
    }
    touchesEnded_[i]->setIdentifier(touch);
    i++;
  }
}

- (void)clearTouches {
  for (int i = 0; i < states_.count; i++) {
    EngineState *state = [states_ objectAtIndex:i];
    [state clearTouches];
  }
}

- (void)start {
  [gameTimer_ start];
}

- (void)stop {
  [gameTimer_ stop];
}

- (void)addUIView:(UIView *)view {
  [view_ addSubview:view];
}

#pragma mark - Private

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
  
  [view_ renderWithTarget:self renderSelector:@selector(render)];
}

- (void)render {
  for (int i = 0; i < states_.count; i++) {
    EngineState *state = [states_ objectAtIndex:i];
    [state render];
  }
}

#pragma mark - UIViewController

- (void)loadView {
  // TODO: Consider handling viewDidUnload: to free memory.
  if (!view_) {
    view_ = [[EAGLView alloc] initWithFrame:gameTouchWindow_.frame];
  }
  self.view = view_;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
  return (interfaceOrientation == UIInterfaceOrientationPortrait ||
          interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown);
}

@end
