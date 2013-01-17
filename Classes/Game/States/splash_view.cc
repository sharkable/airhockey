//
//  splash_view.cc
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-07.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "splash_view.h"

#import "resource_loader.h"
#import "main_menu_state.h"
#import "game_engine.h"
#import "const.h"
#include "soundengine/sound_player.h"

#import "puck.h"

SplashView::SplashView(GameEngine &gameEngine) : EngineView(gameEngine) {
  state_ = kSplashViewStateInitial;
}

void SplashView::Update() {
  EngineView::Update();
  switch (state_) {
    case kSplashViewStateInitial:
      SoundPlayer::instance()->initializeWithDelegate(this);
      state_ = kSplashViewStateLoadingSounds;
      break;
    case kSplashViewStateSoundsDidLoad:
      game_engine().SetRootView(sp<EngineView>(new MainMenuState(game_engine())));
      state_ = kSplashViewStateFinished;
      break;
    default:
      break;
  }
}


// SoundInitializationDelegate

void SplashView::SoundInitialized(SoundPlayer *sound_player) {
  state_ = kSplashViewStateSoundsDidLoad;
}