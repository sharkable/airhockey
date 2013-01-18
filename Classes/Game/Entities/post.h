//
//  post.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-25.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAME_ENTITIES_POST_H_
#define AIRHOCKEY_GAME_ENTITIES_POST_H_

#include "game/entities/round_thing.h"

class Post : public RoundThing {
 public:
  Post(double x, double y);
  
  // ViewEntity
  string Name() { return "Post"; }

  // RoundThing
  virtual void DidBounceOff(RoundThing *other);
  virtual bool IsMovable();
};

#endif
