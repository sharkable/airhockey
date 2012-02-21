//
//  PlayState.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "PlayState.h"
#import "Post.h"
#import "GameEngine.h"
#import "SoundPlayer.h"
#import "MainMenuState.h"
#import "EAGLView.h"
#import "ResourceLoader.h"
#import "IsFree.h"
//#import "FlurryAPI.h"

@implementation PlayState

- (id) initWithNumPlayers:(int)numPlayers numPucks:(int)numPucks difficulty:(ComputerAI)difficulty paddleSize:(PaddleSize)paddleSize {
	[super init];
	
	_numPlayers = numPlayers;
	_roundThings = [[NSMutableArray alloc] init];
	_pucks = [[NSMutableArray alloc] init];
	
	_rink = [[Rink alloc] init];
	[self addEntity:_rink];
	
	NSMutableArray* scoreTextures = [NSMutableArray arrayWithCapacity:WIN_SCORE + 1];
	for (int i = 0; i <= WIN_SCORE; i++) {
		Texture2D* texture = [[ResourceLoader instance] getTextureWithName:[NSString stringWithFormat:@"%d_points", i]];
		[scoreTextures addObject:texture];
	}
	_player1Score = [[SimpleItem alloc] initWithTextures:scoreTextures position:CGPointMake(662, 526)];
	_player2Score = [[SimpleItem alloc] initWithTextures:scoreTextures position:CGPointMake(662, 386)];
	[self addEntity:_player1Score];
	[self addEntity:_player2Score];
	
	_numPucks = numPucks;
	_numActivePucks = _numPucks;
	for (int i = 0; i < _numPucks; i++) {
		Puck* puck = [[[Puck alloc] init] autorelease];
		
		[self addEntity:puck];
		[_pucks addObject:puck];
		[_roundThings addObject:puck];
	}
	
	_paddle1 = [[Paddle alloc] initWithPlayer:PLAYER_1
										 size:paddleSize
							 playerControlled:YES
									  aiLevel:0];
	[self addEntity:_paddle1];
	[_roundThings addObject:_paddle1];

	_paddle2 = [[Paddle alloc] initWithPlayer:PLAYER_2
										 size:paddleSize
							 playerControlled:numPlayers == 2
									  aiLevel:difficulty];
	[self addEntity:_paddle2];
	[_roundThings addObject:_paddle2];
	
	_paddle1.pucks = _pucks;
	_paddle1.otherPaddle = _paddle2;
	_paddle2.pucks = _pucks;
	_paddle2.otherPaddle = _paddle1;
	
	Post* post1 = [[[Post alloc] initWithX:GOAL_LEFT_X y:RINK_TOP_Y] autorelease];
	[self addEntity:post1];
	[_roundThings addObject:post1];

	Post* post2 = [[[Post alloc] initWithX:GOAL_LEFT_X y:RINK_BOTTOM_Y + 1] autorelease];
	[self addEntity:post2];
	[_roundThings addObject:post2];

	Post* post3 = [[[Post alloc] initWithX:GOAL_RIGHT_X + 1 y:RINK_TOP_Y] autorelease];
	[self addEntity:post3];
	[_roundThings addObject:post3];
	
	Post* post4 = [[[Post alloc] initWithX:GOAL_RIGHT_X + 1 y:RINK_BOTTOM_Y + 1] autorelease];
	[self addEntity:post4];
	[_roundThings addObject:post4];
	
	// Add rink left and right pieces.
	Texture2D* leftRinkBorderTexture = [[ResourceLoader instance] getTextureWithName:@"rink_left"];
	SimpleItem* leftRinkBorder = [[[SimpleItem alloc] initWithTexture:leftRinkBorderTexture
															 position:CGPointMake(0, 0)] autorelease];
	[self addEntity:leftRinkBorder];
	Texture2D* rightRinkBorderTexture = [[ResourceLoader instance] getTextureWithName:@"rink_right"];
	CGPoint leftRinkBorderPos = CGPointMake(SCREEN_WIDTH - rightRinkBorderTexture.contentSize.width, 0);
	SimpleItem*  rightRinkBorder = [[[SimpleItem alloc] initWithTexture:rightRinkBorderTexture
															   position:leftRinkBorderPos] autorelease];
	[self addEntity:rightRinkBorder];
	
	Texture2D* winTexture = [[ResourceLoader instance] getTextureWithName:@"win"];
	_win = [[SimpleItem alloc] initWithTexture:winTexture
									  position:CGPointMake(0, 0)];

	Texture2D* loseTexture = [[ResourceLoader instance] getTextureWithName:@"lose"];
	_lose = [[SimpleItem alloc] initWithTexture:loseTexture
									   position:CGPointMake(0, 0)];

	Texture2D* getReadyTexture = [[ResourceLoader instance] getTextureWithName:@"get_ready"];
	_getReady = [[SimpleItem alloc] initWithTexture:getReadyTexture
										   position:CGPointMake((SCREEN_WIDTH - getReadyTexture.contentSize.width) / 2, 
																(SCREEN_HEIGHT - getReadyTexture.contentSize.height) / 2)];

	Texture2D* goTexture = [[ResourceLoader instance] getTextureWithName:@"go"];
	_go = [[SimpleItem alloc] initWithTexture:goTexture
									 position:CGPointMake((SCREEN_WIDTH - goTexture.contentSize.width) / 2, 
														  (SCREEN_HEIGHT - goTexture.contentSize.height) / 2)];
	
	Texture2D* rematchButtonTexture = [[ResourceLoader instance] getTextureWithName:@"rematch_button"];
	Texture2D* rematchButtonPressedTexture = [[ResourceLoader instance] getTextureWithName:@"rematch_button_pressed"];
	CGPoint rematchButtonPos = CGPointMake((SCREEN_WIDTH - rematchButtonTexture.contentSize.width) / 2, 441);
	_rematchButton = [[Button alloc] initWithNormalTexture:rematchButtonTexture
											pressedTexture:rematchButtonPressedTexture
												  position:rematchButtonPos];
	_rematchButton.delegate = self;
	_rematchButton.selector = @selector(rematchPressed);

	Texture2D* menuButtonTexture = [[ResourceLoader instance] getTextureWithName:@"menu_button"];
	Texture2D* menuButtonPressedTexture = [[ResourceLoader instance] getTextureWithName:@"menu_button_pressed"];
	CGPoint menuButtonPos = CGPointMake((SCREEN_WIDTH - menuButtonTexture.contentSize.width) / 2, 546);
	_menuButton = [[Button alloc] initWithNormalTexture:menuButtonTexture
										 pressedTexture:menuButtonPressedTexture
											   position:menuButtonPos];
	_menuButton.delegate = self;
	_menuButton.selector = @selector(menuPressed);

	Texture2D* continueButtonTexture = [[ResourceLoader instance] getTextureWithName:@"continue_button"];
	Texture2D* continueButtonPressedTexture = [[ResourceLoader instance] getTextureWithName:@"continue_button_pressed"];
	CGPoint continueButtonPos = CGPointMake((SCREEN_WIDTH - continueButtonTexture.contentSize.width) / 2, 441);
	_continueButton = [[Button alloc] initWithNormalTexture:continueButtonTexture
											 pressedTexture:continueButtonPressedTexture
												   position:continueButtonPos];
	_continueButton.delegate = self;
	_continueButton.selector = @selector(continuePressed);
	
	_soundSlider = [[SoundSlider alloc] initWithPosition:CGPointMake(331, 336)];
	
	Texture2D* menuBackgroundTexture = [[ResourceLoader instance] getTextureWithName:@"game_menu_bg"];
	_menuBackground = [[SimpleItem alloc] initWithTexture:menuBackgroundTexture
												 position:CGPointMake((SCREEN_WIDTH - menuBackgroundTexture.contentSize.width) / 2, 306)];
	
	Texture2D* pauseButtonTexture = [[ResourceLoader instance] getTextureWithName:@"pause_button"];
	Texture2D* pauseButtonPressedTexture = [[ResourceLoader instance] getTextureWithName:@"pause_button_pressed"];
	
	BOOL isIPhone = (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone);
	
	if (!isIPhone) {
		CGPoint pauseButtonPos1 = CGPointMake(0, 0);
		_pauseButton1 = [[Button alloc] initWithNormalTexture:pauseButtonTexture
											   pressedTexture:pauseButtonPressedTexture
													 position:pauseButtonPos1];

		_pauseButton1.delegate = self;
		_pauseButton1.selector = @selector(pausePressed);
		[self addEntity:_pauseButton1];		
	}
	
	CGPoint pauseButtonPos2 = CGPointMake(SCREEN_WIDTH - pauseButtonTexture.contentSize.width,
										  SCREEN_HEIGHT - pauseButtonTexture.contentSize.height + (NO ? (27 * 768.0/320.0) : 0));
	_pauseButton2 = [[Button alloc] initWithNormalTexture:pauseButtonTexture
										   pressedTexture:pauseButtonPressedTexture
												 position:pauseButtonPos2];
	_pauseButton2.delegate = self;
	_pauseButton2.selector = @selector(pausePressed);
	[self addEntity:_pauseButton2];
		
	if (isIPhone) {
		if (IS_FREE) {
			_player1Wins = [[UILabel alloc] initWithFrame:CGRectMake(47, 278 + 26, 150, 35)];
			_player1Wins.textColor = [UIColor whiteColor];
		} else {
			_player1Wins = [[UILabel alloc] initWithFrame:CGRectMake(5, 448, 150, 35)];
			_player1Wins.textColor = [UIColor grayColor];
		}
	} else {
		_player1Wins = [[UILabel alloc] initWithFrame:CGRectMake(106, 644, 150, 35)];
		_player1Wins.textColor = [UIColor whiteColor];
	}
	_player1Wins.backgroundColor = [UIColor clearColor];
	_player1Wins.textAlignment = UITextAlignmentLeft;
	if (isIPhone) {
		_player1Wins.font = [UIFont fontWithName:@"Helvetica-Bold" size:20];
	} else {
		_player1Wins.font = [UIFont fontWithName:@"Helvetica-Bold" size:35];
	}
	
	if (isIPhone) {
		if (IS_FREE) {
			_player2Wins = [[UILabel alloc] initWithFrame:CGRectMake(47, 155 + 26, 150, 35)];
			_player2Wins.textColor = [UIColor whiteColor];
		} else {
			_player2Wins = [[UILabel alloc] initWithFrame:CGRectMake(5, -5, 150, 35)];
			_player2Wins.textColor = [UIColor grayColor];
		}
	} else {
		_player2Wins = [[UILabel alloc] initWithFrame:CGRectMake(106, 324, 150, 35)];
		_player2Wins.textColor = [UIColor whiteColor];
	}
	_player2Wins.backgroundColor = [UIColor clearColor];
	if (isIPhone) {
		_player2Wins.font = [UIFont fontWithName:@"Helvetica-Bold" size:20];
	} else {
		_player2Wins.font = [UIFont fontWithName:@"Helvetica-Bold" size:35];
	}
	if (_numPlayers == 2) {
		_player2Wins.transform = CGAffineTransformMakeRotation(M_PI); 
		_player2Wins.textAlignment = UITextAlignmentRight;
	} else {
		_player2Wins.textAlignment = UITextAlignmentLeft;
	}
	
	if (!IS_FREE && isIPhone) {
		_player1Wins.text = @"0 wins";
		_player2Wins.text = @"0 wins";
		[EAGLView addUIView:_player1Wins];		
		[EAGLView addUIView:_player2Wins];
	}
	
	_giveExtraPuckToPlayer = PLAYER_1;
	_player1WinCount = 0;
	_player2WinCount = 0;
	[self setUpNewGame];
	
	return self;
}

