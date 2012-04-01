//
//  EngineState.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "EngineState.h"
#import "GameEngine.h"

EngineState::EngineState(GameEngine *gameEngine) {
  gameEngine_ = gameEngine;
}

void EngineState::stateIsShown() {
}

void EngineState::update() {
  for (int i = 0; i < entities_.size(); i++) {
    entities_[i]->update();
  }
}

void EngineState::render() {
  for (int i = 0; i < entities_.size(); i++) {
    entities_[i]->render();
  }
}

void EngineState::addEntity(StateEntity *entity) {
  entities_.push_back(entity);
}

void EngineState::removeEntity(StateEntity *entity) {
  for (vector<StateEntity *>::iterator i = entities_.begin();
      i != entities_.end(); i++) {
    if (*i == entity) {
      entities_.erase(i);
      break;
    }
  }
}

void EngineState::touchesBegan(Touch *touches[], int numTouches) {
  for (int i = 0; i < entities_.size(); i++) {
    entities_[i]->touchesBegan(touches, numTouches);
  }
}

void EngineState::touchesMoved(Touch *touches[], int numTouches) {
  for (int i = 0; i < entities_.size(); i++) {
    entities_[i]->touchesMoved(touches, numTouches);
  }
}

void EngineState::touchesEnded(Touch *touches[], int numTouches) {
  for (int i = 0; i < entities_.size(); i++) {
    entities_[i]->touchesEnded(touches, numTouches);
  }
}

void EngineState::clearTouches() {
  for (int i = 0; i < entities_.size(); i++) {
    entities_[i]->clearTouches();
  }
}
