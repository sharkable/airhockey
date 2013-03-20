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
  static const int kRinkTotalWidth;
  static const int kRinkTotalHeight;
  static const int kRinkLeftX;
  static const int kRinkCenterX;
  static const int kRinkRightX;
  static const int kRinkTopY;
  static const int kRinkCenterY;
  static const int kRinkBottomY;
  static const int kGoalLeftX;
  static const int kGoalRightX;

  void BounceOff(RoundThing *thing);
  void MoveInFromEdge(RoundThing *thing);

  // ViewEntity
  void Update();
  void Render();
};

#endif
