//
//  StoryState.m
//  AirHockeyFree
//
//  Created by Jonathan Sharkey on 10-05-17.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "StoryState.h"
#import "ResourceLoader.h"
#import "GameEngine.h"

StoryState::StoryState(GameEngine *gameEngine) : EngineState(gameEngine) {
  Texture2D storyButtonImage = ResourceLoader::instance()->getTextureWithName("story");
  storyButton_ = new Button(&storyButtonImage, &storyButtonImage,
                            CGPointMake(0, 0));
// TODO
//  storyButton_->setDelegate(self);
//  storyButton_->setSelector(@selector(pressedStory));
  addEntity(storyButton_);


  Texture2D aboutButtonImage = ResourceLoader::instance()->getTextureWithName("about");
  aboutButton_ = new Button(&aboutButtonImage, &aboutButtonImage, CGPointMake(0, 0));
// TODO
//  aboutButton_->setDelegate(self);
//  aboutButton_->setSelector(@selector(pressedAbout));;
}

StoryState::~StoryState() {
  delete storyButton_;
  delete aboutButton_;
}

void StoryState::pressedStory() {
  removeEntity(storyButton_);
  addEntity(aboutButton_);
}

void StoryState::pressedAbout() {
  [getGameEngine() popState];
}
