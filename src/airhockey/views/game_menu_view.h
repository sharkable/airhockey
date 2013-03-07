//
//  game_menu_view.h
//  AirHockey
//
//  Created by Jon Sharkey on 3/4/13.
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
  GameMenuView(sp<GameEngine> game_engine, GameMenuViewDelegate *delegate, bool match_finished);

  // ButtonDelegate
  void ButtonPressed(Button *button);

 private:
  void Init(bool match_finished);
  
  GameMenuViewDelegate *delegate_;
  sp<SoundSlider> sound_slider_;
  sp<SimpleItem> menu_background_;
  sp<Button> rematch_button_;
  sp<Button> menu_button_;
  sp<Button> continue_button_;
};

#endif
