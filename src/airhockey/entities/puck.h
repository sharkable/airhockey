//
//  puck.h
//  AirHockey
//
//  Created by Jon Sharkey on 2010-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_ENTITIES_PUCK_H_
#define AIRHOCKEY_ENTITIES_PUCK_H_

#include "airhockey/entities/paddle.h"
#include "airhockey/entities/rink.h"
#include "airhockey/entities/round_thing.h"

class GameEngine;
class ViewEntity;
namespace SharkSound {
  class Sound;
}

class Puck : public RoundThing {
 public:
  Puck(GameEngine &game_engine, Rink &rink);

  void PlaceForPlayer(PlayerId player_id, const std::vector<RoundThing *> &round_things,
                      bool center);
  void FadeIn();

  // Simulator
  virtual void SimulateStep();

  // Renderer
  virtual void Render(CoordinateSystem const &coordinate_system);

  // RoundThing
  virtual void DidBounceOff(void *other, double total_velocity);

 private:
  Rink &rink_;
  bool hit_puck_last_time_;
  bool hit_puck_this_time_;
  bool hit_paddle_last_time_;
  bool hit_paddle_this_time_;
  bool hit_rink_last_time_;
  bool hit_rink_this_time_;
  int fade_ticks_left_;
  double alpha_;

  SharkSound::Sound *two_puck_hit_sound_;
  SharkSound::Sound *paddle_hit_sound_;
  SharkSound::Sound *puck_rink_bounce_sound_;
};

#endif
