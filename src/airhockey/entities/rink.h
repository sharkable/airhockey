//
//  rink.h
//  AirHockey
//
//  Created by Jon Sharkey on 2010-04-13.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_ENTITIES_RINK_H_
#define AIRHOCKEY_ENTITIES_RINK_H_

#include "gameengine/view_entity.h"

class RoundThing;

class Rink : public ViewEntity {
 public:
  static int EdgeWidth() { return 23; }
  static int TotalWidth() { return 768; }
  static int TotalHeight() { return 1024; }
  static int LeftX() { return EdgeWidth(); }
  static int CenterX() { return TotalWidth() / 2; }
  static int RightX() { return TotalWidth() - EdgeWidth(); }
  static int TopY() { return EdgeWidth(); }
  static int CenterY() { return TotalHeight() / 2; }
  static int BottomY() { return TotalHeight() - EdgeWidth(); }
  static int GoalLeftX() { return 200; }
  static int GoalRightX() { return 568; }

  void BounceOff(RoundThing *thing);
  void MoveInFromEdge(RoundThing *thing);

  // ViewEntity
  void Update();
  void Render();
};

#endif
