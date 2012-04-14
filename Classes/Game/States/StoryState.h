//
//  StoryState.h
//  AirHockeyFree
//
//  Created by Jonathan Sharkey on 10-05-17.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "engine_state.h"
#import "button.h"

class StoryState : public EngineState {
 public:
  StoryState(GameEngine &gameEngine);
  ~StoryState();
  void pressedStory();
  void pressedAbout();

 private:
  Button *storyButton_; 
  Button *aboutButton_; 
};
