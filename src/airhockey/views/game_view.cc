//
//  game_view.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2013-12-18.
//  Copyright 2013 Sharkable. All rights reserved.
//

#include "airhockey/views/game_view.h"

GameView::GameView(GameEngine &game_engine)
    : EngineView(game_engine) ,
      rink_background_(game_engine),
      rink_overlay_(game_engine) {
  main_menu_view_ = new MainMenuView(game_engine);
}


#pragma mark - EngineView

void GameView::Update() {
  if (main_menu_view_) {
    main_menu_view_->SimulateStep();
  }
}

void GameView::Render() {
  rink_background_.Render(CoordinateSystem::BaseSystem());
  rink_overlay_.Render(CoordinateSystem::BaseSystem());

  if (main_menu_view_) {
    main_menu_view_->Render(CoordinateSystem::BaseSystem());
  }
}
