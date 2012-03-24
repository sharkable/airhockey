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

#include <vector>
using namespace std;

#define PLAY_STATE_GET_READY 0
#define PLAY_STATE_PLAYING 1
#define PLAY_STATE_WAITING_FOR_PUCKS 2
#define PLAY_STATE_FINISHED 3
#define PLAY_STATE_PAUSED 4

@interface PlayState : EngineState {
 @private
  int numPlayers_;
  int numPucks_;
  int numActivePucks_;
  int numPlayer1ScoresLastRound_;
  Rink *rink_;
  Paddle *paddle1_;
  Paddle *paddle2_;
  vector<Puck *> pucks_;
  vector<RoundThing *> roundThings_;
  SimpleItem *player1Score_;
  SimpleItem *player2Score_;
  SimpleItem *win_;
  SimpleItem *lose_;
  SimpleItem *getReady_;
  SimpleItem *go_;
  SoundSlider *soundSlider_;
  SimpleItem *menuBackground_;
  Button *rematchButton_;
  Button *menuButton_;
  Button *continueButton_;
  Button *pauseButton1_;
  Button *pauseButton2_;
  
  int waitTicksLeft_;
  int getReadyTicksLeft_;
  int goTicksLeft_;
  int state_;
  int prePauseState_;
  int giveExtraPuckToPlayer_;
  int player1WinCount_;
  int player2WinCount_;
  
  UILabel *player1Wins_;
  UILabel *player2Wins_;
}

- (id)initWithGameEngine:(GameEngine *)gameEngine
              numPlayers:(int)numPlayers
                numPucks:(int)numPucks
              difficulty:(ComputerAI)difficulty
              paddleSize:(PaddleSize)paddleSize;
- (void)setUpNewGame;
- (void)finishGameWithWinner:(int)playerId;
- (void)rematchPressed;
- (void)menuPressed;
- (void)continuePressed;
- (void)pausePressed;

@end
