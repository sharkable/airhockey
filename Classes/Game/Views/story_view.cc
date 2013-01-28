//
//  story_view.cc
//  AirHockeyFree
//
//  Created by Jonathan Sharkey on 10-05-17.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "game/views/story_view.h"

#include "gameengine/game_engine.h"

StoryView::StoryView(sp<GameEngine> game_engine) : EngineView(game_engine) {
  Sprite storyButtonImage = Sprite(game_engine, "story");
  storyButton_.reset(new Button());
  storyButton_->set_normal_sprite(storyButtonImage);
  storyButton_->set_pressed_sprite(storyButtonImage);
  storyButton_->set_position(game_point_make(0, 0));
  storyButton_->set_delegate(this);
  AddEntity(storyButton_);

  Sprite aboutButtonImage = Sprite(game_engine, "about");
  aboutButton_.reset(new Button());
  aboutButton_->set_normal_sprite(aboutButtonImage);
  aboutButton_->set_pressed_sprite(aboutButtonImage);
  aboutButton_->set_position(game_point_make(0, 0));
  aboutButton_->set_delegate(this);
}


// ButtonDelegate

void StoryView::ButtonPressed(Button *button) {
  if (button == storyButton_.get()) {
    RemoveEntity(storyButton_);
    AddEntity(aboutButton_);    
  } else if (button == aboutButton_.get()) {
    game_engine()->PopView();
  }
}
