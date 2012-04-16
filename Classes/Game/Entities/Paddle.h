//
//  Paddle.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-12.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AirHockey_Paddle_h
#define AirHockey_Paddle_h

#import "Paddle.h"
#import "RoundThing.h"
#import "Texture2D.h"
#import "const.h"

class Puck;

#import <vector>
using namespace std;

class Paddle : public RoundThing {
 private:
  int playerId_;
  bool playerControlled_;
  ComputerAI aiLevel_;
  vector<Puck> &pucks_;
  Paddle *otherPaddle_;
  bool targetLeftCorner_;
  bool targetRightCorner_;
  bool targetAwayFromCorner_;

 public:
  Paddle(int playerId, PaddleSize size, bool playerControlled, ComputerAI aiLevel,
         vector<Puck> &pucks);
  ~Paddle();
  void Update();
  void Render();
  bool isGrabbable();
  bool containsTouch(Touch *touch);
  void setInitialPositionForPlayer(int playerId);
  void keepInPlayerBounds();

  vector<Puck> &getPucks() { return pucks_; }
  Paddle *getOtherPaddle() { return otherPaddle_; }
  void setOtherPaddle(Paddle *otherPaddle) { otherPaddle_ = otherPaddle; }
};

#endif
