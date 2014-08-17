//
//  rink_view.h
//  AirHockey
//
//  Created by Jon Sharkey on 2013-03-01.
//  Copyright 2013 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_VIEWS_RINKVIEW_H_
#define AIRHOCKEY_VIEWS_RINKVIEW_H_

#include "sharkengine/platform.h"
#include "sharkengine/shark_engine.h"
#include "sharkengine/coordinates/coordinate_types.h"
#include "sharkengine/render/group_renderer.h"

class SharkEngine;

class RinkView : public GroupRenderer {
 public:
  RinkView(SharkEngine &game_engine);

  static GameSize RinkSizeForTextureGroup(Platform::TextureGroup texture_group);
};

#endif
