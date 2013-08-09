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
#include "gameengine/sprite.h"

RinkView::RinkView(GameEngine *game_engine) : EngineView(game_engine) {
  Sprite background_sprite(game_engine, "rink_bg");
  SimpleItem *background = new SimpleItem(background_sprite, game_engine->position("rink_bg"));
  AddEntity(sp<SimpleItem>(background));
}

GameSize RinkView::RinkSizeForPlatformType(PlatformType platform_type) {
  switch (platform_type) {
    case kPlatformTypePhone:
      return game_size_make(768, 1152);
    case kPlatformTypeTablet:
    case kPlatformTypePC:
      return game_size_make(768, 1024);
  }
}
