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
    storyButton_ = [[Button alloc] initWithNormalTexture:storyButtonImage
                                          pressedTexture:storyButtonImage
                                                position:CGPointMake(0, 0)];
    storyButton_.delegate = self;
    storyButton_.selector = @selector(pressedStory);
    [self addEntity:storyButton_];


    Texture2D *aboutButtonImage = [[ResourceLoader instance] getTextureWithName:@"about"];
    aboutButton_ = [[Button alloc] initWithNormalTexture:aboutButtonImage
                                          pressedTexture:aboutButtonImage
                                                position:CGPointMake(0, 0)];  
    aboutButton_.delegate = self;
    aboutButton_.selector = @selector(pressedAbout);
  }
  
  return self;
}

- (void) dealloc {
  [storyButton_ release];
  [aboutButton_ release];
  
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
