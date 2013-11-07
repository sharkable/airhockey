//
//  post.h
//  AirHockey
//
//  Created by Jon Sharkey on 2010-04-25.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_ENTITIES_POST_H_
#define AIRHOCKEY_ENTITIES_POST_H_

#include "airhockey/entities/round_thing.h"

class GameEngine;
class ViewEntity;

class Post : public RoundThing {
 public:
  Post(GameEngine &game_engine, double x, double y);

  // RoundThing
  void DidBounceOff(ViewEntity *other, double total_velocity);
  bool IsMovable();

  // ViewEntity
  void Render(GamePoint offset);
};

#endif
