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

@class AdEngine;

@interface GameEngine : UIViewController

@property(nonatomic, readonly) AdEngine *adEngine;

- (void)render;
- (void)update;
- (void)pushState:(EngineState *)state;
- (void)popState;
- (void)replaceTopState:(EngineState *)state;

- (void)setTouchesBegan:(NSSet *)touchesBegan;
- (void)setTouchesMoved:(NSSet *)touchesMoved;
- (void)setTouchesEnded:(NSSet *)touchesEnded;
- (void)clearTouches;

- (void)startAnimation;
- (void)stopAnimation;

- (void)addUIView:(UIView *)view;

@end
