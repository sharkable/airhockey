//
//  rink.h
//  AirHockey
//
//  Created by Jon Sharkey on 2010-04-13.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_ENTITIES_RINK_H_
#define AIRHOCKEY_ENTITIES_RINK_H_

#include "gameengine/coordinates/coordinate_types.h"

class RoundThing;

class Rink {
 public:
  Rink(GameSize size);

  double EdgeWidth() { return 23; }
  double TotalWidth() { return size_.width; }
  double TotalHeight() { return size_.height; }
  double LeftX() { return EdgeWidth(); }
  double CenterX() { return TotalWidth() / 2; }
  double RightX() { return TotalWidth() - EdgeWidth(); }
  double TopY() { return EdgeWidth(); }
  double CenterY() { return TotalHeight() / 2; }
  double BottomY() { return TotalHeight() - EdgeWidth(); }
  double GoalLeftX() { return 200; }
  double GoalRightX() { return 568; }

  void BounceOff(RoundThing *thing);
  void MoveInFromEdge(RoundThing *thing);

 private:
  GameSize size_;
};

#endif
