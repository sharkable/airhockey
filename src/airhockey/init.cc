//
//  init.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2013-04-24.
//  Copyright 2013 Sharkable. All rights reserved.
//

#include "gameengine/game_engine.h"
#include "gameengine/game_engine_factory.h"

#include "airhockey/views/rink_view.h"
#include "airhockey/views/splash_view.h"

void sharkengine_init(sp<GameEngine> game_engine) {
  if (game_engine->platform_type() == kPlatformTypePhone) {
    game_engine->load_positions(
        *game_engine->factory()->createAssetReader("assets/positions/game_menu_iphone.xml"));
    game_engine->load_positions(
        *game_engine->factory()->createAssetReader("assets/positions/main_menu_iphone.xml"));
    game_engine->load_positions(
        *game_engine->factory()->createAssetReader("assets/positions/play_iphone.xml"));
    game_engine->load_positions(
        *game_engine->factory()->createAssetReader("assets/positions/rink_iphone.xml"));
  } else {
    game_engine->load_positions(
        *game_engine->factory()->createAssetReader("assets/positions/game_menu.xml"));
    game_engine->load_positions(
        *game_engine->factory()->createAssetReader("assets/positions/main_menu.xml"));
    game_engine->load_positions(
        *game_engine->factory()->createAssetReader("assets/positions/play.xml"));
    game_engine->load_positions(
        *game_engine->factory()->createAssetReader("assets/positions/rink.xml"));
  }

  int game_height = 768 * game_engine->screen_size().height / game_engine->screen_size().width;
  game_engine->SetGameSize(game_size_make(768, game_height));

  sp<EngineView> root_view = sp<EngineView>(new RinkView(game_engine));
  game_engine->SetRootView(root_view);

  sp<EngineView> splash_view = sp<EngineView>(new SplashView(game_engine));
  game_engine->PushView(splash_view);
}
