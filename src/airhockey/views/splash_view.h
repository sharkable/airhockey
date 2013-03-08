//
//  splash_view.h
//  AirHockey
//
//  Created by Jon Sharkey on 2010-05-07.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_VIEWS_SPLASHVIEW_H_
#define AIRHOCKEY_VIEWS_SPLASHVIEW_H_

#include "gameengine/engine_view.h"
#include "soundengine/sound_initialization_delegate.h"

class GameEngine;
class SoundPlayer;

typedef enum {
  kSplashViewStateInitial = 0,
  kSplashViewStateLoadingSounds,
  kSplashViewStateSoundsDidLoad,
  kSplashViewStateFinished
} SplashViewState;

class SplashView : public EngineView, SoundInitializationDelegate {
 public:
  SplashView(sp<GameEngine> game_engine);

  // EngineView
  void Update();

  // SoundInitializationDelegate
  void SoundInitialized(SoundPlayer *sound_player);

 private:
  SplashViewState state_;
};

#endif
