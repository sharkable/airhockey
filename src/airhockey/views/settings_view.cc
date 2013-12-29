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
#include "gameengine/modules/persistence_module.h"
#include "gameengine/game_engine.h"

#include "airhockey/entities/paddle.h"

using std::string;

static const int kMaxNumPucks = 7;
static const int kAnimateTicks = 30;

// Local Store keys
const string kLocalStoreDifficulty = "ls_difficulty";
const string kLocalStoreNumPucks = "ls_num_pucks";
const string kLocalStorePaddleSize = "ls_paddle_size";

SettingsView::SettingsView(GameEngine &game_engine) : game_engine_(game_engine) {
  sp<PersistenceModule> persistence = game_engine.persistence_module();

  double width = game_engine.screen_size_to_game_size(game_engine.screen_size()).width;
  ending_position_ = GamePoint(-width, 0);

  x_position_animation_.Reset(width, 0, kAnimateTicks, kAnimationTypeCubicEaseOut);

  Sprite background_image(game_engine, "settings_bg");
  background_ = new SimpleItem(background_image, game_engine.position("settings_bg"));

  num_pucks_select_ = new MultiSelect(game_engine);
  for (int i = 1; i <= kMaxNumPucks; i++) {
    char pucks_str[15];
    sprintf(pucks_str, "%d", i);
    char pucks_selected_str[15];
    sprintf(pucks_selected_str, "%d_selected", i);

    Sprite num_pucks_image(game_engine, pucks_str);
    Sprite num_pucks_selected_image(game_engine, pucks_selected_str);
    GamePoint num_pucks_position = game_engine.position(pucks_str);
    num_pucks_select_->Add(num_pucks_image, num_pucks_selected_image, num_pucks_position);
  }
  num_pucks_select_->set_selected_value(persistence->IntegerForKey(kLocalStoreNumPucks));

  Sprite bad_image(game_engine, "bad");
  Sprite bad_image_selected(game_engine, "bad_selected");
  Sprite good_image(game_engine, "good");
  Sprite good_image_selected(game_engine, "good_selected");
  Sprite excellent_image(game_engine, "excellent");
  Sprite excellent_image_selected(game_engine, "excellent_selected");
  Sprite amazing_image(game_engine, "amazing");
  Sprite amazing_image_selected(game_engine, "amazing_selected");
  difficulty_select_ = new MultiSelect(game_engine);
  difficulty_select_->Add(bad_image, bad_image_selected, game_engine.position("bad"));
  difficulty_select_->Add(good_image, good_image_selected, game_engine.position("good"));
  difficulty_select_->Add(excellent_image, excellent_image_selected,
                          game_engine.position("excellent"));
  difficulty_select_->Add(amazing_image, amazing_image_selected, game_engine.position("amazing"));
  difficulty_select_->set_selected_value(persistence->IntegerForKey(kLocalStoreDifficulty));

  Sprite small_image(game_engine, "small");
  Sprite small_image_selected(game_engine, "small_selected");
  Sprite medium_image(game_engine, "medium");
  Sprite medium_image_selected(game_engine, "medium_selected");
  Sprite large_image(game_engine, "large");
  Sprite large_image_selected(game_engine, "large_selected");
  paddle_size_select_ = new MultiSelect(game_engine);
  paddle_size_select_->Add(small_image, small_image_selected, game_engine.position("small"));
  paddle_size_select_->Add(medium_image, medium_image_selected, game_engine.position("medium"));
  paddle_size_select_->Add(large_image, large_image_selected, game_engine.position("large"));
  paddle_size_select_->set_selected_value(persistence->IntegerForKey(kLocalStorePaddleSize));

  Sprite ok_button_image(game_engine, "ok_button");
  Sprite ok_button_pressed_image(game_engine, "ok_button_pressed");
  ok_button_ = new Button(game_engine);
  ok_button_->set_normal_sprite(ok_button_image);
  ok_button_->set_pressed_sprite(ok_button_pressed_image);
  ok_button_->set_position(game_engine.position("ok_button"));
  ok_button_->set_delegate(this);
}


// TOOD
//bool SettingsView::HandleBackButton() {
//  ButtonUp(NULL);
//  return true;
//}

#pragma mark - Simulator

void SettingsView::SimulateStep() {
  if (x_position_animation_.IsActive()) {
    x_position_ = x_position_animation_.Update();
  }
}


#pragma mark - Renderer

void SettingsView::Render(CoordinateSystem const &coordinate_system) {
  GamePoint offset(x_position_, 0);
  CoordinateSystem local_system = coordinate_system.Subsystem(0.f, offset);
  background_->Render(local_system);
  num_pucks_select_->Render(offset);
  difficulty_select_->Render(offset);
  paddle_size_select_->Render(offset);
  ok_button_->Render(offset, 0.f);
}


#pragma mark - InputHandler

bool SettingsView::HandleEvent(InputEvent const &event) {
  // TODO NOW support event handling in MultiSelect.
  ok_button_->HandleEvent(event);
  return x_position_ >= 0;
}


#pragma mark - Animatable

void SettingsView::AnimationFinished(Animatable *animatable) {
  // TODO NOW ADD A DELEGATE CALL
//  if (entities_->position().x < 0) {
//    game_engine().RemoveView(this);
//  }
}


#pragma mark - ButtonDelegate

void SettingsView::ButtonUp(Button *button) {
  sp<PersistenceModule> persistence = game_engine_.persistence_module();
  persistence->SetInteger(num_pucks_select_->selected_value(), kLocalStoreNumPucks);
  persistence->SetInteger(difficulty_select_->selected_value(), kLocalStoreDifficulty);
  persistence->SetInteger(paddle_size_select_->selected_value(), kLocalStorePaddleSize);
  x_position_animation_.Reset(x_position_, ending_position_.x, kAnimateTicks,
                              kAnimationTypeCubicEaseIn);
}