- (void) dealloc {
	[_rink           release];
	[_paddle1        release];
	[_paddle2        release];
	[_pucks          release];
	[_roundThings    release];
	[_player1Score   release];
	[_player2Score   release];
	[_win            release];
	[_lose           release];
	[_getReady       release];
	[_go             release];
	[_rematchButton  release];
	[_menuButton     release];
	[_soundSlider    release];
	[_menuBackground release];
	[_continueButton release];
	[_pauseButton1   release];
	[_pauseButton2   release];
	[_player1Wins    release];
	[_player2Wins    release];
	
	[super dealloc];
}

- (void) update {
	if (_state == PLAY_STATE_PAUSED) {
		return;
	} else if (_state == PLAY_STATE_GET_READY) {
		_getReadyTicksLeft--;
		if (_getReadyTicksLeft == SHOW_GET_READY_MESSAGE_TICKS) {
			[self addEntity:_getReady];
			[SoundPlayer playSound:kSoundGetReady];
		} else if (_getReadyTicksLeft == 0) {
			[self removeEntity:_getReady];
			[self addEntity:_go];
			_goTicksLeft = SHOW_GO_MESSAGE_TICKS;
			_state = PLAY_STATE_PLAYING;
			[SoundPlayer playSound:kSoundStart];
		}
		
		return;
	}

	[super update];
	
	if (_goTicksLeft > 0) {
		_goTicksLeft--;
		if (_goTicksLeft == 0) {
			[self removeEntity:_go];
		}
	}
	
	[_paddle1 keepInPlayerBounds];
	[_paddle2 keepInPlayerBounds];
	
	for (int i = 0; i < _roundThings.count; i++) {
		RoundThing* thing = [_roundThings objectAtIndex:i];
		if (!thing.active) {
			continue;
		}
		[_rink bounceOff:thing];
		for (int j = i + 1; j < _roundThings.count; j++) {
			RoundThing* otherThing = [_roundThings objectAtIndex:j];;
			if (otherThing.active) {
				[thing bounceOff:otherThing];
			}
		}
		
		[thing applyFriction];
		
		// TODO If you grab item A and push item B into a corner,
		// it only behaves if item A was added to _roundsThings
		// after item B. This is OK for Air Hockey, but should be fixed
		// for other games.
		[_rink moveInFromEdge:thing];
	}

	for (int i = 0; i < _pucks.count; i++) {
		Puck* puck = [_pucks objectAtIndex:i];
		if (!puck.active) {
			continue;
		}
		if (puck.y < -puck.radius) {
			puck.active = NO;
			if (_player1Score.texture < WIN_SCORE && _state == PLAY_STATE_PLAYING) {
				_player1Score.texture++;
			}
			if (_player1Score.texture == WIN_SCORE && _state == PLAY_STATE_PLAYING) {
				[SoundPlayer playSound:kSoundScoreFinal];	
			} else {
				[SoundPlayer playSound:kSoundScore];
			}
			_numPlayer1ScoresLastRound++;
			_numActivePucks--;
		} else if (puck.y > SCREEN_HEIGHT + puck.radius) {
			puck.active = NO;
			if (_player2Score.texture < WIN_SCORE && _state == PLAY_STATE_PLAYING) {
				_player2Score.texture++;
			}
			if (_player2Score.texture == WIN_SCORE && _state == PLAY_STATE_PLAYING) {
				[SoundPlayer playSound:kSoundScoreFinal];	
			} else {
				[SoundPlayer playSound:kSoundScore];
			}
			
			_numActivePucks--;
		}
	}	
	
	switch (_state) {
		case PLAY_STATE_PLAYING: {			
			if (_player1Score.texture == WIN_SCORE) {
				[self finishGameWithWinner:PLAYER_1];
			} else if (_player2Score.texture == WIN_SCORE) {
				[self finishGameWithWinner:PLAYER_2];
			} else if (_numActivePucks == 0) {
				_waitTicksLeft = WAIT_TICKS;
				_state = PLAY_STATE_WAITING_FOR_PUCKS;
			}
			break;
		}
		case PLAY_STATE_WAITING_FOR_PUCKS: {
			if (_waitTicksLeft-- == 0) {
				for (int i = 0; i < _numPucks; i++) {
					Puck* puck = [_pucks objectAtIndex:i];
					puck.active = YES;
					[puck placeForPlayer:(i < _numPlayer1ScoresLastRound ? PLAYER_2 : PLAYER_1)
							 roundThings:_roundThings
								  center:(i < _numPlayer1ScoresLastRound ? (_numPlayer1ScoresLastRound % 2 == 1)
										  : ((_numPucks - _numPlayer1ScoresLastRound) % 2 == 1))];
					[puck fadeIn];
				}
				_numActivePucks = _numPucks;
				_numPlayer1ScoresLastRound = 0;
				
				_state = PLAY_STATE_PLAYING;				

				break;
			}
		}
	}
}

