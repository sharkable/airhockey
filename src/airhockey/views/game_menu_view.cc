//
//  game_menu_view.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2013-03-04.
//  Copyright 2013 Sharkable. All rights reserved.
//

#include "airhockey/views/game_menu_view.h"

#include "gameengine/entities/simple_item.h"
#include "gameengine/modules/ad_module.h"
#include "gameengine/coordinate_types.h"
#include "gameengine/sprite.h"

#include "airhockey/entities/sound_slider.h"

GameMenuView::GameMenuView(GameEngine *game_engine, GameMenuViewDelegate *delegate,
                           bool match_finished)
    : EngineView(game_engine),
      delegate_(delegate) {
  Init(match_finished);
}


#pragma mark - EngineView

bool GameMenuView::HandleBackButton() {
  ButtonPressed(menu_button_.get());
  return true;
}

void GameMenuView::HandlePauseButton() {
  if (rematch_button_.get()) {
    ButtonPressed(rematch_button_.get());
  } else if (continue_button_.get()) {
    ButtonPressed(continue_button_.get());
  }
}


#pragma mark - ButtonDelegate

void GameMenuView::ButtonPressed(Button *button) {
  if (game_engine()->platform_type() == kPlatformTypeTablet) {
    game_engine()->ad_module()->RemoveAd();
  }
  game_engine()->PopView();
  if (button == rematch_button_.get()) {
    delegate_->RematchPressed();
  } else if (button == menu_button_.get()) {
    delegate_->MenuPressed();
  } else if (button == continue_button_.get()) {
    delegate_->ContinuePressed();
  }
}


#pragma mark - private

void GameMenuView::Init(bool match_finished) {
  Sprite menu_background_sprite(game_engine(), "game_menu_bg");
  menu_background_.reset(new SimpleItem());
  menu_background_->add_sprite(menu_background_sprite);
  menu_background_->set_position(game_engine()->position("game_menu_bg"));
  AddEntity(menu_background_);

  GamePoint sound_slider_position = game_engine()->position("sound_slider_game_menu");
  sound_slider_.reset(new SoundSlider(game_engine(), sound_slider_position));
  AddEntity(sound_slider_);

  if (match_finished) {
    Sprite rematch_button_sprite(game_engine(), "rematch_button");
    Sprite rematch_button_pressed_sprite(game_engine(), "rematch_button_pressed");
    rematch_button_.reset(new Button(game_engine()));
    rematch_button_->set_normal_sprite(rematch_button_sprite);
    rematch_button_->set_pressed_sprite(rematch_button_pressed_sprite);
    rematch_button_->set_position(game_engine()->position("rematch_button"));
    rematch_button_->set_delegate(this);
    AddEntity(rematch_button_);
  } else {
    Sprite continue_button_sprite(game_engine(), "continue_button");
    Sprite continue_button_pressed_sprite(game_engine(), "continue_button_pressed");
    continue_button_.reset(new Button(game_engine()));
    continue_button_->set_normal_sprite(continue_button_sprite);
    continue_button_->set_pressed_sprite(continue_button_pressed_sprite);
    continue_button_->set_position(game_engine()->position("continue_button"));
    continue_button_->set_delegate(this);
    AddEntity(continue_button_);
  }

  Sprite menu_button_sprite(game_engine(), "menu_button");
  Sprite menu_button_pressed_sprite(game_engine(), "menu_button_pressed");
  menu_button_.reset(new Button(game_engine()));
  menu_button_->set_normal_sprite(menu_button_sprite);
  menu_button_->set_pressed_sprite(menu_button_pressed_sprite);
  menu_button_->set_position(game_engine()->position("menu_button"));
  menu_button_->set_delegate(this);
  AddEntity(menu_button_);
}
