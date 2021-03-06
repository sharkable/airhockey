//
//  settings_view.h
//  AirHockey
//
//  Created by Jon Sharkey on 2013-05-08.
//  Copyright 2013 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_VIEWS_SETTINGSVIEW_
#define AIRHOCKEY_VIEWS_SETTINGSVIEW_

#include <string>

#include "sharkengine/entities/button.h"
#include "sharkengine/input/input_handler.h"
#include "sharkengine/render/renderer.h"
#include "sharkengine/simulation/simulator.h"

class MultiSelect;
class SharkEngine;
class SimpleItem;

// Local Store keys
extern const std::string kLocalStoreDifficulty;
extern const std::string kLocalStoreNumPucks;
extern const std::string kLocalStorePaddleSize;

class SettingsViewDelegate {
 public:
  virtual void SettingsViewFinished() = 0;
};

class SettingsView : public Simulator, public Renderer, public InputHandler,
    private ButtonDelegate {
 public:
  SettingsView(SharkEngine &shark_engine, SettingsViewDelegate &delegate);

  // TODO bool HandleBackButton();

  // Simulator
  void SimulateStep();

  // Renderer
  void Render(CoordinateSystem const &coordinate_system);

  // InputHandler
  bool HandleInputEvent(InputEvent const &event, CoordinateSystem const &coordinate_system);

  // ButtonDelegate
  void ButtonUp(Button *button);

 private:
  SharkEngine &shark_engine_;
  SettingsViewDelegate &delegate_;
  float x_position_;
  Animation x_position_animation_;
  SimpleItem *background_;
  MultiSelect *num_pucks_select_;
  MultiSelect *difficulty_select_;
  MultiSelect *paddle_size_select_;
  Button *ok_button_;
  GamePoint ending_position_;
};

#endif
