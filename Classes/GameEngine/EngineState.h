//
//  EngineState.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "StateEntity.h"
#include "Touch.h"

@class GameEngine;

@interface EngineState : NSObject 

@property(nonatomic, readonly) GameEngine *gameEngine;

- (id)initWithGameEngine:(GameEngine *)gameEngine;

- (void)stateIsShown;
- (void)update;
- (void)render;
- (void)addEntity:(id<StateEntity>)entity;
- (void)removeEntity:(id<StateEntity>)entity;
- (void)touchesBegan:(Touch *[])touches numTouches:(int)numTouches;
- (void)touchesMoved:(Touch *[])touches numTouches:(int)numTouches;
- (void)touchesEnded:(Touch *[])touches numTouches:(int)numTouches;
- (void)clearTouches;

@end
