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

void sharkengine_init(SharkEngine &shark_engine) {
  switch (shark_engine.platform().texture_group()) {
    case Platform::kTextureGroupIPhone35cmLowRes:
    case Platform::kTextureGroupIPhone35cmHighRes:
    case Platform::kTextureGroupIPhone40cmHighRes:
    case Platform::kTextureGroupAndroidLowRes:
    case Platform::kTextureGroupAndroidHighRes:
      shark_engine.load_positions(*shark_engine.LoadAsset("assets/positions/game_menu_iphone.xml"));
      shark_engine.load_positions(*shark_engine.LoadAsset("assets/positions/main_menu_iphone.xml"));
      shark_engine.load_positions(*shark_engine.LoadAsset("assets/positions/settings_iphone.xml"));
      shark_engine.load_positions(*shark_engine.LoadAsset("assets/positions/play_iphone.xml"));
      shark_engine.load_positions(*shark_engine.LoadAsset("assets/positions/rink_iphone.xml"));
      break;
    case Platform::kTextureGroupIPadLowRes:
    case Platform::kTextureGroupIPadHighRes:
    case Platform::kTextureGroupPCHighRes:
    case Platform::kTextureGroupPCUltraHighRes:
      shark_engine.load_positions(*shark_engine.LoadAsset("assets/positions/game_menu.xml"));
      shark_engine.load_positions(*shark_engine.LoadAsset("assets/positions/main_menu.xml"));
      shark_engine.load_positions(*shark_engine.LoadAsset("assets/positions/settings.xml"));
      shark_engine.load_positions(*shark_engine.LoadAsset("assets/positions/play.xml"));
      shark_engine.load_positions(*shark_engine.LoadAsset("assets/positions/rink.xml"));
      break;
  }

  switch (shark_engine.platform().texture_group()) {
    case Platform::kTextureGroupIPhone35cmLowRes:
    case Platform::kTextureGroupAndroidLowRes:
      shark_engine.resource_loader().set_texture_name_extention("_iphone");
      break;
    case Platform::kTextureGroupIPhone35cmHighRes:
    case Platform::kTextureGroupIPhone40cmHighRes:
    case Platform::kTextureGroupAndroidHighRes:
      shark_engine.resource_loader().set_texture_name_extention("_iphone@2x");
      break;
    case Platform::kTextureGroupIPadLowRes:
    case Platform::kTextureGroupPCHighRes:
      shark_engine.resource_loader().set_texture_name_extention("");
      break;
    case Platform::kTextureGroupIPadHighRes:
    case Platform::kTextureGroupPCUltraHighRes:
      shark_engine.resource_loader().set_texture_name_extention("@2x");
      break;
  }

  if (shark_engine.platform().os_group() == Platform::kOSGroupAndroid) {
    shark_engine.ad_module()->SetPublisherId("a151d05623e3477");
  } else {
    shark_engine.ad_module()->SetPublisherId("a14bdda6dfc895a");
  }
  shark_engine.ad_module()->PrepareFullScreenAd();

  GameSize screen_size = shark_engine.screen_size();
  GameSize game_size = RinkView::RinkSizeForTextureGroup(shark_engine.platform().texture_group());

  shark_engine.set_screen_to_game_point_ratios(screen_size.width / game_size.width,
                                              screen_size.width / game_size.width);
  double y_offset =
      (screen_size.height - shark_engine.game_size_to_screen_size(game_size).height) / 2;
  shark_engine.set_screen_offset(ScreenPoint(0, y_offset));

  GameView *view = new GameView(shark_engine);
  shark_engine.SetSimulator(view);
  shark_engine.SetRenderer(view);
  shark_engine.SetInputHandler(view);
}
