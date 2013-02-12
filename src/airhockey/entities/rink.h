//
//  rink.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-13.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAME_ENTITIES_RINK_H_
#define AIRHOCKEY_GAME_ENTITIES_RINK_H_

#include "gameengine/sprite.h"
#include "gameengine/view_entity.h"
#include "opengl/Texture2D.h"

class GameEngine;
class RoundThing;

class Rink : public ViewEntity {
 public:
  Rink(sp<GameEngine> game_engine);
  ~Rink();
  
  void BounceOff(RoundThing *thing);
  void MoveInFromEdge(RoundThing *thing);

  // ViewEntity
  void Update();
  void Render();

 private:
  Sprite sprite_;
  GamePoint render_point_;
};

#endif