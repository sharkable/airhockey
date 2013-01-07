//
//  game_engine.cc
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "gameengine/game_engine.h"

#include "gameengine/engine_state.h"

void GameEngine::Update() {
  if (pop_on_next_) {
    states_.pop_back();
    states_.back()->StateIsShown();
    pop_on_next_ = false;
  } else if (replace_on_next_) {
    states_.pop_back();
    states_.push_back(next_state_);
    next_state_->StateIsShown();
    replace_on_next_ = false;
    next_state_.reset();
  }

  // Process input.
  sp<EngineState> top_state = states_.back();
  
  if (touches_began_.size() > 0) {
    top_state->TouchesBegan(touches_began_);
    touches_began_.clear();
  }
  if (touches_moved_.size() > 0) {
    top_state->TouchesMoved(touches_moved_);
    touches_moved_.clear();
  }
  if (touches_ended_.size() > 0) {
    top_state->TouchesEnded(touches_ended_);
    touches_ended_.clear();
  }
  
  // Update states.
  for (int i = 0; i < states_.size(); i++) {
    states_[i]->Update();
  }  
}

void GameEngine::Render() {
  for (int i = 0; i < states_.size(); i++) {
    states_[i]->Render();
  }
}

void GameEngine::ClearTouches() {
  for (int i = 0; i < states_.size(); i++) {
    states_[i]->ClearTouches();
  }
}


void GameEngine::PushState(sp<EngineState> state) {
  states_.push_back(state);
  state->StateIsShown();
}

void GameEngine::PopState() {
  pop_on_next_ = true;
}

void GameEngine::ReplaceTopState(sp<EngineState> state) {
  replace_on_next_ = true;
  next_state_ = state;
}
