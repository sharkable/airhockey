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
#import "GameTouchWindow.h"

void GameEngine::pushState(EngineState *state) {
  states_.push_back(state);
  state->stateIsShown();
}

void GameEngine::popState() {
  popOnNext_ = YES;
}

void GameEngine::replaceTopState(EngineState *state) {
  replaceOnNext_ = YES;
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
    popOnNext_ = NO;
  } else if (replaceOnNext_) {
    states_.pop_back();
    states_.push_back(nextState_);
    nextState_->stateIsShown();
    replaceOnNext_ = NO;
    nextState_ = nil;
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
