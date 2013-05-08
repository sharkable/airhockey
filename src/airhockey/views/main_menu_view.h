//
//  main_menu_view.h
//  AirHockey
//
//  Created by Jon Sharkey on 2010-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_VIEWS_MAINMENUVIEW_H_
#define AIRHOCKEY_VIEWS_MAINMENUVIEW_H_

#include "gameengine/engine_view.h"
#include "gameengine/entities/button.h"

class MultiSelect;
class RinkOverlay;
class SimpleItem;
class SoundSlider;

typedef enum {
  kMainMenuStateRunning = 0,
  kMainMenuStateAnimatingOut
} MainMenuState;

class MainMenuView : public EngineView, private ButtonDelegate {
 public:
  MainMenuView(sp<GameEngine> game_engine);

  // EngineView
  void ViewIsShown();
  void Update();

  // ButtonDelegate
  void ButtonPressed(Button *button);

 private:
  void AnimateOut();
  void PressedStart();
  void PressedStory();
  void PressedRate();
  void PressedUpgrade();

  sp<RinkOverlay> rink_overlay_;
  sp<SimpleItem> title_;
  sp<SimpleItem> main_menu_;
  sp<Button> start_button_;
  sp<Button> story_button_;
  sp<Button> rate_button_;
//  sp<Button> upgrade_button_;
  sp<MultiSelect> num_players_select_;
  sp<MultiSelect> num_pucks_select_;
  sp<MultiSelect> difficulty_select_;
  sp<MultiSelect> paddle_size_select_;
  sp<SoundSlider> sound_slider_;

  MainMenuState state_;
  int animating_out_ticks_left_;
};

#endif
