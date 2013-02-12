//
//  post.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-25.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAME_ENTITIES_POST_H_
#define AIRHOCKEY_GAME_ENTITIES_POST_H_

#include "airhockey/entities/round_thing.h"

class Post : public RoundThing {
 public:
  Post(sp<GameEngine> game_engine, double x, double y);
  
  // RoundThing
  virtual void DidBounceOff(ViewEntity *other);
  virtual bool IsMovable();
};

#endif
