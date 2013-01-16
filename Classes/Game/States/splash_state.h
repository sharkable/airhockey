//
//  splash_state.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-07.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAME_STATES_SPLASHSTATE_H_
#define AIRHOCKEY_GAME_STATES_SPLASHSTATE_H_

#import "engine_state.h"
#import "simple_item.h"
#include "soundengine/sound_initialization_delegate.h"

class SplashState : public EngineState, SoundInitializationDelegate {
 public:
  SplashState(GameEngine &gameEngine);
  ~SplashState();

  // EngineState
  string Name() { return "SplashState"; }
  void Update();
  
  // SoundInitializationDelegate
  void SoundInitialized(SoundPlayer *sound_player);
  
 private:
  // UIActivityIndicatorView *spinner_;
  bool soundInitialized_;
};

#endif
