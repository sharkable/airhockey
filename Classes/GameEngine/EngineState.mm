//
//  EngineState.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "EngineState.h"
#import "GameEngine.h"

#include <vector>
using namespace std;

@implementation EngineState {
 @private
  vector<StateEntity *> entities_;  // TODO can I do this in Objective-C?
                                    // will it be initialized?
  GameEngine *gameEngine_;  // weak
}

@synthesize gameEngine = gameEngine_;

- (void)stateIsShown {
}

- (void)update {
  for (int i = 0; i < entities_.size(); i++) {
    entities_[i]->update();
  }
}

- (void)render {
  for (int i = 0; i < entities_.size(); i++) {
    entities_[i]->render();
  }
}

- (void)addEntity:(StateEntity *)entity {
  entities_.push_back(entity);
}

- (void)removeEntity:(StateEntity *)entity {
  for (vector<StateEntity *>::iterator i = entities_.begin();
      i != entities_.end(); i++) {
    if (*i == entity) {
      entities_.erase(i);
    }
  }
}

- (void)touchesBegan:(Touch *[])touches numTouches:(int)numTouches {
  for (int i = 0; i < entities_.size(); i++) {
    entities_[i]->touchesBegan(touches, numTouches);
  }
}

- (void)touchesMoved:(Touch *[])touches numTouches:(int)numTouches {
  for (int i = 0; i < entities_.size(); i++) {
    entities_[i]->touchesMoved(touches, numTouches);
  }
}

- (void)touchesEnded:(Touch *[])touches numTouches:(int)numTouches {
  for (int i = 0; i < entities_.size(); i++) {
    entities_[i]->touchesEnded(touches, numTouches);
  }
}

- (void) clearTouches {
  for (int i = 0; i < entities_.size(); i++) {
    entities_[i]->clearTouches();
}}

@end
