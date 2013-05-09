//
//  settings_view.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2013-05-08.
//  Copyright 2013 Sharkable. All rights reserved.
//

#include "airhockey/views/settings_view.h"

#include "gameengine/entities/multi_select.h"
#include "gameengine/entities/simple_item.h"
#include "gameengine/modules/local_store.h"
#include "gameengine/game_engine.h"

#include "airhockey/entities/paddle.h"

using std::string;

static const int kMaxNumPucks = 7;

// Local Store keys
const string kLocalStoreDifficulty = "ls_difficulty";
const string kLocalStoreNumPucks = "ls_num_pucks";
const string kLocalStorePaddleSize = "ls_paddle_size";

SettingsView::SettingsView(sp<GameEngine> game_engine) : EngineView(game_engine) {
  Sprite background_image(game_engine, "settings_bg");
  background_.reset(new SimpleItem(background_image, game_engine->position("settings_bg")));
  AddEntity(background_);

  num_pucks_select_.reset(new MultiSelect());
  for (int i = 1; i <= kMaxNumPucks; i++) {
    char pucks_str[15];
    sprintf(pucks_str, "%d", i);
    char pucks_selected_str[15];
    sprintf(pucks_selected_str, "%d_selected", i);

    Sprite num_pucks_image(game_engine, pucks_str);
    Sprite num_pucks_selected_image(game_engine, pucks_selected_str);
    GamePoint num_pucks_position = game_engine->position(pucks_str);
    num_pucks_select_->Add(num_pucks_image, num_pucks_selected_image, num_pucks_position);
  }
  num_pucks_select_->set_selected_value(LocalStore::IntegerForKey(kLocalStoreNumPucks));
  AddEntity(num_pucks_select_);

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
  if (LocalStore::HasEntryForKey(kLocalStoreDifficulty)) {
    difficulty_select_->set_selected_value(LocalStore::IntegerForKey(kLocalStoreDifficulty));
  } else {
    difficulty_select_->set_selected_value(kComputerAIGood);
  }
  AddEntity(difficulty_select_);

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
  if (LocalStore::HasEntryForKey(kLocalStorePaddleSize)) {
    paddle_size_select_->set_selected_value(LocalStore::IntegerForKey(kLocalStorePaddleSize));
  } else {
    paddle_size_select_->set_selected_value(kPaddleSizeLarge);
  }
  AddEntity(paddle_size_select_);

  Sprite ok_button_image(game_engine, "ok_button");
  Sprite ok_button_pressed_image(game_engine, "ok_button_pressed");
  ok_button_.reset(new Button());
  ok_button_->set_normal_sprite(ok_button_image);
  ok_button_->set_pressed_sprite(ok_button_pressed_image);
  ok_button_->set_position(game_engine->position("ok_button"));
  ok_button_->set_delegate(this);
  AddEntity(ok_button_);
}


// ButtonDelegate

void SettingsView::ButtonPressed(Button *button) {
  LocalStore::SetInteger(num_pucks_select_->selected_value(), kLocalStoreNumPucks);
  LocalStore::SetInteger(difficulty_select_->selected_value(), kLocalStoreDifficulty);
  LocalStore::SetInteger(paddle_size_select_->selected_value(), kLocalStorePaddleSize);
  game_engine()->PopView();
}
