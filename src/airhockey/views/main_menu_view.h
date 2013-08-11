//
//  main_menu_view.h
//  AirHockey
//
//  Created by Jon Sharkey on 2010-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_VIEWS_MAINMENUVIEW_H_
#define AIRHOCKEY_VIEWS_MAINMENUVIEW_H_

#include <string>

#include "gameengine/entities/button.h"
#include "gameengine/modules/app_store_engine.h"
#include "gameengine/engine_view.h"

class RinkOverlay;
class SimpleItem;
class SoundSlider;

typedef enum {
  kMainMenuStateRunning = 0,
  kMainMenuStateAnimatingOut
} MainMenuState;

// Local Store keys
extern const std::string kLocalStoreUpgraded;

class MainMenuView : public EngineView, private AppStoreEngineDelegate, private ButtonDelegate {
 public:
  MainMenuView(GameEngine *game_engine);

  // EngineView
  void ViewDidGainFocus();
  void Update();

  // AppStoreEngineDelegate
  void UpgradeSucceeded();

  // ButtonDelegate
  void ButtonPressed(Button *button);

 private:
  void InitializeSettings();
  void AnimateOut();
  void PressedStart(int num_players);
  void PressedSettings();
  void PressedStory();
  void PressedUpgrade();

  sp<RinkOverlay> rink_overlay_;
  sp<SimpleItem> title_;
  sp<Button> start_1_player_button_;
  sp<Button> start_2_player_button_;
  sp<Button> settings_button_;
  sp<Button> story_button_;
  sp<Button> upgrade_button_;
  sp<SoundSlider> sound_slider_;

  MainMenuState state_;
  int animating_out_ticks_left_;
  bool show_upgrade_button_;
  bool supports_2_player_;
};

#endif
