//
//  splash_view.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2010-05-07.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "airhockey/views/splash_view.h"

//#include "gameengine/modules/sound_player.h"
#include "gameengine/engine_view.h"
#include "gameengine/game_engine.h"

#include "airhockey/entities/rink_overlay.h"
#include "airhockey/views/main_menu_view.h"

SplashView::SplashView(GameEngine *game_engine) : EngineView(game_engine) {
  AddEntity(new RinkOverlay(game_engine));
}

void SplashView::Update() {
  EngineView::Update();
  game_engine()->PopView();
  game_engine()->PushView(sp<EngineView>(new MainMenuView(game_engine())));
}
