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

RinkOverlay::RinkOverlay(GameEngine *game_engine) {
  string extention = game_engine->resource_loader().texture_name_extention();
  if (extention == "_iphone" || extention == "_iphone@2x") {
    // Add rink top and bottom pieces.

    Sprite rink_top_sprite(game_engine, "rink_top");
    SimpleItem *rink_top = new SimpleItem(rink_top_sprite, game_engine->position("rink_top"));
    rink_pieces_.AddEntity(rink_top);

    Sprite rink_bottom_sprite(game_engine, "rink_bottom");
    SimpleItem *rink_bottom = new SimpleItem(rink_bottom_sprite,
                                             game_engine->position("rink_bottom"));
    rink_pieces_.AddEntity(rink_bottom);
  }

  // Add rink left and right pieces.

  Sprite left_rink_border_sprite(game_engine, "rink_left");
  SimpleItem *left_rink_border = new SimpleItem(left_rink_border_sprite,
                                                game_engine->position("rink_left"));
  rink_pieces_.AddEntity(left_rink_border);

  Sprite right_rink_border_sprite(game_engine, "rink_right");
  SimpleItem *right_rink_border = new SimpleItem(right_rink_border_sprite,
                                                 game_engine->position("rink_right"));
  rink_pieces_.AddEntity(right_rink_border);
}


#pragma mark - ViewEntity

void RinkOverlay::Update() {
  rink_pieces_.Update();
}

void RinkOverlay::Render(GamePoint offset) {
  rink_pieces_.Render(offset);
}
