//
//  MainMenuState.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "EngineState.h"
#import "SimpleItem.h"
#import "Button.h"
#import "MultiSelect.h"
#import "SoundSlider.h"

class MainMenuState : public EngineState, private ButtonDelegate {
 public:
  MainMenuState(GameEngine *gameEngine);
  void stateIsShown();
  void pressedStart();
  void pressedFeedback();
  void pressedStory();
  void pressedUpgrade();
  void buttonPressed(Button *button);

 private:
  Button startButton_; 
  Button feedbackButton_; 
  Button storyButton_; 
  Button upgradeButton_; 
  MultiSelect numPlayersSelect_;
  MultiSelect numPucksSelect_;
  MultiSelect difficultySelect_;
  MultiSelect paddleSizeSelect_;
  SoundSlider soundSlider_;
};
