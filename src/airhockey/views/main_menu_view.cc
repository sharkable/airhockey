//
//  main_menu_view.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2010-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "airhockey/views/main_menu_view.h"

#include <sstream>

#include "gameengine/entities/multi_select.h"
#include "gameengine/entities/simple_item.h"
#include "gameengine/ad_engine.h"
#include "gameengine/analytics_engine.h"
#include "gameengine/coordinate_types.h"
#include "gameengine/game_engine.h"
#include "gameengine/local_store.h"
#include "gameengine/sprite.h"

#include "airhockey/entities/sound_slider.h"
#include "airhockey/views/play_view.h"
#include "airhockey/views/story_view.h"
#include "airhockey/const.h"

using std::map;
using std::string;

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
  bool is_iphone = false;  // TODO UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone;

  state_ = kMainMenuStateRunning;

  Sprite title_sprite(game_engine, "title");
  title_.reset(new SimpleItem(title_sprite, game_engine->position("title")));
  AddEntity(title_);
  title_->set_zoom(1.3);
  title_->AnimateToZoom(1, kAnimationTypeCubicEaseOut, 900);
  title_->set_alpha(0);
  title_->AnimateToAlpha(1, kAnimationTypeCubicEaseOut, 900);

  Sprite main_menu_sprite(game_engine, "main_menu");
  GamePoint main_menu_position = game_engine->position("main_menu");
  main_menu_.reset(new SimpleItem(main_menu_sprite, main_menu_position));
  AddEntity(main_menu_);
  main_menu_->set_alpha(0);
  main_menu_->AnimateToAlpha(1, kAnimationTypeLinear, 45);

  Sprite start_button_image(game_engine, "start_button");
  Sprite start_button_pressed_image(game_engine, "start_button_pressed");
  start_button_.reset(new Button());
  start_button_->set_normal_sprite(start_button_image);
  start_button_->set_pressed_sprite(start_button_pressed_image);
  start_button_->set_position(game_engine->position("start_button"));
  start_button_->set_delegate(this);
  AddEntity(start_button_);
  fade_in(start_button_.get());
  
  Sprite story_button_image(game_engine, "story_button");
  Sprite story_button_pressed_image(game_engine, "story_button_pressed");
  story_button_.reset(new Button());
  story_button_->set_normal_sprite(story_button_image);
  story_button_->set_pressed_sprite(story_button_pressed_image);
  story_button_->set_position(game_engine->position("story_button"));
  story_button_->set_delegate(this);
  AddEntity(story_button_);
  fade_in(story_button_.get());

  Sprite one_player_image(game_engine, "1_player");
  Sprite two_player_image(game_engine, "2_player");
  Sprite one_player_selected_image(game_engine, "1_player_selected");
  Sprite two_player_selected_image(game_engine, "2_player_selected");
  num_players_select_.reset(new MultiSelect());
  num_players_select_->Add(one_player_image, one_player_selected_image,
                           game_engine->position("1_player"));
  num_players_select_->Add(two_player_image, two_player_selected_image,
                           game_engine->position("2_player"));
  num_players_select_->set_selected_value(LocalStore::IntegerForKey(LS_NUM_PLAYERS));
  AddEntity(num_players_select_);
  fade_in(num_players_select_.get());

  num_pucks_select_.reset(new MultiSelect());
  for (int i = 1; i <= MAX_NUM_PUCKS; i++) {
    char pucks_str[15];
    sprintf(pucks_str, "%d", i);
    char pucks_selected_str[15];
    sprintf(pucks_selected_str, "%d_selected", i);

    Sprite num_pucks_image(game_engine, pucks_str);
    Sprite num_pucks_selected_image(game_engine, pucks_selected_str);
    GamePoint num_pucks_position = game_engine->position(pucks_str);
    num_pucks_select_->Add(num_pucks_image, num_pucks_selected_image, num_pucks_position);
  }
  num_pucks_select_->set_selected_value(LocalStore::IntegerForKey(LS_NUM_PUCKS));
  AddEntity(num_pucks_select_);
  fade_in(num_pucks_select_.get());

  Sprite bad_image(game_engine, "bad");
  Sprite bad_image_selected(game_engine, "bad_selected");
  Sprite good_image(game_engine, "good");
  Sprite good_image_selected(game_engine, "good_selected");
  Sprite excellent_image(game_engine, "excellent");
  Sprite excellent_image_selected(game_engine, "excellent_selected");
  Sprite amazing_image(game_engine, "amazing");
  Sprite amazing_image_selected(game_engine, "amazing_selected");
  difficulty_select_.reset(new MultiSelect());
  difficulty_select_->Add(bad_image, bad_image_selected, game_engine->position("bad"));
  difficulty_select_->Add(good_image, good_image_selected, game_engine->position("good"));
  difficulty_select_->Add(excellent_image, excellent_image_selected,
                          game_engine->position("excellent"));
  difficulty_select_->Add(amazing_image, amazing_image_selected, game_engine->position("amazing"));
  if (LocalStore::HasEntryForKey(LS_DIFFICULTY)) {
    difficulty_select_->set_selected_value(LocalStore::IntegerForKey(LS_DIFFICULTY));
  } else {
    difficulty_select_->set_selected_value(caiGood);
  }
  AddEntity(difficulty_select_);
  fade_in(difficulty_select_.get());

  if (!is_iphone) {
    Sprite small_image(game_engine, "small");
    Sprite small_image_selected(game_engine, "small_selected");
    Sprite medium_image(game_engine, "medium");
    Sprite medium_image_selected(game_engine, "medium_selected");
    Sprite large_image(game_engine, "large");
    Sprite large_image_selected(game_engine, "large_selected");
    paddle_size_select_.reset(new MultiSelect());
    paddle_size_select_->Add(small_image, small_image_selected, game_engine->position("small"));
    paddle_size_select_->Add(medium_image, medium_image_selected, game_engine->position("medium"));
    paddle_size_select_->Add(large_image, large_image_selected, game_engine->position("large"));
    if (LocalStore::HasEntryForKey(LS_PADDLE_SIZE)) {
      paddle_size_select_->set_selected_value(LocalStore::IntegerForKey(LS_PADDLE_SIZE));
    } else {
      paddle_size_select_->set_selected_value(psLarge);
    }
    AddEntity(paddle_size_select_);
    fade_in(paddle_size_select_.get());
  }

  sound_slider_.reset(new SoundSlider(game_engine,
                                      game_engine->position("sound_slider_main_menu")));
  AddEntity(sound_slider_);
}


