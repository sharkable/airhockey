//
//  puck.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAME_ENTITIES_PUCK_H_
#define AIRHOCKEY_GAME_ENTITIES_PUCK_H_

#include "game/entities/round_thing.h"

class Puck : public RoundThing {
 public:
  Puck();

  void PlaceForPlayer(int player_id, const vector<sp<RoundThing> > &round_things, bool center);
  void FadeIn();

  // ViewEntity
  void Update();
  void Render();
  string Name() { return "Puck"; }

 private:
  int fade_ticks_left_;
  double alpha_;
};

#endif
