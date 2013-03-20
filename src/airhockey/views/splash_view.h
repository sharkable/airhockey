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

class GameEngine;
class SoundPlayer;

class SplashView : public EngineView {
 public:
  SplashView(sp<GameEngine> game_engine);

  // EngineView
  void Update();
};

#endif
