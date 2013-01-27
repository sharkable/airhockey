//
//  rink.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-13.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAME_ENTITIES_RINK_H_
#define AIRHOCKEY_GAME_ENTITIES_RINK_H_

#include "gameengine/view_entity.h"
#include "opengl/Texture2D.h"

class RoundThing;

class Rink : public ViewEntity {
 public:
  Rink();
  ~Rink();
  
  void BounceOff(RoundThing *thing);
  void MoveInFromEdge(RoundThing *thing);

  // ViewEntity
  void Update();
  void Render();

 private:
  Texture2D texture_;
  ScreenPoint renderPoint_;
};

#endif
