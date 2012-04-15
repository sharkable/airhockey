//
//  EngineState.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAMEENGINE_ENGINESTATE_H_
#define AIRHOCKEY_GAMEENGINE_ENGINESTATE_H_

#include <vector>
using namespace std;

#include "gameengine/touch.h"

class GameEngine;
class StateEntity;

class EngineState {
 public:
  EngineState(GameEngine &game_engine);

  virtual void StateIsShown() {}
  virtual void Update();
  virtual void Render();
  virtual void TouchesBegan(vector<Touch> touches);
  virtual void TouchesMoved(vector<Touch> touches);
  virtual void TouchesEnded(vector<Touch> touches);
  virtual void ClearTouches();
  void AddEntity(StateEntity &entity);
  void RemoveEntity(StateEntity &entity);

  GameEngine &game_engine() { return game_engine_; }

 protected:

 private:
  GameEngine &game_engine_;
  vector<StateEntity *> entities_;
};

#endif
