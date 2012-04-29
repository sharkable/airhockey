//
//  SplashState.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-07.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAME_STATES_SPLASHSTATE_H_
#define AIRHOCKEY_GAME_STATES_SPLASHSTATE_H_

#import "engine_state.h"
#import "simple_item.h"

class SplashState : public EngineState {
 public:
  SplashState(GameEngine &gameEngine);
  ~SplashState();

  // EngineState
  void soundInitialized();
  void Update();

 private:
  // UIActivityIndicatorView *spinner_;
};

#endif
