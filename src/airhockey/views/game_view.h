//
//  game_view.h
//  AirHockey_osx
//
//  Created by Jon Sharkey on 2013-12-18.
//  Copyright 2013 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_VIEWS_GAMEVIEW_H_
#define AIRHOCKEY_VIEWS_GAMEVIEW_H_

#include "gameengine/input/input_handler.h"
#include "gameengine/render/renderer.h"
#include "gameengine/simulation/simulator.h"

#include "airhockey/entities/rink_overlay.h"
#include "airhockey/views/rink_view.h"
#include "airhockey/views/main_menu_view.h"

class GameEngine;

class GameView : public Simulator, public Renderer, public InputHandler {
 public:
  GameView(GameEngine &game_engine);

  // Simulator
  virtual void SimulateStep();

  // Renderer
  virtual void Render(CoordinateSystem const &coordinate_system);

  // InputHandler
  virtual bool HandleInputEvent(InputEvent const &event);

 private:
  RinkView rink_background_;
  RinkOverlay rink_overlay_;
  MainMenuView *main_menu_view_;
};

#endif
