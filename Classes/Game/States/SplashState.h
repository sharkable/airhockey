//
//  SplashState.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-07.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "engine_state.h"
#import "simple_item.h"

class SplashState : public EngineState {
public:
  SplashState(GameEngine &gameEngine);
  ~SplashState();
  void soundInitialized();
  void Update();
 private:
  // UIActivityIndicatorView *spinner_;
};
