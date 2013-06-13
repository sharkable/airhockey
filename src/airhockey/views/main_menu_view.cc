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

#include "gameengine/entities/simple_item.h"
#include "gameengine/modules/ad_engine.h"
#include "gameengine/modules/analytics_engine.h"
#include "gameengine/modules/local_store.h"
#include "gameengine/coordinate_types.h"
#include "gameengine/game_engine.h"
#include "gameengine/sprite.h"

#include "airhockey/entities/rink_overlay.h"
#include "airhockey/entities/sound_slider.h"
#include "airhockey/views/play_view.h"
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

MainMenuView::MainMenuView(sp<GameEngine> game_engine) : EngineView(game_engine) {
  InitializeSettings();
  state_ = kMainMenuStateRunning;

  rink_overlay_.reset(new RinkOverlay(game_engine));
  AddEntity(rink_overlay_);

  Sprite title_sprite(game_engine, "title");
  title_.reset(new SimpleItem(title_sprite, game_engine->position("title")));
  AddEntity(title_);
  title_->set_zoom(1.3);
  title_->AnimateToZoom(1, kAnimationTypeCubicEaseOut, 900);
  title_->set_alpha(0);
  title_->AnimateToAlpha(1, kAnimationTypeCubicEaseOut, 900);

  Sprite start_1_player_button_image(game_engine, "start_1_player_button");
  Sprite start_1_player_button_pressed_image(game_engine, "start_1_player_button_pressed");
  start_1_player_button_.reset(new Button());
  start_1_player_button_->set_normal_sprite(start_1_player_button_image);
  start_1_player_button_->set_pressed_sprite(start_1_player_button_pressed_image);
  start_1_player_button_->set_position(game_engine->position("start_1_player_button"));
  start_1_player_button_->set_delegate(this);
  AddEntity(start_1_player_button_);
  fade_in(start_1_player_button_.get());

  Sprite start_2_player_button_image(game_engine, "start_2_player_button");
  Sprite start_2_player_button_pressed_image(game_engine, "start_2_player_button_pressed");
  start_2_player_button_.reset(new Button());
  start_2_player_button_->set_normal_sprite(start_2_player_button_image);
  start_2_player_button_->set_pressed_sprite(start_2_player_button_pressed_image);
  start_2_player_button_->set_position(game_engine->position("start_2_player_button"));
  start_2_player_button_->set_delegate(this);
  AddEntity(start_2_player_button_);
  fade_in(start_2_player_button_.get());

  Sprite settings_button_image(game_engine, "settings_button");
  Sprite settings_button_pressed_image(game_engine, "settings_button_pressed");
  settings_button_.reset(new Button());
  settings_button_->set_normal_sprite(settings_button_image);
  settings_button_->set_pressed_sprite(settings_button_pressed_image);
  settings_button_->set_position(game_engine->position("settings_button"));
  settings_button_->set_delegate(this);
  AddEntity(settings_button_);
  fade_in(settings_button_.get());

  Sprite story_button_image(game_engine, "story_button");
  Sprite story_button_pressed_image(game_engine, "story_button_pressed");
  story_button_.reset(new Button());
  story_button_->set_normal_sprite(story_button_image);
  story_button_->set_pressed_sprite(story_button_pressed_image);
  story_button_->set_position(game_engine->position("story_button"));
  story_button_->set_delegate(this);
  AddEntity(story_button_);
  fade_in(story_button_.get());

  // TODO disabling this until I get app store selling sorted out.
  Sprite upgrade_button_image(game_engine, "upgrade_button");
  Sprite upgrade_button_pressed_image(game_engine, "upgrade_button_pressed");
  upgrade_button_.reset(new Button());
  upgrade_button_->set_normal_sprite(upgrade_button_image);
  upgrade_button_->set_pressed_sprite(upgrade_button_pressed_image);
  upgrade_button_->set_position(game_engine->position("upgrade_button"));
  upgrade_button_->set_delegate(this);
  if (!game_engine->local_store()->BoolForKey(kLocalStoreUpgraded)) {
    AddEntity(upgrade_button_);
  }
  fade_in(upgrade_button_.get());

  sound_slider_.reset(new SoundSlider(game_engine,
                                      game_engine->position("sound_slider_main_menu")));
  AddEntity(sound_slider_);
}


// EngineView

