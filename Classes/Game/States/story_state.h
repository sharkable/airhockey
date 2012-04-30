//
//  story_state.h
//  AirHockeyFree
//
//  Created by Jonathan Sharkey on 10-05-17.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAME_STATES_STORYSTATE_H_
#define AIRHOCKEY_GAME_STATES_STORYSTATE_H_

#include "gameengine/engine_state.h"
#include "gameengine/entities/button.h"

class StoryState : public EngineState, ButtonDelegate {
 public:
  StoryState(GameEngine &gameEngine);

  // ButtonDelegate
  void ButtonPressed(Button *button);

 private:  
  Button storyButton_; 
  Button aboutButton_; 
};

#endif
