//
//  splash_view.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2010-05-07.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "airhockey/views/splash_view.h"

#include "gameengine/engine_view.h"
#include "gameengine/game_engine.h"
#include "soundengine/sound_player.h"

#include "airhockey/views/main_menu_view.h"

#include "airhockey.h"

SplashView::SplashView(sp<GameEngine> game_engine) : EngineView(game_engine) {
}

void SplashView::Update() {
  s_log("SplashView::Update() start");

  EngineView::Update();
  SoundPlayer::instance()->initialize();
  SoundPlayer::instance()->setSoundEffectsOn(true);
  game_engine()->PopView();
  game_engine()->PushView(sp<EngineView>(new MainMenuView(game_engine())));

  s_log("SplashView::Update() end");
}
