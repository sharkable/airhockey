//
//  PlayState.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "EngineState.h"
#import "Rink.h"
#import "Puck.h"
#import "Paddle.h"
#import "SimpleItem.h"
#import "Button.h"
#import "SoundSlider.h"
#import "const.h"

#define PLAY_STATE_GET_READY         0
#define PLAY_STATE_PLAYING           1
#define PLAY_STATE_WAITING_FOR_PUCKS 2
#define PLAY_STATE_FINISHED          3
#define PLAY_STATE_PAUSED            4

@interface PlayState : EngineState {
  int numPlayers_;
  int numPucks_;
  int numActivePucks_;
  int numPlayer_1ScoresLastRound;
  Rink* rink_;
  Paddle* paddle_1;
  Paddle* paddle_2;
  NSMutableArray* pucks_;
  NSMutableArray* roundThings_;
  SimpleItem* player_1Score;
  SimpleItem* player_2Score;
  SimpleItem* win_;
  SimpleItem* lose_;
  SimpleItem* getReady_;
  SimpleItem* go_;
  SoundSlider* soundSlider_;
  SimpleItem*  menuBackground_;
  Button*      rematchButton_;
  Button*      menuButton_;
  Button*      continueButton_;
  Button*      pauseButton_1;
  Button*      pauseButton_2;
  
  int waitTicksLeft_;
  int getReadyTicksLeft_;
  int goTicksLeft_;
  int state_;
  int prePauseState_;
  int giveExtraPuckToPlayer_;
  int player_1WinCount;
  int player_2WinCount;
  
  UILabel* player_1Wins;
  UILabel* player_2Wins;
}

- (id) initWithNumPlayers:(int)numPlayers numPucks:(int)numPucks difficulty:(ComputerAI)difficulty paddleSize:(PaddleSize)paddleSize;
- (void) setUpNewGame;
- (void) finishGameWithWinner:(int)playerId;
- (void) rematchPressed;
- (void) menuPressed;
- (void) continuePressed;
- (void) pausePressed;

@end
