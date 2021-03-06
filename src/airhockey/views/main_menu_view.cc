//
//  main_menu_view.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2010-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "airhockey/views/main_menu_view.h"

#include <map>
#include <sstream>
using std::map;
using std::string;

#include "sharkengine/coordinates/coordinate_types.h"
#include "sharkengine/entities/simple_item.h"
#include "sharkengine/modules/ad_module.h"
#include "sharkengine/modules/analytics_module.h"
#include "sharkengine/modules/persistence_module.h"
#include "sharkengine/platform.h"
#include "sharkengine/shark_engine.h"
#include "sharkengine/sprite.h"

#include "airhockey/entities/rink_overlay.h"
#include "airhockey/entities/sound_slider.h"
#include "airhockey/views/game_view.h"
#include "airhockey/views/settings_view.h"
#include "airhockey/views/story_view.h"

static const int kAnimateOutTicks = 15;

// Local Store keys
const string kLocalStoreMainMenuViewCount = "ls_main_menu_view_count";
const string kLocalStoreUpgraded = "ls_upgraded";

inline string to_string(int i) {
  std::stringstream ss;
  ss << i;
  return ss.str();
}

void fade_in(Animatable *entity) {
  entity->set_alpha(0);
  entity->AnimateToAlpha(1, kAnimationTypeLinear, 15);
}

void fade_out(Animatable *entity) {
  entity->AnimateToAlpha(0, kAnimationTypeLinear, 15);
}

MainMenuView::MainMenuView(SharkEngine &shark_engine, GameView &game_view)
    : shark_engine_(shark_engine),
      game_view_(game_view),
      story_view_(NULL),
      settings_view_(NULL),
      title_(NULL),
      start_1_player_button_(NULL),
      start_2_player_button_(NULL),
      settings_button_(NULL),
      story_button_(NULL),
      upgrade_button_(NULL),
      sound_slider_(NULL) {
  show_upgrade_button_ = shark_engine.app_store_module()->IsImplemented() &&
      shark_engine.persistence_module()->BoolForKey(kLocalStoreUpgraded);
  supports_2_player_ = shark_engine.platform().input_group() == Platform::kInputGroupTouchScreen;

  InitializeSettings();
  state_ = kMainMenuStateRunning;

  Sprite title_sprite(shark_engine, "title", Sprite::kAnchorCenter);
  title_ =
      new SimpleItem(title_sprite,
                     shark_engine.position("title") + GamePoint(title_sprite.content_size()) / 2);
  title_->set_scale(1.3);
  title_->AnimateToScale(1, kAnimationTypeCubicEaseOut, 900);
  title_->set_alpha(0);
  title_->AnimateToAlpha(1, kAnimationTypeCubicEaseOut, 900);

  char const *start_button;
  char const *start_button_pressed;
  if (supports_2_player_) {
    start_button = "start_1_player_button";
    start_button_pressed = "start_1_player_button_pressed";
  } else {
    start_button = "start_button";
    start_button_pressed = "start_button_pressed";
  }
  Sprite start_1_player_button_image(shark_engine, start_button);
  Sprite start_1_player_button_pressed_image(shark_engine, start_button_pressed);
  start_1_player_button_ = new Button(shark_engine);
  start_1_player_button_->set_normal_sprite(start_1_player_button_image);
  start_1_player_button_->set_pressed_sprite(start_1_player_button_pressed_image);
  GamePoint player_1_position = shark_engine.position("start_1_player_button");
  if (!show_upgrade_button_) {
    player_1_position.y += 90;
  }
  start_1_player_button_->set_position(player_1_position);
  start_1_player_button_->set_delegate(this);
  fade_in(start_1_player_button_);

  if (supports_2_player_) {
    Sprite start_2_player_button_image(shark_engine, "start_2_player_button");
    Sprite start_2_player_button_pressed_image(shark_engine, "start_2_player_button_pressed");
    start_2_player_button_ = new Button(shark_engine);
    start_2_player_button_->set_normal_sprite(start_2_player_button_image);
    start_2_player_button_->set_pressed_sprite(start_2_player_button_pressed_image);
    GamePoint player_2_position = shark_engine.position("start_2_player_button");
    if (!show_upgrade_button_) {
      player_2_position.y += 90;
    }
    start_2_player_button_->set_position(player_2_position);
    start_2_player_button_->set_delegate(this);
    fade_in(start_2_player_button_);
  } else {
    start_2_player_button_ = NULL;
  }

  Sprite settings_button_image(shark_engine, "settings_button");
  Sprite settings_button_pressed_image(shark_engine, "settings_button_pressed");
  settings_button_ = new Button(shark_engine);
  settings_button_->set_normal_sprite(settings_button_image);
  settings_button_->set_pressed_sprite(settings_button_pressed_image);
  settings_button_->set_position(shark_engine.position("settings_button"));
  settings_button_->set_delegate(this);
  fade_in(settings_button_);

  Sprite story_button_image(shark_engine, "story_button");
  Sprite story_button_pressed_image(shark_engine, "story_button_pressed");
  story_button_ = new Button(shark_engine);
  story_button_->set_normal_sprite(story_button_image);
  story_button_->set_pressed_sprite(story_button_pressed_image);
  story_button_->set_position(shark_engine.position("story_button"));
  story_button_->set_delegate(this);
  fade_in(story_button_);

  if (show_upgrade_button_) {
    Sprite upgrade_button_image(shark_engine, "upgrade_button");
    Sprite upgrade_button_pressed_image(shark_engine, "upgrade_button_pressed");
    upgrade_button_ = new Button(shark_engine);
    upgrade_button_->set_normal_sprite(upgrade_button_image);
    upgrade_button_->set_pressed_sprite(upgrade_button_pressed_image);
    upgrade_button_->set_position(shark_engine.position("upgrade_button"));
    upgrade_button_->set_delegate(this);
    fade_in(upgrade_button_);
  } else {
    upgrade_button_ = NULL;
  }

  sound_slider_ = new SoundSlider(shark_engine, shark_engine.position("sound_slider_main_menu"));
}

