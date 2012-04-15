//
//  EngineState.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "gameengine/engine_state.h"

#import "gameengine/game_engine.h"
#import "gameengine/StateEntity.h"

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

void EngineState::AddEntity(StateEntity &entity) {
  entities_.push_back(&entity);
}

void EngineState::RemoveEntity(StateEntity &entity) {
  for (vector<StateEntity *>::iterator i = entities_.begin(); i != entities_.end(); i++) {
    if (*i == &entity) {
      entities_.erase(i);
      break;
    }
  }
}