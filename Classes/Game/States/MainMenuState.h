//
//  MainMenuState.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAME_STATES_MAINMENUSTATE_H_
#define AIRHOCKEY_GAME_STATES_MAINMENUSTATE_H_

#import "game/entities/SoundSlider.h"
#import "gameengine/engine_state.h"
#import "gameengine/entities/button.h"
#import "gameengine/entities/multi_select.h"

class MainMenuState : public EngineState, private ButtonDelegate {
 public:
  MainMenuState(GameEngine &game_engine);
  
  // EngineState
  void StateIsShown();
  
  // ButtonDelegate
  void ButtonPressed(Button *button);

 private:
  void PressedStart();
  void PressedFeedback();
  void PressedStory();
  void PressedUpgrade();
  
  Button start_button_; 
  Button feedback_button_; 
  Button story_button_; 
  Button upgrade_button_; 
  MultiSelect num_players_select_;
  MultiSelect num_pucks_select_;
  MultiSelect difficulty_select_;
  MultiSelect paddle_size_select_;
  SoundSlider sound_slider_;
};

#endif
