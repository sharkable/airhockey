//
//  SplashState.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-07.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "EngineState.h"
#import "SimpleItem.h"

class SplashState : public EngineState {
public:
  SplashState(GameEngine *gameEngine);
  ~SplashState();
  void soundInitialized();
  void update();
 private:
  // UIActivityIndicatorView *spinner_;
};
