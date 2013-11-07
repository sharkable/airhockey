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

RinkView::RinkView(GameEngine &game_engine) : EngineView(game_engine) {
  Sprite background_sprite(game_engine, "rink_bg");
  SimpleItem *background = new SimpleItem(background_sprite, game_engine.position("rink_bg"));
  AddEntity(background);
}

GameSize RinkView::RinkSizeForTextureGroup(Platform::TextureGroup texture_group) {
  switch (texture_group) {
    case Platform::kTextureGroupIPhone35cmLowRes:
    case Platform::kTextureGroupIPhone35cmHighRes:
    case Platform::kTextureGroupIPhone40cmHighRes:
    case Platform::kTextureGroupAndroidLowRes:
    case Platform::kTextureGroupAndroidHighRes:
      return game_size_make(768, 1152);
    case Platform::kTextureGroupIPadLowRes:
    case Platform::kTextureGroupIPadHighRes:
    case Platform::kTextureGroupPCHighRes:
    case Platform::kTextureGroupPCUltraHighRes:
      return game_size_make(768, 1024);
  }
}
