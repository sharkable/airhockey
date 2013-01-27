//
//  story_view.cc
//  AirHockeyFree
//
//  Created by Jonathan Sharkey on 10-05-17.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "game/views/story_view.h"

#include "gameengine/game_engine.h"
#include "gameengine/resource_loader.h"

StoryView::StoryView(GameEngine &gameEngine) : EngineView(gameEngine) {
  Texture2D storyButtonImage = ResourceLoader::Instance().TextureWithName("story");
  storyButton_.reset(new Button());
  storyButton_->set_normal_texture(storyButtonImage);
  storyButton_->set_pressed_texture(storyButtonImage);
  storyButton_->set_position(ScreenPointMake(0, 0));
  storyButton_->set_delegate(this);
  AddEntity(storyButton_);

  Texture2D aboutButtonImage = ResourceLoader::Instance().TextureWithName("about");
  aboutButton_.reset(new Button());
  aboutButton_->set_normal_texture(aboutButtonImage);
  aboutButton_->set_pressed_texture(aboutButtonImage);
  aboutButton_->set_position(ScreenPointMake(0, 0));
  aboutButton_->set_delegate(this);
}


// ButtonDelegate

void StoryView::ButtonPressed(Button *button) {
  if (button == storyButton_.get()) {
    RemoveEntity(storyButton_);
    AddEntity(aboutButton_);    
  } else if (button == aboutButton_.get()) {
    game_engine().PopView();
  }
}
