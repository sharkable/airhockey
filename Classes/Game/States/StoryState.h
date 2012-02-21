//
//  StoryState.h
//  AirHockeyFree
//
//  Created by Jonathan Sharkey on 10-05-17.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "EngineState.h"
#import "Button.h"

@interface StoryState : EngineState {
  Button* _storyButton; 
  Button* _aboutButton; 
}

- (void) pressedStory;
- (void) pressedAbout;

@end