- (void) setUpNewGame {
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
	} else {
		[EAGLView removeAd];
	}

	// Place paddles!
	[_paddle1 setInitialPositionForPlayer:PLAYER_1];
	[_paddle2 setInitialPositionForPlayer:PLAYER_2];
	
	// Place pucks!
	// First move them all out of the way. That way we can lay them out properly.
	// ([Puck placeForPlayer] avoids hitting other RoundThings objects.)
	for (int i = 0; i < _numPucks; i++) {
		Puck* puck = [_pucks objectAtIndex:i];
		puck.x = 0;
		puck.y = 0;
	}
	for (int i = 0; i < _numPucks; i++) {
		Puck* puck = [_pucks objectAtIndex:i];
		puck.active = YES;
		int playerId = (i % 2 == 0) ? _giveExtraPuckToPlayer : 1 - _giveExtraPuckToPlayer;
		BOOL center = !((playerId == _giveExtraPuckToPlayer && (_numPucks == 3 || _numPucks == 4 || _numPucks == 7)) ||
						(playerId == 1 - _giveExtraPuckToPlayer && (_numPucks == 4 || _numPucks == 5)));
		[puck placeForPlayer:playerId roundThings:_roundThings center:center];
	}
	
	_player1Score.texture = 0;
	_player2Score.texture = 0;
	[self removeEntity:_menuBackground];
	[self removeEntity:_soundSlider];
	[self removeEntity:_rematchButton];
	[self removeEntity:_menuButton];
	[self removeEntity:_win];
	[self removeEntity:_lose];
	
	_numActivePucks = _numPucks;
	_numPlayer1ScoresLastRound = 0;
	
	_state = PLAY_STATE_GET_READY;
	_getReadyTicksLeft = GET_READY_TICKS_TOTAL;
}

