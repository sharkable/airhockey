//
//  game_view.h
//  AirHockey_osx
//
//  Created by Jon Sharkey on 2013-12-18.
//  Copyright 2013 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_VIEWS_GAMEVIEW_H_
#define AIRHOCKEY_VIEWS_GAMEVIEW_H_

#include "gameengine/engine_view.h"

#include "airhockey/entities/rink_overlay.h"
#include "airhockey/views/rink_view.h"
#include "airhockey/views/main_menu_view.h"

class GameEngine;

class GameView : public EngineView {
 public:
  GameView(GameEngine &game_engine);

  // EngineView
  virtual void Update();
  virtual void Render();
  virtual void TouchesBegan(std::vector<Touch> &touches);
  virtual void TouchesMoved(std::vector<Touch> const &touches);
  virtual void TouchesEnded(std::vector<Touch> const &touches);
  virtual void ClearTouches();

 private:
  RinkView rink_background_;
  RinkOverlay rink_overlay_;
  MainMenuView *main_menu_view_;
};

#endif
