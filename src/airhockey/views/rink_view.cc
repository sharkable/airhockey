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
  GameSize image_size = background_sprite.content_size();
  GameSize rink_size = RinkSizeForScreenSize(game_engine->screen_size());
  GamePoint rink_point = game_point_make((rink_size.width - image_size.width) / 2,
                                         (rink_size.height - image_size.height) / 2);
  SimpleItem *background = new SimpleItem(background_sprite, rink_point);
  AddEntity(sp<SimpleItem>(background));
}

GameSize RinkView::RinkSizeForScreenSize(ScreenSize screen_size) {
  double screen_ratio = screen_size.width / screen_size.height;
  if (screen_ratio <= 2.0 / 3.0) {
    return game_size_make(768, 1152);
  } else {
    return game_size_make(768, 1024);
  }
}