// EngineView

void MainMenuView::ViewIsShown() {
  if (IS_FREE) {
    if (game_engine()->platform_type() == kPlatformTypePhone) {
      game_engine()->ad_engine()->SetAdAtPoint(kScreenPointZero);
    } else {
      game_engine()->ad_engine()->SetAdAtPoint(screen_point_make(45, 40));
    }
  }
}

void MainMenuView::Update() {
  EngineView::Update();
  if (state_ == kMainMenuStateAnimatingOut) {
    animating_out_ticks_left_--;
    if (animating_out_ticks_left_ <= 0) {
      game_engine()->RemoveView(this);
    }
  }
}


// ButtonDelegate

void MainMenuView::ButtonPressed(Button *button) {
  if (button == start_button_.get()) {
    PressedStart();
  } else if (button == story_button_.get()) {
    PressedStory();
  }
}


// private

void MainMenuView::AnimateOut() {
  state_ = kMainMenuStateAnimatingOut;

  title_->AnimateToZoom(0, kAnimationTypeLinear, 15);
  fade_out(main_menu_.get());
  fade_out(start_button_.get());
  fade_out(story_button_.get());
  fade_out(num_players_select_.get());
  fade_out(num_pucks_select_.get());
  fade_out(difficulty_select_.get());
  fade_out(paddle_size_select_.get());
  RemoveEntity(sound_slider_);

  animating_out_ticks_left_ = 15;
}

void MainMenuView::PressedStart() {
  if (game_engine()->platform_type() == kPlatformTypeTablet) {
    game_engine()->ad_engine()->RemoveAd();
  }

  bool is_iphone = false; // TODO UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone;

  LocalStore::SetInteger(num_players_select_->selected_value(), LS_NUM_PLAYERS);
  LocalStore::SetInteger(num_pucks_select_->selected_value(), LS_NUM_PUCKS);
  LocalStore::SetInteger(difficulty_select_->selected_value(), LS_DIFFICULTY);
  if (!is_iphone) {
    LocalStore::SetInteger(paddle_size_select_->selected_value(), LS_PADDLE_SIZE);
  }

  map<string, string> analytics_params;
  analytics_params["NumPlayers"] = to_string(num_players_select_->selected_value() + 1);
  analytics_params["NumPucks"] = to_string(num_pucks_select_->selected_value() + 1);
  analytics_params["Difficulty"] = to_string(difficulty_select_->selected_value());
  analytics_params["PaddleSize"] = to_string(paddle_size_select_->selected_value());
  game_engine()->analytics_engine()->LogEvent("START_GAME", analytics_params);

  PaddleSize paddle_size = PaddleSize(is_iphone ? psLarge : paddle_size_select_->selected_value());
  PlayView *play_view = new PlayView(game_engine(),
                                     num_players_select_->selected_value() + 1,
                                     num_pucks_select_->selected_value() + 1,
                                     ComputerAI(difficulty_select_->selected_value()),
                                     paddle_size);
  AnimateOut();
  game_engine()->PushView(sp<EngineView>(play_view));
}

void MainMenuView::PressedStory() {
  game_engine()->analytics_engine()->LogEvent("STORY_PRESSED");
  if (game_engine()->platform_type() == kPlatformTypeTablet) {
    game_engine()->ad_engine()->RemoveAd();
  }
  game_engine()->PushView(sp<EngineView>(new StoryView(game_engine())));
}
