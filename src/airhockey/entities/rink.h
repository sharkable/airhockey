//
//  rink.h
//  AirHockey
//
//  Created by Jon Sharkey on 2010-04-13.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_ENTITIES_RINK_H_
#define AIRHOCKEY_ENTITIES_RINK_H_

#include "gameengine/view_entity.h"

class RoundThing;

class Rink : public ViewEntity {
 public:
  void BounceOff(RoundThing *thing);
  void MoveInFromEdge(RoundThing *thing);

  // ViewEntity
  void Update();
  void Render();
};

#endif