MainMenuView::~MainMenuView() {
  delete title_;
  delete start_1_player_button_;
  if (start_2_player_button_) {
    delete start_2_player_button_;
  }
  delete settings_button_;
  delete story_button_;
  if (upgrade_button_) {
    delete upgrade_button_;
  }
  if (sound_slider_) {
    delete sound_slider_;
  }
}


// TODO NOW
void MainMenuView::ViewDidGainFocus() {
  if (show_upgrade_button_) {
    // Force the popup for rating and upgrading just once.
    int main_menu_view_count =
        shark_engine_.persistence_module()->IntegerForKey(kLocalStoreMainMenuViewCount) + 1;
    shark_engine_.persistence_module()->SetInteger(main_menu_view_count,
                                                   kLocalStoreMainMenuViewCount);
    if (main_menu_view_count == 10) {
      PressedUpgrade();
    }
  }
}


#pragma mark - Simulator

void MainMenuView::SimulateStep() {
  if (state_ == kMainMenuStateAnimatingOut) {
    animating_out_ticks_left_--;
    // TODO This sometimes causes a brief overlap with the overlap in PlayView.
    if (animating_out_ticks_left_ == kAnimateOutTicks) {
//      RemoveEntity(rink_overlay_);
    }
    if (animating_out_ticks_left_ <= 0) {
      game_view_.RemoveMainMenu();
      return;
    }
  }

  title_->SimulateStep();
  start_1_player_button_->SimulateStep();
  if (start_2_player_button_) {
    start_2_player_button_->SimulateStep();
  }
  settings_button_->SimulateStep();
  story_button_->SimulateStep();
  if (upgrade_button_) {
    upgrade_button_->SimulateStep();
  }
  if (story_view_) {
    story_view_->SimulateStep();
  }
  if (settings_view_) {
    settings_view_->SimulateStep();
  }
}


#pragma mark - Renderer

void MainMenuView::Render(CoordinateSystem const &coordinate_system) {
  title_->Render(coordinate_system);
  start_1_player_button_->Render(coordinate_system);
  if (start_2_player_button_) {
    start_2_player_button_->Render(coordinate_system);
  }
  settings_button_->Render(coordinate_system);
  story_button_->Render(coordinate_system);
  if (upgrade_button_) {
    upgrade_button_->Render(coordinate_system);
  }
  if (sound_slider_) {
    sound_slider_->Render(coordinate_system);
  }
  if (story_view_) {
    story_view_->Render(coordinate_system);
  }
  if (settings_view_) {
    settings_view_->Render(coordinate_system);
  }
}


#pragma mark - InputHandler

