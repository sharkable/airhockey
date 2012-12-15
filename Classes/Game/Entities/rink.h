//
//  rink.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-13.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAME_ENTITIES_RINK_H_
#define AIRHOCKEY_GAME_ENTITIES_RINK_H_

#include "gameengine/state_entity.h"
#include "opengl/Texture2D.h"

class RoundThing;

class Rink : public StateEntity {
 public:
  Rink();
  ~Rink();
  
  void BounceOff(RoundThing *thing);
  void MoveInFromEdge(RoundThing *thing);

  // StateEntity
  void Update();
  void Render();
  string Name() { return "Rink"; }

 private:
  Texture2D texture_;
  SGPoint renderPoint_;
};

#endif
