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

GameMenuView::GameMenuView(GameEngine &game_engine, GameMenuViewDelegate *delegate,
                           bool match_finished)
    : EngineView(game_engine),
      delegate_(delegate),
      sound_slider_(NULL),
      menu_background_(NULL),
      rematch_button_(NULL),
      menu_button_(NULL),
      continue_button_(NULL) {
  Init(match_finished);
}

GameMenuView::~GameMenuView() {
  delete sound_slider_;
  delete menu_background_;
  if (rematch_button_) {
    delete rematch_button_;
  }
  if (menu_button_) {
    delete menu_button_;
  }
  if (continue_button_) {
    delete continue_button_;
  }
}


#pragma mark - EngineView

bool GameMenuView::HandleBackButton() {
  ButtonUp(menu_button_);
  return true;
}

void GameMenuView::HandlePauseButton() {
  if (rematch_button_) {
    ButtonUp(rematch_button_);
  } else if (continue_button_) {
    ButtonUp(continue_button_);
  }
}


#pragma mark - ButtonDelegate

void GameMenuView::ButtonUp(Button *button) {
  game_engine().PopView();
  if (button == rematch_button_) {
    delegate_->RematchPressed();
  } else if (button == menu_button_) {
    delegate_->MenuPressed();
  } else if (button == continue_button_) {
    delegate_->ContinuePressed();
  }
}


#pragma mark - private

void GameMenuView::Init(bool match_finished) {
  Sprite menu_background_sprite(game_engine(), "game_menu_bg");
  menu_background_ = new SimpleItem();
  menu_background_->add_sprite(menu_background_sprite);
  menu_background_->set_position(game_engine().position("game_menu_bg"));
  AddEntity(menu_background_, false);

  GamePoint sound_slider_position = game_engine().position("sound_slider_game_menu");
  sound_slider_ = new SoundSlider(game_engine(), sound_slider_position);
  AddEntity(sound_slider_, false);

  if (match_finished) {
    Sprite rematch_button_sprite(game_engine(), "rematch_button");
    Sprite rematch_button_pressed_sprite(game_engine(), "rematch_button_pressed");
    rematch_button_ = new Button(game_engine());
    rematch_button_->set_normal_sprite(rematch_button_sprite);
    rematch_button_->set_pressed_sprite(rematch_button_pressed_sprite);
    rematch_button_->set_position(game_engine().position("rematch_button"));
    rematch_button_->set_delegate(this);
    AddEntity(rematch_button_, false);
  } else {
    Sprite continue_button_sprite(game_engine(), "continue_button");
    Sprite continue_button_pressed_sprite(game_engine(), "continue_button_pressed");
    continue_button_ = new Button(game_engine());
    continue_button_->set_normal_sprite(continue_button_sprite);
    continue_button_->set_pressed_sprite(continue_button_pressed_sprite);
    continue_button_->set_position(game_engine().position("continue_button"));
    continue_button_->set_delegate(this);
    AddEntity(continue_button_, false);
  }

  Sprite menu_button_sprite(game_engine(), "menu_button");
  Sprite menu_button_pressed_sprite(game_engine(), "menu_button_pressed");
  menu_button_ = new Button(game_engine());
  menu_button_->set_normal_sprite(menu_button_sprite);
  menu_button_->set_pressed_sprite(menu_button_pressed_sprite);
  menu_button_->set_position(game_engine().position("menu_button"));
  menu_button_->set_delegate(this);
  AddEntity(menu_button_, false);
}