- (void) finishGameWithWinner:(int)playerId {
	_state = PLAY_STATE_FINISHED;
	
	double loseX = (SCREEN_WIDTH - _lose.size.width)/2;
	double winX =  (SCREEN_WIDTH - _win.size.width)/2;
	double topY = 70;
	double bottomY = SCREEN_HEIGHT - topY - _lose.size.height;
	switch (playerId) {
		case PLAYER_1: {
			_player1WinCount++;

			_win.position = CGPointMake(winX, bottomY);
			_win.angle = 0;
			[self addEntity:_win];
			
			if (_numPlayers == 2) {
				_lose.position = CGPointMake(loseX, topY);
				_lose.angle = 180;
				[self addEntity:_lose];
			}
			
			_giveExtraPuckToPlayer = PLAYER_2;
			
			break;
		}
		case PLAYER_2: {
			_player2WinCount++;
			
			if (_numPlayers == 2) {
				_win.position = CGPointMake(winX, topY);
				_win.angle = 180;			
				[self addEntity:_win];
			}
			
			_lose.position = CGPointMake(loseX, bottomY);
			_lose.angle = 0;
			[self addEntity:_lose];
			
			_giveExtraPuckToPlayer = PLAYER_1;
			
			break;
		}
	}

	_player1Wins.text = [NSString stringWithFormat:@"%d win%@", _player1WinCount, _player1WinCount == 1 ? @"" : @"s"];
	_player2Wins.text = [NSString stringWithFormat:@"%d win%@", _player2WinCount, _player2WinCount == 1 ? @"" : @"s"];
	if (IS_FREE || UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		[EAGLView addUIView:_player1Wins];
		[EAGLView addUIView:_player2Wins];
	}
	
	[self addEntity:_menuBackground];
	[self addEntity:_soundSlider];
	[self addEntity:_rematchButton];
	[self addEntity:_menuButton];
	
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		[EAGLView addAdAtPoint:CGPointMake((SCREEN_WIDTH - 320)/2, 385)];
	} else {
		[EAGLView addAdAtPoint:CGPointMake(0, 0)];
	}
}

