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
#include "gameengine/engine_view.h"

class GameEngine;
class MultiSelect;
class SimpleItem;

// Local Store keys
extern const std::string kLocalStoreDifficulty;
extern const std::string kLocalStoreNumPucks;
extern const std::string kLocalStorePaddleSize;

class SettingsView : public EngineView, private AnimatableDelegate, private ButtonDelegate {
 public:
  SettingsView(GameEngine *game_engine);

  // EngineView
  bool IsCapturingTouches();
  bool HandleBackButton();

  // Animatable
  void AnimationFinished(Animatable *animatable);

  // ButtonDelegate
  void ButtonUp(Button *button);

 private:
  CompositeEntity *entities_;  // weak
  SimpleItem *background_;  // weak
  MultiSelect *num_pucks_select_;  // weak
  MultiSelect *difficulty_select_;  // weak
  MultiSelect *paddle_size_select_;  // weak
  Button *ok_button_;  // weak
  GamePoint ending_position_;
};

#endif
