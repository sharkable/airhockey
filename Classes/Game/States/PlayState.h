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
	int _numPlayers;
	int _numPucks;
	int _numActivePucks;
	int _numPlayer1ScoresLastRound;
	Rink* _rink;
	Paddle* _paddle1;
	Paddle* _paddle2;
	NSMutableArray* _pucks;
	NSMutableArray* _roundThings;
	SimpleItem* _player1Score;
	SimpleItem* _player2Score;
	SimpleItem* _win;
	SimpleItem* _lose;
	SimpleItem* _getReady;
	SimpleItem* _go;
	SoundSlider* _soundSlider;
	SimpleItem*  _menuBackground;
	Button*      _rematchButton;
	Button*      _menuButton;
	Button*      _continueButton;
	Button*      _pauseButton1;
	Button*      _pauseButton2;
	
	int _waitTicksLeft;
	int _getReadyTicksLeft;
	int _goTicksLeft;
	int _state;
	int _prePauseState;
	int _giveExtraPuckToPlayer;
	int _player1WinCount;
	int _player2WinCount;
	
	UILabel* _player1Wins;
	UILabel* _player2Wins;
}

- (id) initWithNumPlayers:(int)numPlayers numPucks:(int)numPucks difficulty:(ComputerAI)difficulty paddleSize:(PaddleSize)paddleSize;
- (void) setUpNewGame;
- (void) finishGameWithWinner:(int)playerId;
- (void) rematchPressed;
- (void) menuPressed;
- (void) continuePressed;
- (void) pausePressed;

@end
