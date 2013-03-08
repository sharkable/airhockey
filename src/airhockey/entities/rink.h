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

#include "airhockey/const.h"

class RoundThing;

static const int kRinkEdgeWidth = 23;
static const int kRinkLeftX = kRinkEdgeWidth;
static const int kRinkRightX = (SCREEN_WIDTH - kRinkEdgeWidth);
static const int kRinkTopY = kRinkEdgeWidth;
static const int kRinkBottomY = (SCREEN_HEIGHT - kRinkEdgeWidth);
static const int kGoalLeftX = 200;
static const int kGoalRightX = 568;

class Rink : public ViewEntity {
 public:
  void BounceOff(RoundThing *thing);
  void MoveInFromEdge(RoundThing *thing);

  // ViewEntity
  void Update();
  void Render();
};

#endif
