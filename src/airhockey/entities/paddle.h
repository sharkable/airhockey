//
//  paddle.h
//  AirHockey
//
//  Created by Jon Sharkey on 2010-04-12.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_ENTITIES_PADDLE_H_
#define AIRHOCKEY_ENTITIES_PADDLE_H_

#include "airhockey/entities/round_thing.h"
#include "airhockey/entities/rink.h"

class Puck;

#include <vector>

typedef enum {
  kPlayerId1 = 0,
  kPlayerId2
} PlayerId;

typedef enum {
  kComputerAIBad = 0,
  kComputerAIGood,
  kComputerAIExcellent,
  kComputerAIAmazing,
} ComputerAI;

typedef enum {
  kPaddleSizeSmall = 0,
  kPaddleSizeMedium,
  kPaddleSizeLarge,
} PaddleSize;

class Paddle : public RoundThing {
 public:
  Paddle(GameEngine &game_engine, Rink &rink, PlayerId player_id, PaddleSize size,
         bool player_controlled, ComputerAI ai_level, std::vector<Puck *> &pucks);

  void SetInitialPosition();
  void KeepInPlayerBounds();
  void SetReadyToPlay(bool ready);
  
  // Accessors
  std::vector<Puck *> &pucks() { return pucks_; }

  // ViewEntity
  void Update();
  void Render(GamePoint offset);

  // RoundThing
  void DidBounceOff(ViewEntity *other, double total_velocity);
  bool ContainsPoint(GamePoint p);
  bool IsGrabbable();
  bool IsMovable();
  bool is_grabbed();

 private:
  void RunAITick();
  double StartingX();
  double StartingY();

  Rink &rink_;
  PlayerId player_id_;
  bool player_controlled_;
  bool ready_to_play_;
  ComputerAI ai_level_;
  std::vector<Puck *> &pucks_;
  bool target_left_corner_;
  bool target_right_corner_;
  bool target_away_from_corner_;
  int ai_initial_pause_ticks_;
  bool always_grabbed_;
};

#endif
