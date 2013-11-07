//
//  game_menu_view.h
//  AirHockey
//
//  Created by Jon Sharkey on 2013-03-04.
//  Copyright 2013 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_VIEWS_GAMEMENUVIEW_H_
#define AIRHOCKEY_VIEWS_GAMEMENUVIEW_H_

#include "gameengine/entities/button.h"
#include "gameengine/engine_view.h"

class SimpleItem;
class SoundSlider;

class GameMenuViewDelegate {
 public:
  virtual void RematchPressed() = 0;
  virtual void MenuPressed() = 0;
  virtual void ContinuePressed() = 0;
};

class GameMenuView : public EngineView, private ButtonDelegate {
 public:
  GameMenuView(GameEngine &game_engine, GameMenuViewDelegate *delegate, bool match_finished);
  ~GameMenuView();

  // EngineView
  bool HandleBackButton();
  void HandlePauseButton();

  // ButtonDelegate
  void ButtonUp(Button *button);

 private:
  void Init(bool match_finished);

  GameMenuViewDelegate *delegate_;
  SoundSlider *sound_slider_;
  SimpleItem *menu_background_;
  Button *rematch_button_;
  Button *menu_button_;
  Button *continue_button_;
};

#endif
