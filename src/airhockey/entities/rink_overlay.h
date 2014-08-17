//
//  rink_overlay.h
//  AirHockey
//
//  Created by Jon Sharkey on 2013-05-07.
//  Copyright 2013 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_ENTITIES_RINKOVERLAY_H_
#define AIRHOCKEY_ENTITIES_RINKOVERLAY_H_

#include "sharkengine/render/group_renderer.h"

class SharkEngine;

class RinkOverlay : public GroupRenderer {
 public:
  RinkOverlay(SharkEngine &shark_engine);
};

#endif
