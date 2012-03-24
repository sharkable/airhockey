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

@implementation StoryState

- (id)initWithGameEngine:(GameEngine *)gameEngine {
  self = [super initWithGameEngine:gameEngine];
  
  if (self) {
    Texture2D *storyButtonImage = [[ResourceLoader instance] getTextureWithName:@"story"];
    storyButton_ = new Button(storyButtonImage, storyButtonImage,
                              CGPointMake(0, 0));
    storyButton_->setDelegate(self);
    storyButton_->setSelector(@selector(pressedStory));
    [self addEntity:storyButton_];


    Texture2D *aboutButtonImage = [[ResourceLoader instance] getTextureWithName:@"about"];
    aboutButton_ = new Button(aboutButtonImage, aboutButtonImage, CGPointMake(0, 0));
    aboutButton_->setDelegate(self);
    aboutButton_->setSelector(@selector(pressedAbout));;
  }
  
  return self;
}

- (void) dealloc {
  delete storyButton_;
  delete aboutButton_;
  
  [super dealloc];
}

- (void) pressedStory {
  [self removeEntity:storyButton_];
  [self addEntity:aboutButton_];
}

- (void) pressedAbout {
  [self.gameEngine popState];
}

@end
