//
//  rink_view.h
//  AirHockey
//
//  Created by Jon Sharkey on 2013-03-01.
//  Copyright 2013 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_VIEWS_RINKVIEW_H_
#define AIRHOCKEY_VIEWS_RINKVIEW_H_

#include "gameengine/coordinate_types.h"
#include "gameengine/engine_view.h"
#include "gameengine/game_engine.h"
#include "gameengine/platform.h"

class GameEngine;

class RinkView : public EngineView {
 public:
  RinkView(GameEngine *game_engine);

  static GameSize RinkSizeForTextureGroup(Platform::TextureGroup texture_group);
};

#endif
