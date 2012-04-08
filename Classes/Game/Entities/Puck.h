//
//  Puck.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AirHockey_Puck_h
#define AirHockey_Puck_h

#import "RoundThing.h"
#import "Texture2D.h"
#import "Paddle.h"

class Puck : public RoundThing {
 private:
  int fadeTicksLeft_;
  double alpha_;
 public:
  Puck();
  void update();
  void render();
  void placeForPlayer(int playerId, const vector<RoundThing *> &roundThings, bool center);
  void fadeIn();
};

#endif
