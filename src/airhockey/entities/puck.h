//
//  puck.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAME_ENTITIES_PUCK_H_
#define AIRHOCKEY_GAME_ENTITIES_PUCK_H_

#include "airhockey/entities/round_thing.h"

class Puck : public RoundThing {
 public:
  Puck(sp<GameEngine> game_engine);

  void PlaceForPlayer(int player_id, const vector<sp<RoundThing> > &round_things, bool center);
  void FadeIn();

  // ViewEntity
  void Update();
  void Render();
  
  // RoundThing
  void DidBounceOff(ViewEntity *other, double total_velocity);
  
 private:
  bool hit_puck_last_time_;
  bool hit_puck_this_time_;
  bool hit_paddle_last_time_;
  bool hit_paddle_this_time_;
  bool hit_rink_last_time_;
  bool hit_rink_this_time_;
  int fade_ticks_left_;
  double alpha_;
};

#endif