void MainMenuView::ViewIsShown() {
  // Force the popup for rating and upgrading just once.
  int main_menu_view_count =
      game_engine()->local_store()->IntegerForKey(kLocalStoreMainMenuViewCount) + 1;
  game_engine()->local_store()->SetInteger(main_menu_view_count, kLocalStoreMainMenuViewCount);
  if (main_menu_view_count == 10) {
    PressedUpgrade();
  }
}

void MainMenuView::Update() {
  EngineView::Update();
  if (state_ == kMainMenuStateAnimatingOut) {
    animating_out_ticks_left_--;
    // TODO This sometimes causes a brief overlap with the overlap in PlayView.
    if (animating_out_ticks_left_ == kAnimateOutTicks) {
      RemoveEntity(rink_overlay_);
    }
    if (animating_out_ticks_left_ <= 0) {
      game_engine()->RemoveView(this);
    }
  }
}


// AppStoreEngineDelegate

void MainMenuView::UpgradeSucceeded() {
  game_engine()->local_store()->SetBool(true, kLocalStoreUpgraded);
  RemoveEntity(upgrade_button_);
}


// ButtonDelegate

void MainMenuView::ButtonPressed(Button *button) {
  if (button == start_1_player_button_.get()) {
    PressedStart(1);
  } else if (button == start_2_player_button_.get()) {
    PressedStart(2);
  } else if (button == settings_button_.get()) {
    PressedSettings();
  } else if (button == story_button_.get()) {
    PressedStory();
  } else if (button == upgrade_button_.get()) {
    PressedUpgrade();
  }
}


// private

void MainMenuView::InitializeSettings() {
  if (!game_engine()->local_store()->HasEntryForKey(kLocalStoreDifficulty)) {
    game_engine()->local_store()->SetInteger(kComputerAIBad, kLocalStoreDifficulty);
    game_engine()->local_store()->SetInteger(kPaddleSizeMedium, kLocalStorePaddleSize);
  }
}

void MainMenuView::AnimateOut() {
  state_ = kMainMenuStateAnimatingOut;

  title_->AnimateToZoom(0, kAnimationTypeLinear, 15);
  fade_out(start_1_player_button_.get());
  fade_out(start_2_player_button_.get());
  fade_out(settings_button_.get());
  fade_out(story_button_.get());
  fade_out(upgrade_button_.get());
  RemoveEntity(sound_slider_);

  animating_out_ticks_left_ = kAnimateOutTicks;
}

void MainMenuView::PressedStart(int num_players) {
  if (game_engine()->platform_type() == kPlatformTypeTablet) {
    game_engine()->ad_engine()->RemoveAd();
  }

  // The stored number of pucks is one less than the desired value. Not ideal. This is for:
  // 1) Legacy.
  // 2) Defaults to 0, which means 1 puck.
  int num_pucks = game_engine()->local_store()->IntegerForKey(kLocalStoreNumPucks) + 1;
  ComputerAI difficulty =
      (ComputerAI)game_engine()->local_store()->IntegerForKey(kLocalStoreDifficulty);
  PaddleSize paddle_size =
      (PaddleSize)game_engine()->local_store()->IntegerForKey(kLocalStorePaddleSize);
  map<string, string> analytics_params;
  analytics_params["NumPlayers"] = num_players;
  analytics_params["NumPucks"] = num_pucks;
  analytics_params["Difficulty"] = difficulty;
  analytics_params["PaddleSize"] = paddle_size;
  game_engine()->analytics_engine()->LogEvent("START_GAME", analytics_params);

  PlayView *play_view = new PlayView(game_engine(),
                                     num_players,
                                     num_pucks,
                                     difficulty,
                                     paddle_size);
  AnimateOut();
  game_engine()->PushView(sp<EngineView>(play_view));
}

void MainMenuView::PressedSettings() {
  game_engine()->PushView(sp<SettingsView>(new SettingsView(game_engine())));
}

void MainMenuView::PressedStory() {
  game_engine()->analytics_engine()->LogEvent("STORY_PRESSED");
  if (game_engine()->platform_type() == kPlatformTypeTablet) {
    game_engine()->ad_engine()->RemoveAd();
  }
  game_engine()->PushView(sp<EngineView>(new StoryView(game_engine())));
}

void MainMenuView::PressedUpgrade() {
  if (!game_engine()->local_store()->BoolForKey(kLocalStoreUpgraded)) {
    game_engine()->app_store_engine()->AskForUpgrade("Glide Hockey HD", "GlideHockeyHDUpgrade",
                                                     this);
  }
}
