//
//  story_view.cc
//  AirHockeyFree
//
//  Created by Jonathan Sharkey on 10-05-17.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "airhockey/views/story_view.h"

#include "gameengine/game_engine.h"

StoryView::StoryView(sp<GameEngine> game_engine) : EngineView(game_engine) {
  // TODO We're loading this twice so the counter for this texture goes up. Rethink texture
  // management.
  Sprite story_button_image = Sprite(game_engine, "story");
  Sprite story_button_image_2 = Sprite(game_engine, "story");
  GameSize screen_size = game_engine->game_size();
  GamePoint button_position =
      game_point_make((screen_size.width - story_button_image.content_size().width) / 2,
                      (screen_size.height - story_button_image.content_size().height) / 2);
  double zoom = screen_size.width / story_button_image.content_size().width;
  story_button_.reset(new Button());
  story_button_->set_normal_sprite(story_button_image);
  story_button_->set_pressed_sprite(story_button_image_2);
  story_button_->set_zoom(zoom);
  story_button_->set_position(button_position);
  story_button_->set_delegate(this);
  AddEntity(story_button_);

  Sprite about_button_image = Sprite(game_engine, "about");
  Sprite about_button_image_2 = Sprite(game_engine, "about");
  about_button_.reset(new Button());
  about_button_->set_normal_sprite(about_button_image);
  about_button_->set_pressed_sprite(about_button_image_2);
  about_button_->set_zoom(zoom);
  about_button_->set_position(button_position);
  about_button_->set_delegate(this);
}


// ButtonDelegate

void StoryView::ButtonPressed(Button *button) {
  if (button == story_button_.get()) {
    RemoveEntity(story_button_);
    AddEntity(about_button_);    
  } else if (button == about_button_.get()) {
    game_engine()->PopView();
  }
}
