//
//  story_state.cc
//  AirHockeyFree
//
//  Created by Jonathan Sharkey on 10-05-17.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "story_state.h"
#import "resource_loader.h"
#import "game_engine.h"

StoryState::StoryState(GameEngine &gameEngine) : EngineState(gameEngine) {
  Texture2D storyButtonImage = ResourceLoader::Instance().TextureWithName("story");
  storyButton_ = new Button(storyButtonImage, storyButtonImage,
                            SGPointMake(0, 0));
// TODO
//  storyButton_->set_delegate(self);
//  storyButton_->setSelector(@selector(pressedStory));
  AddEntity(*storyButton_);


  Texture2D aboutButtonImage = ResourceLoader::Instance().TextureWithName("about");
  aboutButton_ = new Button(aboutButtonImage, aboutButtonImage, SGPointMake(0, 0));
// TODO
//  aboutButton_->set_delegate(self);
//  aboutButton_->setSelector(@selector(pressedAbout));;
}

StoryState::~StoryState() {
  delete storyButton_;
  delete aboutButton_;
}

void StoryState::pressedStory() {
  RemoveEntity(*storyButton_);
  AddEntity(*aboutButton_);
}

void StoryState::pressedAbout() {
  game_engine().PopState();
}
