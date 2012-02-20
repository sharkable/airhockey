//
//  StoryState.m
//  AirHockeyFree
//
//  Created by Jonathan Sharkey on 10-05-17.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "StoryState.h"
#import "ResourceLoader.h"
#import "GameEngine.h"

@implementation StoryState

- (id) init {
	[super init];
	
	Texture2D* storyButtonImage = [[ResourceLoader instance] getTextureWithName:@"story"];
	_storyButton = [[Button alloc] initWithNormalTexture:storyButtonImage
										  pressedTexture:storyButtonImage
												position:CGPointMake(0, 0)];
	_storyButton.delegate = self;
	_storyButton.selector = @selector(pressedStory);
	[self addEntity:_storyButton];


	Texture2D* aboutButtonImage = [[ResourceLoader instance] getTextureWithName:@"about"];
	_aboutButton = [[Button alloc] initWithNormalTexture:aboutButtonImage
										  pressedTexture:aboutButtonImage
												position:CGPointMake(0, 0)];	
	_aboutButton.delegate = self;
	_aboutButton.selector = @selector(pressedAbout);
	
	return self;
}

- (void) dealloc {
	[_storyButton release];
	[_aboutButton release];
	
	[super dealloc];
}

- (void) pressedStory {
	[self removeEntity:_storyButton];
	[self addEntity:_aboutButton];
}

- (void) pressedAbout {
	[[GameEngine instance] popState];
}

@end
