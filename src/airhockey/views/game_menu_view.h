//
//  game_menu_view.h
//  AirHockey
//
//  Created by Jon Sharkey on 2013-03-04.
//  Copyright 2013 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_VIEWS_GAMEMENUVIEW_H_
#define AIRHOCKEY_VIEWS_GAMEMENUVIEW_H_

#include "sharkengine/entities/button.h"
#include "sharkengine/input/input_handler.h"
#include "sharkengine/render/renderer.h"
#include "sharkengine/simulation/simulator.h"

class SimpleItem;
class SoundSlider;

class GameMenuViewDelegate {
 public:
  virtual void RematchPressed() = 0;
  virtual void MenuPressed() = 0;
  virtual void ContinuePressed() = 0;
};

class GameMenuView : public Simulator, public Renderer, public InputHandler,
    private ButtonDelegate {
 public:
  GameMenuView(SharkEngine &shark_engine, GameMenuViewDelegate *delegate, bool match_finished);
  ~GameMenuView();

//  bool HandleBackButton();
//  void HandlePauseButton();

  // Simulator
  virtual void SimulateStep();

  // Renderer
  virtual void Render(CoordinateSystem const &coordinate_system);

  // InputHandler
  virtual bool HandleInputEvent(InputEvent const &event, CoordinateSystem const &coordinate_system);

  // ButtonDelegate
  void ButtonUp(Button *button);

 private:
  void Init(bool match_finished);

  SharkEngine &shark_engine_;
  GameMenuViewDelegate *delegate_;
  SoundSlider *sound_slider_;
  SimpleItem *menu_background_;
  Button *rematch_button_;
  Button *menu_button_;
  Button *continue_button_;
};

#endif
