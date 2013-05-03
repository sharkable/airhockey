//
//  rink_view.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2013-03-01.
//  Copyright 2013 Sharkable. All rights reserved.
//

#include "airhockey/views/rink_view.h"

#include <string>
using std::string;

#include "gameengine/entities/simple_item.h"
#include "gameengine/game_engine.h"
#include "gameengine/sprite.h"

RinkView::RinkView(sp<GameEngine> game_engine) : EngineView(game_engine) {
  string rink_bg_name;
  if (game_engine->platform_type() == kPlatformTypePhone) {
    rink_bg_name = "rink_bg_2_3";
  } else {
    rink_bg_name = "rink_bg_3_4";
  }
  Sprite background_sprite(game_engine, rink_bg_name);
  SimpleItem *background = new SimpleItem(background_sprite, game_engine->position(rink_bg_name));
  AddEntity(sp<SimpleItem>(background));
}
