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

#import "puck.h"

SplashState::SplashState(GameEngine &gameEngine) : EngineState(gameEngine) {
  state_ = kSplashStateInitial;
}

void SplashState::Update() {
  EngineState::Update();
  switch (state_) {
    case kSplashStateInitial:
      SoundPlayer::instance()->initializeWithDelegate(this);
      state_ = kSplashStateLoadingSounds;
      break;
    case kSplashStateSoundsDidLoad:
      game_engine().ReplaceTopState(sp<EngineState>(new MainMenuState(game_engine())));
      state_ = kSplashStateFinished;
      break;
    default:
      break;
  }
}


// SoundInitializationDelegate

void SplashState::SoundInitialized(SoundPlayer *sound_player) {
  state_ = kSplashStateSoundsDidLoad;
}
