//
//  splash_state.cc
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-07.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "splash_state.h"
#import "resource_loader.h"
#import "main_menu_state.h"
#import "game_engine.h"
#import "const.h"

SplashState::SplashState(GameEngine &gameEngine) : EngineState(gameEngine) {
//  spinner_ = [[UIActivityIndicatorView alloc]
//                initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
//  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
//    spinner_.center = CGPointMake(320 / 2, 480 / 2);
//  } else {
//    spinner_.center = CGPointMake(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
//  }
//  getGameEngine()->addUIView(spinner_);
//  [spinner_ startAnimating];
//  soundInitialized();  // TODO temporary until we actually can load sound.
}

SplashState::~SplashState() {
//  [spinner_ removeFromSuperview];
//  [spinner_ release];
}

void SplashState::soundInitialized() {
  game_engine().ReplaceTopState(new MainMenuState(game_engine()));
}


void SplashState::Update() {
  static int c = 0;
  EngineState::Update();
  if (++c == 2) {
    soundInitialized();
  }
}