- (void) rematchPressed {
//	[FlurryAPI logEvent:@"REMATCH"];
	if (IS_FREE || UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		[EAGLView removeUIView:_player1Wins];
		[EAGLView removeUIView:_player2Wins];
	}
	[self setUpNewGame];
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		[EAGLView removeAd];
	}	
}


- (void) menuPressed {
	[EAGLView removeUIView:_player1Wins];
	[EAGLView removeUIView:_player2Wins];
	[[GameEngine instance] replaceTopState:[[[MainMenuState alloc] init] autorelease]];
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		[EAGLView removeAd];
	}
}

- (void) continuePressed {
	_state = _prePauseState;
	[self removeEntity:_menuBackground];
	[self removeEntity:_soundSlider];
	[self removeEntity:_menuButton];
	[self removeEntity:_continueButton];
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		[EAGLView removeAd];
	}
}

- (void) pausePressed {
	if (_state != PLAY_STATE_FINISHED && _state != PLAY_STATE_PAUSED) {
		_prePauseState = _state;
		_state = PLAY_STATE_PAUSED;
		[self addEntity:_menuBackground];
		[self addEntity:_soundSlider];
		[self addEntity:_menuButton];
		[self addEntity:_continueButton];
		if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
			[EAGLView addAdAtPoint:CGPointMake((SCREEN_WIDTH - 320)/2, 385)];
		}
	}
}

