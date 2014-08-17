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

#include "sharkengine/entities/button.h"
#include "sharkengine/input/input_handler.h"
#include "sharkengine/modules/app_store_module.h"
#include "sharkengine/render/renderer.h"
#include "sharkengine/simulation/simulator.h"

#include "airhockey/views/play_view.h"
#include "airhockey/views/settings_view.h"
#include "airhockey/views/story_view.h"

class GameEngine;
class GameView;
class SimpleItem;
class SoundSlider;

typedef enum {
  kMainMenuStateRunning = 0,
  kMainMenuStateAnimatingOut
} MainMenuState;

// Local Store keys
extern const std::string kLocalStoreUpgraded;

class MainMenuView : public Simulator, public Renderer, public InputHandler,
    private StoryViewDelegate, private SettingsViewDelegate, private AppStoreModuleDelegate,
    private ButtonDelegate {
 public:
  MainMenuView(GameEngine &game_engine, GameView &game_view);
  ~MainMenuView();

  // TODO?
  void ViewDidGainFocus();

  // Simulator
  virtual void SimulateStep();

  // Renderer
  virtual void Render(CoordinateSystem const &coordinate_system);

  // InputHandler
  virtual bool HandleInputEvent(InputEvent const &event, CoordinateSystem const &coordinate_system);

  // SettingsViewDelegate
  virtual void SettingsViewFinished();

  // StoryViewDelegate
  virtual void StoryViewFinished();

  // AppStoreModuleDelegate
  virtual void UpgradeSucceeded();

  // ButtonDelegate
  void ButtonUp(Button *button);

 private:
  void InitializeSettings();
  void AnimateOut();
  void PressedStart(int num_players);
  void PressedSettings();
  void PressedStory();
  void PressedUpgrade();

  GameEngine &game_engine_;
  GameView &game_view_;

  SettingsView *settings_view_;
  StoryView *story_view_;

  SimpleItem *title_;
  Button *start_1_player_button_;
  Button *start_2_player_button_;
  Button *settings_button_;
  Button *story_button_;
  Button *upgrade_button_;
  SoundSlider *sound_slider_;

  MainMenuState state_;
  int animating_out_ticks_left_;
  bool show_upgrade_button_;
  bool supports_2_player_;
};

#endif
