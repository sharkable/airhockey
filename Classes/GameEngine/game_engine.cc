//
//  GameEngine.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "game_engine.h"

#include "const.h"

void GameEngine::pushState(EngineState *state) {
  states_.push_back(state);
  state->stateIsShown();
}

void GameEngine::popState() {
  popOnNext_ = true;
}

void GameEngine::replaceTopState(EngineState *state) {
  replaceOnNext_ = true;
  nextState_ = state;
}

void GameEngine::clearTouches() {
  for (int i = 0; i < states_.size(); i++) {
    EngineState *state = states_[i];
    state->clearTouches();
  }
}

//void GameEngine::addUIView(UIView *view) {
//  [view_ addSubview:view];
//}

void GameEngine::update() {
  if (popOnNext_) {
    states_.pop_back();
    states_.back()->stateIsShown();
    popOnNext_ = false;
  } else if (replaceOnNext_) {
    states_.pop_back();
    states_.push_back(nextState_);
    nextState_->stateIsShown();
    replaceOnNext_ = false;
    nextState_ = NULL;
  }
  
  // Process input.
  EngineState *topState = states_.back();
  if (touchesBegan_.size() > 0) {
    topState->touchesBegan(touchesBegan_);
  }
  if (touchesMoved_.size() > 0) {
    topState->touchesMoved(touchesMoved_);
  }
  if (touchesEnded_.size() > 0) {
    topState->touchesEnded(touchesEnded_);
  }
  
  // Update states.
  for (int i = 0; i < states_.size(); i++) {
    EngineState *state = states_[i];
    state->update();
  }  
}

void GameEngine::render() {
  for (int i = 0; i < states_.size(); i++) {
    EngineState *state = states_[i];
    state->render();
  }
}
