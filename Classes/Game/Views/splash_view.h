//
//  splash_view.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-07.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAME_VIEWS_SPLASHVIEW_H_
#define AIRHOCKEY_GAME_VIEWS_SPLASHVIEW_H_

#import "engine_view.h"
#import "simple_item.h"
#include "soundengine/sound_initialization_delegate.h"

typedef enum {
  kSplashViewStateInitial = 0,
  kSplashViewStateLoadingSounds,
  kSplashViewStateSoundsDidLoad,
  kSplashViewStateFinished
} SplashViewState;

class SplashView : public EngineView, SoundInitializationDelegate {
 public:
  SplashView(GameEngine &gameEngine);

  // EngineView
  void Update();
  
  // SoundInitializationDelegate
  void SoundInitialized(SoundPlayer *sound_player);
  
 private:
  SplashViewState state_;
};

#endif
