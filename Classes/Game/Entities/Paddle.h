//
//  Paddle.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-12.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AirHockey_Paddle_h
#define AirHockey_Paddle_h

#import <Foundation/Foundation.h>
#import "Paddle.h"
#import "RoundThing.h"
#import "Texture2D.h"
#import "const.h"

class Puck;

#import <vector>
using namespace std;

class Paddle : public RoundThing {
 private:
  int        playerId_;
  BOOL       playerControlled_;
  ComputerAI aiLevel_;
  vector<Puck *> pucks_;
  Paddle*    otherPaddle_;
  BOOL       targetLeftCorner_;
  BOOL       targetRightCorner_;
  BOOL       targetAwayFromCorner_;

 public:
  Paddle(int playerId, PaddleSize size, bool playerControlled, ComputerAI aiLevel);
  ~Paddle();
  void update();
  void render();
  bool isGrabbable();
  bool containsTouch(Touch *touch);
  void setInitialPositionForPlayer(int playerId);
  void keepInPlayerBounds();

  vector<Puck *> getPucks() { return pucks_; }
  void setPucks(vector<Puck *> pucks) { pucks_ = pucks; }
  Paddle *getOtherPaddle() { return otherPaddle_; }
  void setOtherPaddle(Paddle *otherPaddle) { otherPaddle_ = otherPaddle; }
};

#endif
