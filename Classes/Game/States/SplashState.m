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

- (id)init {
  [super init];
  
  if (self) {
      spinner_ = [[UIActivityIndicatorView alloc]
                  initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
      spinner_.center = CGPointMake(320 / 2, 480 / 2);
    } else {
      spinner_.center = CGPointMake(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    }
    [EAGLView addUIView:spinner_];
    [spinner_ startAnimating];
  }
  
  return self;
}

- (void)dealloc {
  [EAGLView removeUIView:spinner_];
  [spinner_ release];
  
  [super dealloc];
}

- (void)soundInitialized {
  [self.gameEngine replaceTopState:[[[MainMenuState alloc] init] autorelease]];
}

@end
