//
//  rink_overlay.h
//  AirHockey
//
//  Created by Jon Sharkey on 2013-05-07.
//  Copyright 2013 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_ENTITIES_RINKOVERLAY_
#define AIRHOCKEY_ENTITIES_RINKOVERLAY_

#include "gameengine/entities/composite_entity.h"
#include "gameengine/view_entity.h"

class GameEngine;

class RinkOverlay : public ViewEntity {
 public:
  RinkOverlay(GameEngine *game_engine);

  // ViewEntity
  void Update();
  void Render(GamePoint offset);

 private:
  CompositeEntity rink_pieces_;
};

#endif
