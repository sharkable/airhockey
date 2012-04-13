//
//  EngineState.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include <vector>
using namespace std;

#include "StateEntity.h"
#include "Touch.h"

class GameEngine;

class EngineState {
 public:
  GameEngine *getGameEngine() { return gameEngine_; }
  virtual void stateIsShown();
  virtual void update();
  virtual void render();
  void addEntity(StateEntity *entity);
  void removeEntity(StateEntity *entity);
  virtual void touchesBegan(vector<Touch> touches);
  virtual void touchesMoved(vector<Touch> touches);
  virtual void touchesEnded(vector<Touch> touches);
  virtual void clearTouches();

 protected:
  EngineState(GameEngine *gameEngine);

 private:
  vector<StateEntity *> entities_;
  GameEngine *gameEngine_;
};
