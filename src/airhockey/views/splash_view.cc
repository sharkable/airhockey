//
//  splash_view.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2010-05-07.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "airhockey/views/splash_view.h"

#include "gameengine/engine_view.h"
#include "gameengine/game_engine.h"
#include "soundengine/sound_player.h"

#include "airhockey/views/main_menu_view.h"

SplashView::SplashView(sp<GameEngine> game_engine) : EngineView(game_engine) {
  state_ = kSplashViewStateInitial;
}

void SplashView::Update() {
  EngineView::Update();
  switch (state_) {
    case kSplashViewStateInitial:
      SoundPlayer::instance()->initializeWithDelegate(this);
      SoundPlayer::instance()->setSoundEffectsOn(true);
      state_ = kSplashViewStateLoadingSounds;
      break;
    case kSplashViewStateSoundsDidLoad:
      game_engine()->PopView();
      game_engine()->PushView(sp<EngineView>(new MainMenuView(game_engine())));
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
