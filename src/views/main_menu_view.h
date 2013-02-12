//
//  main_menu_view.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAME_VIEWS_MAINMENUVIEW_H_
#define AIRHOCKEY_GAME_VIEWS_MAINMENUVIEW_H_

#import "gameengine/engine_view.h"
#import "gameengine/entities/button.h"
#import "gameengine/entities/multi_select.h"

#import "game/entities/sound_slider.h"

class MainMenuView : public EngineView, private ButtonDelegate {
 public:
  MainMenuView(sp<GameEngine> game_engine);
  
  // EngineView
  void ViewIsShown();

  // ButtonDelegate
  void ButtonPressed(Button *button);

 private:
  void PressedStart();
  void PressedFeedback();
  void PressedStory();
  void PressedUpgrade();
  
  sp<Button> start_button_;
  sp<Button> feedback_button_;
  sp<Button> story_button_;
  sp<Button> upgrade_button_;
  sp<MultiSelect> num_players_select_;
  sp<MultiSelect> num_pucks_select_;
  sp<MultiSelect> difficulty_select_;
  sp<MultiSelect> paddle_size_select_;
  sp<SoundSlider> sound_slider_;
};

#endif
