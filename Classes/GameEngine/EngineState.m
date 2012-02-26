//
//  EngineState.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "EngineState.h"
#import "GameEngine.h"

@implementation EngineState {
 @private
  NSMutableArray *entities_;  // <id<StateEntity>>
  GameEngine *gameEngine_;  // weak
}

@synthesize gameEngine = gameEngine_;

- (id) init {
  [super init];
  
  if (self) {
    entities_ = [[NSMutableArray alloc] init];
  }
  
  return self;
}

- (void) dealloc {
  [entities_ release];
  
  [super dealloc];
}

- (void)stateIsShown {
}

- (void)update {
  for (id<StateEntity> entity in entities_) {
    [entity update];
  }
}

- (void)render {
  for (id<StateEntity> entity in entities_) {
    [entity render];
  }
}

- (void) addEntity:(id<StateEntity>)entity {
  [entities_ addObject:entity];
}

- (void) removeEntity:(id<StateEntity>)entity {
  [entities_ removeObject:entity];
}

- (void) touchesBegan:(Touch *[])touches numTouches:(int)numTouches {
  for (NSObject<StateEntity> *entity in entities_) {
    if ([entity respondsToSelector:@selector(touchesBegan:numTouches:)]) {
      [entity touchesBegan:touches numTouches:numTouches];
    }
  }
}

- (void) touchesMoved:(Touch *[])touches numTouches:(int)numTouches {
  for (NSObject<StateEntity> *entity in entities_) {
    if ([entity respondsToSelector:@selector(touchesMoved:numTouches:)]) {
      [entity touchesMoved:touches numTouches:numTouches];
    }
  }
}

- (void) touchesEnded:(Touch *[])touches numTouches:(int)numTouches {
  for (NSObject<StateEntity> *entity in entities_) {
    if ([entity respondsToSelector:@selector(touchesEnded:numTouches:)]) {
      [entity touchesEnded:touches numTouches:numTouches];
    }
  }
}

- (void) clearTouches {
  for (NSObject<StateEntity> *entity in entities_) {
    if ([entity respondsToSelector:@selector(clearTouches)]) {
      [entity clearTouches];
    }
  }
}

@end
