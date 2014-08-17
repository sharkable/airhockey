//
//  game_view.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2013-12-18.
//  Copyright 2013 Sharkable. All rights reserved.
//

#include "airhockey/views/game_view.h"

#include "airhockey/views/main_menu_view.h"
#include "airhockey/views/play_view.h"

GameView::GameView(SharkEngine &shark_engine)
    : shark_engine_(shark_engine),
      rink_background_(shark_engine),
      rink_overlay_(shark_engine),
      main_menu_view_(nullptr),
      play_view_(nullptr) {
  ShowMainMenu();
}

void GameView::ShowMainMenu() {
  if (!main_menu_view_) {
    main_menu_view_ = new MainMenuView{shark_engine_, *this};
  }
}

void GameView::RemoveMainMenu() {
  if (main_menu_view_) {
    delete main_menu_view_;
    main_menu_view_ = nullptr;
  }
}

void GameView::ShowPlay(int num_players, int num_pucks, ComputerAI difficulty,
                        PaddleSize paddle_size) {
  if (!play_view_) {
    play_view_ =
        new PlayView{shark_engine_, *this, num_players, num_pucks, difficulty, paddle_size};
  }
}

void GameView::RemovePlay() {
  if (play_view_) {
    delete play_view_;
    play_view_ = nullptr;
  }
}


#pragma mark - Simulator

void GameView::SimulateStep() {
  if (main_menu_view_) {
    main_menu_view_->SimulateStep();
  }
  if (play_view_) {
    play_view_->SimulateStep();
  }
}


#pragma mark - Renderer

void GameView::Render(CoordinateSystem const &coordinate_system) {
  rink_background_.Render(CoordinateSystem::BaseSystem());
  rink_overlay_.Render(CoordinateSystem::BaseSystem());

  if (main_menu_view_) {
    main_menu_view_->Render(CoordinateSystem::BaseSystem());
  }
  if (play_view_) {
    play_view_->Render(CoordinateSystem::BaseSystem());
  }
}


#pragma mark - InputHandler

bool GameView::HandleInputEvent(InputEvent const &event,
                                CoordinateSystem const &coordinate_system) {
  if (main_menu_view_) {
    main_menu_view_->HandleInputEvent(event, coordinate_system);
  }
  if (play_view_) {
    play_view_->HandleInputEvent(event, coordinate_system);
  }
  return true;
}
