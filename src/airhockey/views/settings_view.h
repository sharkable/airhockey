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

#include "gameengine/entities/animatable.h"
#include "gameengine/entities/button.h"
#include "gameengine/entities/composite_entity.h"
#include "gameengine/input/input_handler.h"
#include "gameengine/render/renderer.h"
#include "gameengine/simulation/simulator.h"

class GameEngine;
class MultiSelect;
class SimpleItem;

// Local Store keys
extern const std::string kLocalStoreDifficulty;
extern const std::string kLocalStoreNumPucks;
extern const std::string kLocalStorePaddleSize;

class SettingsView : public Simulator, public Renderer, public InputHandler,
    private AnimatableDelegate, private ButtonDelegate {
 public:
  SettingsView(GameEngine &game_engine);

  // TODO bool HandleBackButton();

  // Simulator
  void SimulateStep();

  // Renderer
  void Render(CoordinateSystem const &coordinate_system);

  // InputHandler
  bool HandleEvent(InputEvent const &event);

  // Animatable
  void AnimationFinished(Animatable *animatable);

  // ButtonDelegate
  void ButtonUp(Button *button);

 private:
  GameEngine &game_engine_;
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
