//
//  init.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2013-04-24.
//  Copyright 2013 Sharkable. All rights reserved.
//

#include "gameengine/modules/ad_engine.h"
#include "gameengine/coordinate_types.h"
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
        *game_engine->factory()->createAssetReader("assets/positions/settings_iphone.xml"));
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
        *game_engine->factory()->createAssetReader("assets/positions/settings.xml"));
    game_engine->load_positions(
        *game_engine->factory()->createAssetReader("assets/positions/play.xml"));
    game_engine->load_positions(
        *game_engine->factory()->createAssetReader("assets/positions/rink.xml"));
  }

#ifdef __ANDROID__
  game_engine->ad_engine()->SetPublisherId("a151d05623e3477");
#else
  game_engine->ad_engine()->SetPublisherId("a14bdda6dfc895a");
#endif
  game_engine->ad_engine()->PrepareFullScreenAd();

  ScreenSize screen_size = game_engine->screen_size();
  GameSize game_size = RinkView::RinkSizeForPlatformType(game_engine->platform_type());

  game_engine->set_screen_to_game_point_ratio(screen_size.width / game_size.width);
  double y_offset =
      (screen_size.height - game_engine->game_size_to_screen_size(game_size).height) / 2;
  game_engine->set_screen_offset(screen_point_make(0, y_offset));

  sp<EngineView> root_view = sp<EngineView>(new RinkView(game_engine));
  game_engine->SetRootView(root_view);

  sp<EngineView> splash_view = sp<EngineView>(new SplashView(game_engine));
  game_engine->PushView(splash_view);
}