bool MainMenuView::HandleInputEvent(InputEvent const &event,
                                    CoordinateSystem const &coordinate_system) {
  if (story_view_ && story_view_->HandleInputEvent(event, coordinate_system)) {
    return true;
  }
  if (settings_view_ && settings_view_->HandleInputEvent(event, coordinate_system)) {
    return true;
  }
  if (start_1_player_button_->HandleInputEvent(event, coordinate_system)) {
    return true;
  }
  if (start_2_player_button_ &&
      start_2_player_button_->HandleInputEvent(event, coordinate_system)) {
    return true;
  }
  if (settings_button_->HandleInputEvent(event, coordinate_system)) {
    return true;
  }
  if (story_button_->HandleInputEvent(event, coordinate_system)) {
    return true;
  }
  if (upgrade_button_ && upgrade_button_->HandleInputEvent(event, coordinate_system)) {
    return true;
  }
  if (sound_slider_ && sound_slider_->HandleInputEvent(event, coordinate_system)) {
    return true;
  }
  return false;
}


#pragma mark - SettingsViewDelegate

void MainMenuView::SettingsViewFinished() {
  delete settings_view_;
  settings_view_ = NULL;
}


#pragma mark - StoryViewDelegate

void MainMenuView::StoryViewFinished() {
  delete story_view_;
  story_view_ = NULL;
}


#pragma mark - AppStoreModuleDelegate

void MainMenuView::UpgradeSucceeded() {
  shark_engine_.persistence_module()->SetBool(true, kLocalStoreUpgraded);
  if (upgrade_button_) {
    delete upgrade_button_;
    upgrade_button_ = NULL;
  }
}


#pragma mark - ButtonDelegate

void MainMenuView::ButtonUp(Button *button) {
  if (button == start_1_player_button_) {
    PressedStart(1);
  } else if (button == start_2_player_button_) {
    PressedStart(2);
  } else if (button == settings_button_) {
    PressedSettings();
  } else if (button == story_button_) {
    PressedStory();
  } else if (button == upgrade_button_) {
    PressedUpgrade();
  }
}


#pragma mark - private

void MainMenuView::InitializeSettings() {
  if (!shark_engine_.persistence_module()->HasEntryForKey(kLocalStoreDifficulty)) {
    shark_engine_.persistence_module()->SetInteger(kComputerAIBad, kLocalStoreDifficulty);
    PaddleSize default_size = kPaddleSizeMedium;
    if (shark_engine_.platform().screen_size_group() == Platform::kScreenSizeGroupPhone) {
      default_size = kPaddleSizeLarge;
    }
    shark_engine_.persistence_module()->SetInteger(default_size, kLocalStorePaddleSize);
  }
}

void MainMenuView::AnimateOut() {
  state_ = kMainMenuStateAnimatingOut;

  title_->AnimateToScale(0, kAnimationTypeLinear, 15);
  fade_out(start_1_player_button_);
  if (supports_2_player_) {
    fade_out(start_2_player_button_);
  }
  fade_out(settings_button_);
  fade_out(story_button_);
  if (show_upgrade_button_) {
    fade_out(upgrade_button_);
  }
  if (sound_slider_) {
    delete sound_slider_;
    sound_slider_ = NULL;
  }

  animating_out_ticks_left_ = kAnimateOutTicks;
}

void MainMenuView::PressedStart(int num_players) {
  // The stored number of pucks is one less than the desired value. Not ideal. This is for:
  // 1) Legacy.
  // 2) Defaults to 0, which means 1 puck.
  int num_pucks = shark_engine_.persistence_module()->IntegerForKey(kLocalStoreNumPucks) + 1;
  ComputerAI difficulty =
      (ComputerAI)shark_engine_.persistence_module()->IntegerForKey(kLocalStoreDifficulty);
  PaddleSize paddle_size =
      (PaddleSize)shark_engine_.persistence_module()->IntegerForKey(kLocalStorePaddleSize);
  map<string, string> analytics_params;
  analytics_params["NumPlayers"] = to_string(num_players);
  analytics_params["NumPucks"] = to_string(num_pucks);
  analytics_params["Difficulty"] = to_string(difficulty);
  analytics_params["PaddleSize"] = to_string(paddle_size);
  shark_engine_.analytics_module()->LogEvent("START_GAME", analytics_params);

  game_view_.ShowPlay(num_players, num_pucks, difficulty, paddle_size);
  AnimateOut();
}

void MainMenuView::PressedSettings() {
  if (settings_view_) {
    delete settings_view_;
  }
  settings_view_ = new SettingsView(shark_engine_, *this);
}

void MainMenuView::PressedStory() {
  shark_engine_.analytics_module()->LogEvent("STORY_PRESSED");
  story_view_ = new StoryView(shark_engine_, *this);
}

void MainMenuView::PressedUpgrade() {
  if (!shark_engine_.persistence_module()->BoolForKey(kLocalStoreUpgraded)) {
    shark_engine_.app_store_module()->AskForUpgrade("Glide Hockey HD", "GlideHockeyHDUpgrade",
                                                   this);
  }
}
