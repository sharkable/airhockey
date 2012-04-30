//
//  story_state.cc
//  AirHockeyFree
//
//  Created by Jonathan Sharkey on 10-05-17.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "game/states/story_state.h"

#include "gameengine/game_engine.h"
#include "gameengine/resource_loader.h"

StoryState::StoryState(GameEngine &gameEngine) : EngineState(gameEngine) {
  Texture2D storyButtonImage = ResourceLoader::Instance().TextureWithName("story");
  storyButton_.set_normal_texture(storyButtonImage);
  storyButton_.set_pressed_texture(storyButtonImage);
  storyButton_.set_position(SGPointMake(0, 0));
  storyButton_.set_delegate(this);
  AddEntity(storyButton_);

  Texture2D aboutButtonImage = ResourceLoader::Instance().TextureWithName("about");
  aboutButton_.set_normal_texture(aboutButtonImage);
  aboutButton_.set_pressed_texture(aboutButtonImage);
  aboutButton_.set_position(SGPointMake(0, 0));
  aboutButton_.set_delegate(this);
}


// ButtonDelegate

void StoryState::ButtonPressed(Button *button) {
  if (button == &storyButton_) {
    RemoveEntity(storyButton_);
    AddEntity(aboutButton_);    
  } else if (button == &aboutButton_) {
    game_engine().PopState();    
  }
}
