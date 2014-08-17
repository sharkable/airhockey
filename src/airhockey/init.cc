//
//  init.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2013-04-24.
//  Copyright 2013 Sharkable. All rights reserved.
//

#include "sharkengine/platform.h"
#include "sharkengine/resource_loader.h"
#include "sharkengine/shark_engine.h"
#include "sharkengine/coordinates/coordinate_types.h"
#include "sharkengine/modules/ad_module.h"

#include "airhockey/views/game_view.h"
#include "airhockey/views/rink_view.h"

void sharkengine_init(GameEngine &game_engine) {
  switch (game_engine.platform().texture_group()) {
    case Platform::kTextureGroupIPhone35cmLowRes:
    case Platform::kTextureGroupIPhone35cmHighRes:
    case Platform::kTextureGroupIPhone40cmHighRes:
    case Platform::kTextureGroupAndroidLowRes:
    case Platform::kTextureGroupAndroidHighRes:
      game_engine.load_positions(*game_engine.LoadAsset("assets/positions/game_menu_iphone.xml"));
      game_engine.load_positions(*game_engine.LoadAsset("assets/positions/main_menu_iphone.xml"));
      game_engine.load_positions(*game_engine.LoadAsset("assets/positions/settings_iphone.xml"));
      game_engine.load_positions(*game_engine.LoadAsset("assets/positions/play_iphone.xml"));
      game_engine.load_positions(*game_engine.LoadAsset("assets/positions/rink_iphone.xml"));
      break;
    case Platform::kTextureGroupIPadLowRes:
    case Platform::kTextureGroupIPadHighRes:
    case Platform::kTextureGroupPCHighRes:
    case Platform::kTextureGroupPCUltraHighRes:
      game_engine.load_positions(*game_engine.LoadAsset("assets/positions/game_menu.xml"));
      game_engine.load_positions(*game_engine.LoadAsset("assets/positions/main_menu.xml"));
      game_engine.load_positions(*game_engine.LoadAsset("assets/positions/settings.xml"));
      game_engine.load_positions(*game_engine.LoadAsset("assets/positions/play.xml"));
      game_engine.load_positions(*game_engine.LoadAsset("assets/positions/rink.xml"));
      break;
  }

  switch (game_engine.platform().texture_group()) {
    case Platform::kTextureGroupIPhone35cmLowRes:
    case Platform::kTextureGroupAndroidLowRes:
      game_engine.resource_loader().set_texture_name_extention("_iphone");
      break;
    case Platform::kTextureGroupIPhone35cmHighRes:
    case Platform::kTextureGroupIPhone40cmHighRes:
    case Platform::kTextureGroupAndroidHighRes:
      game_engine.resource_loader().set_texture_name_extention("_iphone@2x");
      break;
    case Platform::kTextureGroupIPadLowRes:
    case Platform::kTextureGroupPCHighRes:
      game_engine.resource_loader().set_texture_name_extention("");
      break;
    case Platform::kTextureGroupIPadHighRes:
    case Platform::kTextureGroupPCUltraHighRes:
      game_engine.resource_loader().set_texture_name_extention("@2x");
      break;
  }

  if (game_engine.platform().os_group() == Platform::kOSGroupAndroid) {
    game_engine.ad_module()->SetPublisherId("a151d05623e3477");
  } else {
    game_engine.ad_module()->SetPublisherId("a14bdda6dfc895a");
  }
  game_engine.ad_module()->PrepareFullScreenAd();

  GameSize screen_size = game_engine.screen_size();
  GameSize game_size = RinkView::RinkSizeForTextureGroup(game_engine.platform().texture_group());

  game_engine.set_screen_to_game_point_ratios(screen_size.width / game_size.width,
                                              screen_size.width / game_size.width);
  double y_offset =
      (screen_size.height - game_engine.game_size_to_screen_size(game_size).height) / 2;
  game_engine.set_screen_offset(ScreenPoint(0, y_offset));

  GameView *view = new GameView(game_engine);
  game_engine.SetSimulator(view);
  game_engine.SetRenderer(view);
  game_engine.SetInputHandler(view);
}
