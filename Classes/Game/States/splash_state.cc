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
#include "soundengine/sound_player.h"

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
  soundInitialized_ = false;
}

SplashState::~SplashState() {
//  [spinner_ removeFromSuperview];
//  [spinner_ release];
}

void SplashState::Update() {
  static int c = 0;
  if (c++ == 3) {
    SoundPlayer::instance()->initializeWithDelegate(this);
  }
  if (soundInitialized_) {
    game_engine().ReplaceTopState(sp<EngineState>(new MainMenuState(game_engine())));
  }
}


// SoundInitializationDelegate

void SplashState::SoundInitialized(SoundPlayer *sound_player) {
  soundInitialized_ = true;
}
