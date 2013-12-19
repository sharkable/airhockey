//
//  rink_overlay.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2013-05-07.
//  Copyright 2013 Sharkable. All rights reserved.
//

#include <string>
using std::string;

#include "gameengine/entities/simple_item.h"
#include "gameengine/game_engine.h"
#include "gameengine/resource_loader.h"

#include "airhockey/entities/rink_overlay.h"

RinkOverlay::RinkOverlay(GameEngine &game_engine) {
  string extention = game_engine.resource_loader().texture_name_extention();
  if (extention == "_iphone" || extention == "_iphone@2x") {
    // Add rink top and bottom pieces.

    Sprite rink_top_sprite(game_engine, "rink_top");
    SimpleItem *rink_top = new SimpleItem(rink_top_sprite, game_engine.position("rink_top"));
    AddRenderer(rink_top);

    Sprite rink_bottom_sprite(game_engine, "rink_bottom");
    SimpleItem *rink_bottom = new SimpleItem(rink_bottom_sprite,
                                             game_engine.position("rink_bottom"));
    AddRenderer(rink_bottom);
  }

  // Add rink left and right pieces.

  Sprite left_rink_border_sprite(game_engine, "rink_left");
  SimpleItem *left_rink_border = new SimpleItem(left_rink_border_sprite,
                                                game_engine.position("rink_left"));
  AddRenderer(left_rink_border);

  Sprite right_rink_border_sprite(game_engine, "rink_right");
  SimpleItem *right_rink_border = new SimpleItem(right_rink_border_sprite,
                                                 game_engine.position("rink_right"));
  AddRenderer(right_rink_border);
}
