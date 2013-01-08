//
//  engine_state.cc
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "gameengine/engine_state.h"

#include "gameengine/game_engine.h"
#include "gameengine/state_entity.h"

EngineState::EngineState(GameEngine &game_engine) : game_engine_(game_engine) {
}

void EngineState::Update() {
  for (int i = 0; i < entities_.size(); i++) {
    entities_[i]->Update();
  }
}

void EngineState::Render() {
  for (int i = 0; i < entities_.size(); i++) {
    entities_[i]->Render();
  }
}

void EngineState::TouchesBegan(vector<Touch> touches) {
  for (int i = 0; i < entities_.size(); i++) {
    entities_[i]->TouchesBegan(touches);
  }
}

void EngineState::TouchesMoved(vector<Touch> touches) {
  for (int i = 0; i < entities_.size(); i++) {
    entities_[i]->TouchesMoved(touches);
  }
}

void EngineState::TouchesEnded(vector<Touch> touches) {
  for (int i = 0; i < entities_.size(); i++) {
    entities_[i]->TouchesEnded(touches);
  }
}

void EngineState::ClearTouches() {
  for (int i = 0; i < entities_.size(); i++) {
    entities_[i]->ClearTouches();
  }
}

void EngineState::AddEntity(StateEntity *entity) {
  entities_.push_back(sp<StateEntity>(entity));
}

void EngineState::AddEntity(sp<StateEntity> entity) {
  assert(entity);
  entities_.push_back(entity);
}

void EngineState::RemoveEntity(sp<StateEntity> entity) {
  for (vector<sp<StateEntity> >::iterator i = entities_.begin(); i != entities_.end(); i++) {
    if (*i == entity) {
      entities_.erase(i);
      break;
    }
  }
}
