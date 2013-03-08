//
//  rink_view.cpp
//  AirHockey
//
//  Created by Jon Sharkey on 2013-03-01.
//  Copyright 2013 Sharkable. All rights reserved.
//

#include "airhockey/views/rink_view.h"

#import "gameengine/entities/simple_item.h"
#import "gameengine/sprite.h"

RinkView::RinkView(sp<GameEngine> game_engine) : EngineView(game_engine) {
  Sprite background_sprite(game_engine, "rink_bg");
  SimpleItem *background = new SimpleItem(background_sprite, game_engine->position("rink_bg"));
  AddEntity(sp<SimpleItem>(background));
}
