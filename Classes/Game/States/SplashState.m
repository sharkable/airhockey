//
//  SplashState.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-07.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "SplashState.h"
#import "ResourceLoader.h"
#import "MainMenuState.h"
#import "GameEngine.h"
#import "EAGLView.h"
#import "const.h"

@implementation SplashState

- (id) init {
	[super init];
	
	_spinner = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
		_spinner.center = CGPointMake(320/2, 480/2);		
	} else {
		_spinner.center = CGPointMake(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	}
	[EAGLView addUIView:_spinner];
	[_spinner startAnimating];
	
	return self;
}

- (void) dealloc {
	[EAGLView removeUIView:_spinner];
	[_spinner release];
	
	[super dealloc];
}

- (void) soundInitialized {
	[[GameEngine instance] replaceTopState:[[[MainMenuState alloc] init] autorelease]];
}

@end
