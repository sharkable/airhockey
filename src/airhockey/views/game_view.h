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

#include "airhockey/entities/paddle.h"
#include "airhockey/entities/rink_overlay.h"
#include "airhockey/views/rink_view.h"

class GameEngine;
class MainMenuView;
class PlayView;

class GameView : public Simulator, public Renderer, public InputHandler {
 public:
  GameView(GameEngine &game_engine);

  void ShowMainMenu();
  void RemoveMainMenu();
  void ShowPlay(int num_players, int num_pucks, ComputerAI difficulty, PaddleSize paddle_size);
  void RemovePlay();

  // Simulator
  virtual void SimulateStep();

  // Renderer
  virtual void Render(CoordinateSystem const &coordinate_system);

  // InputHandler
  virtual bool HandleInputEvent(InputEvent const &event,
                                CoordinateSystem const &coordinate_system);

 private:
  GameEngine &game_engine_;
  RinkView rink_background_;
  RinkOverlay rink_overlay_;
  MainMenuView *main_menu_view_;
  PlayView *play_view_;
};

#endif
