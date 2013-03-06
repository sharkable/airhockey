//
//  paddle.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-12.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAME_ENTITIES_PADDLE_H_
#define AIRHOCKEY_GAME_ENTITIES_PADDLE_H_

#include "const.h"
#include "airhockey/entities/round_thing.h"

class Puck;

#import <vector>
using namespace std;

class Paddle : public RoundThing {
 public:
  Paddle(sp<GameEngine> game_engine, int player_id, PaddleSize size, bool player_controlled,
         ComputerAI ai_level, vector<sp<Puck> > &pucks);

  void SetInitialPositionForPlayer(int player_id);
  void KeepInPlayerBounds();
  void RunAITick();

  // Accessors
  vector<sp<Puck> > &pucks() { return pucks_; }
  void set_ready_to_play(bool ready) {
    ready_to_play_ = ready;
    if (!ready) grabbed_ = false;
    ai_initial_pause_ticks_ = 30;  // TODO hacky
  }

  // ViewEntity
  void Update();
  void Render();

  // RoundThing
  void DidBounceOff(ViewEntity *other, double total_velocity);
  bool ContainsTouch(Touch *touch);
  bool IsGrabbable();
  bool IsMovable();
  // TODO hack for 1.2 release...
  void set_grabbed(bool grabbed) { grabbed_ = false; }

 private:
  int player_id_;
  bool player_controlled_;
  bool ready_to_play_;
  ComputerAI ai_level_;
  vector<sp<Puck> > &pucks_;
  bool target_left_corner_;
  bool target_right_corner_;
  bool target_away_from_corner_;
  int ai_initial_pause_ticks_;
};

#endif
