//
//  rink_view.cpp
//  AirHockey
//
//  Created by Jon Sharkey on 3/1/13.
//  Copyright 2013 Sharkable. All rights reserved.
//

#include "airhockey/views/rink_view.h"

#import "gameengine/entities/simple_item.h"
#import "gameengine/sprite.h"

RinkView::RinkView(sp<GameEngine> game_engine) : EngineView(game_engine) {
  Sprite background_sprite(game_engine, "rink_bg");
  SimpleItem *background = new SimpleItem(background_sprite, game_engine->position("rink_bg"));
  AddEntity(sp<SimpleItem>(background));
  
  // Add rink left and right pieces.
  Sprite left_rink_border_sprite(game_engine, "rink_left");
  SimpleItem *left_rink_border = new SimpleItem(left_rink_border_sprite,
                                                game_engine->position("rink_left"));
  AddEntity(left_rink_border);
  Sprite right_rink_border_sprite(game_engine, "rink_right");
  SimpleItem *right_rink_border = new SimpleItem(right_rink_border_sprite,
                                                 game_engine->position("rink_right"));
  AddEntity(right_rink_border);
}
