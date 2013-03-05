//
//  game_menu_view.cc
//  AirHockey
//
//  Created by Jon Sharkey on 3/4/13.
//  Copyright 2014 Sharkable. All rights reserved.
//

#include "airhockey/views/game_menu_view.h"

#include "gameengine/entities/simple_item.h"
#include "gameengine/coordinate_types.h"

#include "airhockey/entities/sound_slider.h"
#include "airhockey/const.h"

GameMenuView::GameMenuView(sp<GameEngine> game_engine, GameMenuViewDelegate *delegate,
                           bool match_finished)
    : EngineView(game_engine),
      delegate_(delegate) {
  Init(match_finished);
}


// ButtonDelegate

void GameMenuView::ButtonPressed(Button *button) {
  game_engine()->PopView();
  if (button == rematch_button_.get()) {
    delegate_->RematchPressed();
  } else if (button == menu_button_.get()) {
    delegate_->MenuPressed();
  } else if (button == continue_button_.get()) {
    delegate_->ContinuePressed();
  }
}


// private

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
    rematch_button_.reset(new Button());
    rematch_button_->set_normal_sprite(rematch_button_sprite);
    rematch_button_->set_pressed_sprite(rematch_button_pressed_sprite);
    rematch_button_->set_position(game_engine()->position("rematch_button"));
    rematch_button_->set_delegate(this);
    AddEntity(rematch_button_);
  } else {
    Sprite continue_button_sprite(game_engine(), "continue_button");
    Sprite continue_button_pressed_sprite(game_engine(), "continue_button_pressed");
    continue_button_.reset(new Button());
    continue_button_->set_normal_sprite(continue_button_sprite);
    continue_button_->set_pressed_sprite(continue_button_pressed_sprite);
    continue_button_->set_position(game_engine()->position("continue_button"));
    continue_button_->set_delegate(this);
    AddEntity(continue_button_);
  }

  Sprite menu_button_sprite(game_engine(), "menu_button");
  Sprite menu_button_pressed_sprite(game_engine(), "menu_button_pressed");
  menu_button_.reset(new Button());
  menu_button_->set_normal_sprite(menu_button_sprite);
  menu_button_->set_pressed_sprite(menu_button_pressed_sprite);
  menu_button_->set_position(game_engine()->position("menu_button"));
  menu_button_->set_delegate(this);
  AddEntity(menu_button_);
}
