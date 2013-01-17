//
//  story_state.h
//  AirHockeyFree
//
//  Created by Jonathan Sharkey on 10-05-17.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAME_STATES_STORYSTATE_H_
#define AIRHOCKEY_GAME_STATES_STORYSTATE_H_

#include "gameengine/engine_view.h"
#include "gameengine/entities/button.h"

class StoryState : public EngineView, ButtonDelegate {
 public:
  StoryState(GameEngine &gameEngine);

  // EngineState
  string Name() { return "StoryState"; }
  
  // ButtonDelegate
  void ButtonPressed(Button *button);

 private:  
  sp<Button> storyButton_;
  sp<Button> aboutButton_;
};

#endif