- (void) touchesBegan:(Touch*[])touches numTouches:(int)numTouches {
	// When paused, only allow touches on the menu and continue buttons.
	if (_state == PLAY_STATE_PAUSED) {
		[_menuButton touchesBegan:touches numTouches:numTouches];
		[_continueButton touchesBegan:touches numTouches:numTouches];
		[_soundSlider touchesBegan:touches numTouches:numTouches];
	} else if (_state == PLAY_STATE_GET_READY) {
		[_pauseButton1 touchesBegan:touches numTouches:numTouches];
		[_pauseButton2 touchesBegan:touches numTouches:numTouches];
	} else {
		[super touchesBegan:touches numTouches:numTouches];
	}
}

- (void) touchesMoved:(Touch*[])touches numTouches:(int)numTouches {
	// When paused, only allow touches on the menu and continue buttons.
	if (_state == PLAY_STATE_PAUSED) {
		[_soundSlider touchesMoved:touches numTouches:numTouches];
	} else if (_state == PLAY_STATE_GET_READY) {
	} else {
		[super touchesMoved:touches numTouches:numTouches];
	}
}

- (void) touchesEnded:(Touch*[])touches numTouches:(int)numTouches {
	// When paused, only allow touches on the menu and continue buttons.
	if (_state == PLAY_STATE_PAUSED) {
		[_menuButton touchesEnded:touches numTouches:numTouches];
		[_continueButton touchesEnded:touches numTouches:numTouches];		
		[_soundSlider touchesEnded:touches numTouches:numTouches];
	} else if (_state == PLAY_STATE_GET_READY) {
		[_pauseButton1 touchesEnded:touches numTouches:numTouches];
		[_pauseButton2 touchesEnded:touches numTouches:numTouches];		
	} else {
		[super touchesEnded:touches numTouches:numTouches];
	}
}

- (void) clearTouches {
	[super clearTouches];
	[self pausePressed];
}

@end
