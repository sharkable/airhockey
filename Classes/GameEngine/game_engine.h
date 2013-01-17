//
//  game_engine.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAMEENGINE_GAMEENGINE_H_
#define AIRHOCKEY_GAMEENGINE_GAMEENGINE_H_

#include <vector>
using namespace std;

#include "gameengine/touch.h"

class EngineView;

class GameEngine {
 public:
  void Update();
  void Render();
  void ClearTouches();

  void PushView(sp<EngineView> view);
  void PopView();
  void SetRootView(sp<EngineView> view);

  void set_touches_began(vector<Touch> touches_began) { touches_began_ = touches_began; }
  void set_touches_moved(vector<Touch> touches_moved) { touches_moved_ = touches_moved; }
  void set_touches_ended(vector<Touch> touches_ended) { touches_ended_ = touches_ended; }
 
 private:
  vector<sp<EngineView> > views_;
  vector<Touch> touches_began_;
  vector<Touch> touches_moved_;
  vector<Touch> touches_ended_;
  bool pop_on_next_;
  bool replace_on_next_;
  sp<EngineView> next_view_;
};

#endif
