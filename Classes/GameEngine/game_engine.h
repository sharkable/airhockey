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

struct GameSize {
  double width;
  double height;
};
inline GameSize game_size_make(double width, double height) {
  GameSize size;
  size.width = width;
  size.height = height;
  return size;
}

struct GamePoint {
  double x;
  double y;
};
inline GamePoint game_point_make(double x, double y) {
  GamePoint point;
  point.x = x;
  point.y = y;
  return point;
}

class GameEngine {
 public:
  void SetScreenSize(ScreenSize screen_size, GameSize game_size);

  void Update();
  void Render();
  void ClearTouches();

  void PushView(sp<EngineView> view);
  void PopView();
  void SetRootView(sp<EngineView> view);

  void set_touches_began(vector<Touch> touches_began) { touches_began_ = touches_began; }
  void set_touches_moved(vector<Touch> touches_moved) { touches_moved_ = touches_moved; }
  void set_touches_ended(vector<Touch> touches_ended) { touches_ended_ = touches_ended; }

  ScreenSize game_size_to_screen_size(GameSize game_size_) {
    return screen_size_make(game_size_.width * game_to_screen_point_ratio_x_,
                            game_size_.height * game_to_screen_point_ratio_y_);
  }
  ScreenPoint game_point_to_screen_point(GamePoint game_point_) {
    return screen_point_make(game_point_.x * game_to_screen_point_ratio_x_,
                             game_point_.y * game_to_screen_point_ratio_y_);
  }
  GameSize screen_size_to_game_size(ScreenSize screen_size_) {
    return game_size_make(screen_size_.width * screen_to_game_point_ratio_x_,
                          screen_size_.height * screen_to_game_point_ratio_y_);
  }
  GamePoint screen_point_to_game_point(ScreenPoint screen_point_) {
    return game_point_make(screen_point_.x * screen_to_game_point_ratio_x_,
                           screen_point_.y * screen_to_game_point_ratio_y_);
  }

 private:
  vector<sp<EngineView> > views_;
  vector<Touch> touches_began_;
  vector<Touch> touches_moved_;
  vector<Touch> touches_ended_;
  bool pop_on_next_;
  bool replace_on_next_;
  sp<EngineView> next_view_;
  double game_to_screen_point_ratio_x_;
  double game_to_screen_point_ratio_y_;
  double screen_to_game_point_ratio_x_;
  double screen_to_game_point_ratio_y_;
};

#endif
