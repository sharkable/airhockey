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

@interface MainMenuState : EngineState <UIAlertViewDelegate> {
	Button*      _startButton; 
	Button*      _feedbackButton; 
	Button*      _upgradeButton; 
	MultiSelect* _numPlayersSelect;
	MultiSelect* _numPucksSelect;
	MultiSelect* _difficultySelect;
	MultiSelect* _paddleSizeSelect;
	SoundSlider* _soundSlider;
}

- (void) pressedStart;
- (void) pressedFeedback;
- (void) pressedUpgrade;

@end
