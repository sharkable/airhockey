//
//  rink_view.h
//  AirHockey
//
//  Created by Jon Sharkey on 3/1/13.
//  Copyright 2013 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_VIEWS_RINKVIEW_H_
#define AIRHOCKEY_VIEWS_RINKVIEW_H_

#include "gameengine/engine_view.h"

class RinkView : public EngineView {
 public:
  RinkView(sp<GameEngine> game_engine);
};

#endif