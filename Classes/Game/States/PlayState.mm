//
//  PlayState.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "PlayState.h"

#import "AdEngine.h"
#import "Post.h"
#import "GameEngine.h"
#import "SoundPlayer.h"
#import "MainMenuState.h"
#import "EAGLView.h"
#import "ResourceLoader.h"
#import "FlurryAnalytics.h"

@implementation PlayState

- (id)initWithNumPlayers:(int)numPlayers
                numPucks:(int)numPucks
              difficulty:(ComputerAI)difficulty
              paddleSize:(PaddleSize)paddleSize {
  [super init];
  
  if (self) {
    numPlayers_ = numPlayers;
    roundThings_ = [[NSMutableArray alloc] init];
    pucks_ = [[NSMutableArray alloc] init];
    
    rink_ = [[Rink alloc] init];
    [self addEntity:rink_];
    
    NSMutableArray *scoreTextures = [NSMutableArray arrayWithCapacity:WIN_SCORE + 1];
    for (int i = 0; i <= WIN_SCORE; i++) {
      Texture2D *texture = [[ResourceLoader instance]
                            getTextureWithName:[NSString stringWithFormat:@"%d_points", i]];
      [scoreTextures addObject:texture];
    }
    player1Score_ = [[SimpleItem alloc] initWithTextures:scoreTextures
                                                position:CGPointMake(662, 526)];
    player2Score_ = [[SimpleItem alloc] initWithTextures:scoreTextures
                                                position:CGPointMake(662, 386)];
    [self addEntity:player1Score_];
    [self addEntity:player2Score_];
    
    numPucks_ = numPucks;
    numActivePucks_ = numPucks_;
    for (int i = 0; i < numPucks_; i++) {
      Puck* puck = [[[Puck alloc] init] autorelease];
      
      [self addEntity:puck];
      [pucks_ addObject:puck];
      [roundThings_ addObject:puck];
    }
    
    paddle1_ = [[Paddle alloc] initWithPlayer:PLAYER_1
                                         size:paddleSize
                             playerControlled:YES
                                      aiLevel:caiBad];
    [self addEntity:paddle1_];
    [roundThings_ addObject:paddle1_];

    paddle2_ = [[Paddle alloc] initWithPlayer:PLAYER_2
                                         size:paddleSize
                             playerControlled:numPlayers == 2
                                      aiLevel:difficulty];
    [self addEntity:paddle2_];
    [roundThings_ addObject:paddle2_];
    
    paddle1_.pucks = pucks_;
    paddle1_.otherPaddle = paddle2_;
    paddle2_.pucks = pucks_;
    paddle2_.otherPaddle = paddle1_;
    
    Post *post1 = [[[Post alloc] initWithX:GOAL_LEFT_X y:RINK_TOP_Y] autorelease];
    [self addEntity:post1];
    [roundThings_ addObject:post1];

    Post *post2 = [[[Post alloc] initWithX:GOAL_LEFT_X y:RINK_BOTTOM_Y + 1] autorelease];
    [self addEntity:post2];
    [roundThings_ addObject:post2];

    Post *post3 = [[[Post alloc] initWithX:GOAL_RIGHT_X + 1 y:RINK_TOP_Y] autorelease];
    [self addEntity:post3];
    [roundThings_ addObject:post3];
    
    Post *post4 = [[[Post alloc] initWithX:GOAL_RIGHT_X + 1 y:RINK_BOTTOM_Y + 1] autorelease];
    [self addEntity:post4];
    [roundThings_ addObject:post4];
    
    // Add rink left and right pieces.
    Texture2D *leftRinkBorderTexture = [[ResourceLoader instance] getTextureWithName:@"rink_left"];
    SimpleItem *leftRinkBorder = [[[SimpleItem alloc] initWithTexture:leftRinkBorderTexture
                                                             position:CGPointMake(0, 0)]
                                  autorelease];
    [self addEntity:leftRinkBorder];
    Texture2D *rightRinkBorderTexture =
        [[ResourceLoader instance] getTextureWithName:@"rink_right"];
    CGPoint leftRinkBorderPos = CGPointMake(SCREEN_WIDTH - rightRinkBorderTexture.contentSize.width,
                                            0);
    SimpleItem *rightRinkBorder = [[[SimpleItem alloc] initWithTexture:rightRinkBorderTexture
                                   position:leftRinkBorderPos] autorelease];
    [self addEntity:rightRinkBorder];
    
    Texture2D *winTexture = [[ResourceLoader instance] getTextureWithName:@"win"];
    win_ = [[SimpleItem alloc] initWithTexture:winTexture
                                      position:CGPointMake(0, 0)];

    Texture2D *loseTexture = [[ResourceLoader instance] getTextureWithName:@"lose"];
    lose_ = [[SimpleItem alloc] initWithTexture:loseTexture
                                       position:CGPointMake(0, 0)];

    Texture2D *getReadyTexture = [[ResourceLoader instance] getTextureWithName:@"get_ready"];
    CGPoint getReadyPosition =
        CGPointMake((SCREEN_WIDTH - getReadyTexture.contentSize.width) / 2, 
                    (SCREEN_HEIGHT - getReadyTexture.contentSize.height) / 2);
    getReady_ = [[SimpleItem alloc] initWithTexture:getReadyTexture
                                           position:getReadyPosition];

    Texture2D *goTexture = [[ResourceLoader instance] getTextureWithName:@"go"];
    CGPoint goPosition = CGPointMake((SCREEN_WIDTH - goTexture.contentSize.width) / 2, 
                                     (SCREEN_HEIGHT - goTexture.contentSize.height) / 2);
    go_ = [[SimpleItem alloc] initWithTexture:goTexture
                                     position:goPosition];
    
    Texture2D *rematchButtonTexture =
        [[ResourceLoader instance] getTextureWithName:@"rematch_button"];
    Texture2D *rematchButtonPressedTexture =
        [[ResourceLoader instance] getTextureWithName:@"rematch_button_pressed"];
    CGPoint rematchButtonPos =
        CGPointMake((SCREEN_WIDTH - rematchButtonTexture.contentSize.width) / 2, 441);
    rematchButton_ = [[Button alloc] initWithNormalTexture:rematchButtonTexture
                                            pressedTexture:rematchButtonPressedTexture
                                                  position:rematchButtonPos];
    rematchButton_.delegate = self;
    rematchButton_.selector = @selector(rematchPressed);

    Texture2D *menuButtonTexture = [[ResourceLoader instance] getTextureWithName:@"menu_button"];
    Texture2D *menuButtonPressedTexture =
        [[ResourceLoader instance] getTextureWithName:@"menu_button_pressed"];
    CGPoint menuButtonPos = CGPointMake((SCREEN_WIDTH - menuButtonTexture.contentSize.width) / 2,
                                        546);
    menuButton_ = [[Button alloc] initWithNormalTexture:menuButtonTexture
                                         pressedTexture:menuButtonPressedTexture
                                               position:menuButtonPos];
    menuButton_.delegate = self;
    menuButton_.selector = @selector(menuPressed);

    Texture2D *continueButtonTexture =
        [[ResourceLoader instance] getTextureWithName:@"continue_button"];
    Texture2D *continueButtonPressedTexture =
        [[ResourceLoader instance] getTextureWithName:@"continue_button_pressed"];
    CGPoint continueButtonPos =
        CGPointMake((SCREEN_WIDTH - continueButtonTexture.contentSize.width) / 2, 441);
    continueButton_ = [[Button alloc] initWithNormalTexture:continueButtonTexture
                                             pressedTexture:continueButtonPressedTexture
                                                   position:continueButtonPos];
    continueButton_.delegate = self;
    continueButton_.selector = @selector(continuePressed);
    
    soundSlider_ = [[SoundSlider alloc] initWithPosition:CGPointMake(331, 336)];
    
    Texture2D *menuBackgroundTexture =
        [[ResourceLoader instance] getTextureWithName:@"game_menu_bg"];
    CGPoint menuBackgroundPosition =
        CGPointMake((SCREEN_WIDTH - menuBackgroundTexture.contentSize.width) / 2, 306);
    menuBackground_ = [[SimpleItem alloc] initWithTexture:menuBackgroundTexture
                                                 position:menuBackgroundPosition];
    
    Texture2D *pauseButtonTexture = [[ResourceLoader instance] getTextureWithName:@"pause_button"];
    Texture2D *pauseButtonPressedTexture =
        [[ResourceLoader instance] getTextureWithName:@"pause_button_pressed"];
    
    BOOL isIPhone = (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone);
    
    if (!isIPhone) {
      CGPoint pauseButtonPos1 = CGPointMake(0, 0);
      pauseButton1_ = [[Button alloc] initWithNormalTexture:pauseButtonTexture
                                             pressedTexture:pauseButtonPressedTexture
                                                   position:pauseButtonPos1];

      pauseButton1_.delegate = self;
      pauseButton1_.selector = @selector(pausePressed);
      [self addEntity:pauseButton1_];    
    }
    
    CGPoint pauseButtonPos2 =
        CGPointMake(SCREEN_WIDTH - pauseButtonTexture.contentSize.width,
                    SCREEN_HEIGHT - pauseButtonTexture.contentSize.height +
                        (NO ? (27 * 768.0/320.0) : 0));
    pauseButton2_ = [[Button alloc] initWithNormalTexture:pauseButtonTexture
                                           pressedTexture:pauseButtonPressedTexture
                                                 position:pauseButtonPos2];
    pauseButton2_.delegate = self;
    pauseButton2_.selector = @selector(pausePressed);
    [self addEntity:pauseButton2_];
      
    if (isIPhone) {
      if (IS_FREE) {
        player1Wins_ = [[UILabel alloc] initWithFrame:CGRectMake(47, 278 + 26, 150, 35)];
        player1Wins_.textColor = [UIColor whiteColor];
      } else {
        player1Wins_ = [[UILabel alloc] initWithFrame:CGRectMake(5, 448, 150, 35)];
        player1Wins_.textColor = [UIColor grayColor];
      }
    } else {
      player1Wins_ = [[UILabel alloc] initWithFrame:CGRectMake(106, 644, 150, 35)];
      player1Wins_.textColor = [UIColor whiteColor];
    }
    player1Wins_.backgroundColor = [UIColor clearColor];
    player1Wins_.textAlignment = UITextAlignmentLeft;
    if (isIPhone) {
      player1Wins_.font = [UIFont fontWithName:@"Helvetica-Bold" size:20];
    } else {
      player1Wins_.font = [UIFont fontWithName:@"Helvetica-Bold" size:35];
    }
    
    if (isIPhone) {
      if (IS_FREE) {
        player2Wins_ = [[UILabel alloc] initWithFrame:CGRectMake(47, 155 + 26, 150, 35)];
        player2Wins_.textColor = [UIColor whiteColor];
      } else {
        player2Wins_ = [[UILabel alloc] initWithFrame:CGRectMake(5, -5, 150, 35)];
        player2Wins_.textColor = [UIColor grayColor];
      }
    } else {
      player2Wins_ = [[UILabel alloc] initWithFrame:CGRectMake(106, 324, 150, 35)];
      player2Wins_.textColor = [UIColor whiteColor];
    }
    player2Wins_.backgroundColor = [UIColor clearColor];
    if (isIPhone) {
      player2Wins_.font = [UIFont fontWithName:@"Helvetica-Bold" size:20];
    } else {
      player2Wins_.font = [UIFont fontWithName:@"Helvetica-Bold" size:35];
    }
    if (numPlayers_ == 2) {
      player2Wins_.transform = CGAffineTransformMakeRotation(M_PI); 
      player2Wins_.textAlignment = UITextAlignmentRight;
    } else {
      player2Wins_.textAlignment = UITextAlignmentLeft;
    }
    
    if (!IS_FREE && isIPhone) {
      player1Wins_.text = @"0 wins";
      player2Wins_.text = @"0 wins";
      [self.gameEngine addUIView:player1Wins_];
      [self.gameEngine addUIView:player2Wins_];
    }
    
    giveExtraPuckToPlayer_ = PLAYER_1;
    player1WinCount_ = 0;
    player2WinCount_ = 0;
    [self setUpNewGame];
  }
  
  return self;
}

- (void)dealloc {
  [rink_ release];
  [paddle1_ release];
  [paddle2_ release];
  [pucks_ release];
  [roundThings_ release];
  [player1Score_ release];
  [player2Score_ release];
  [win_ release];
  [lose_ release];
  [getReady_ release];
  [go_ release];
  [rematchButton_ release];
  [menuButton_ release];
  [soundSlider_  release];
  [menuBackground_ release];
  [continueButton_ release];
  [pauseButton1_ release];
  [pauseButton2_ release];
  [player1Wins_ release];
  [player2Wins_ release];
  
  [super dealloc];
}

- (void)update {
  if (state_ == PLAY_STATE_PAUSED) {
    return;
  } else if (state_ == PLAY_STATE_GET_READY) {
    getReadyTicksLeft_--;
    if (getReadyTicksLeft_ == SHOW_GET_READY_MESSAGE_TICKS) {
      [self addEntity:getReady_];
      [SoundPlayer playSound:kSoundGetReady];
    } else if (getReadyTicksLeft_ == 0) {
      [self removeEntity:getReady_];
      [self addEntity:go_];
      goTicksLeft_ = SHOW_GO_MESSAGE_TICKS;
      state_ = PLAY_STATE_PLAYING;
      [SoundPlayer playSound:kSoundStart];
    }
    
    return;
  }

  [super update];
  
  if (goTicksLeft_ > 0) {
    goTicksLeft_--;
    if (goTicksLeft_ == 0) {
      [self removeEntity:go_];
    }
  }
  
  [paddle1_ keepInPlayerBounds];
  [paddle2_ keepInPlayerBounds];
  
  for (int i = 0; i < roundThings_.count; i++) {
    RoundThing *thing = [roundThings_ objectAtIndex:i];
    if (!thing.active) {
      continue;
    }
    [rink_ bounceOff:thing];
    for (int j = i + 1; j < roundThings_.count; j++) {
      RoundThing *otherThing = [roundThings_ objectAtIndex:j];;
      if (otherThing.active) {
        [thing bounceOff:otherThing];
      }
    }
    
    [thing applyFriction];
    
    // TODO If you grab item A and push item B into a corner,
    // it only behaves if item A was added to roundsThings_
    // after item B. This is OK for Air Hockey, but should be fixed
    // for other games.
    [rink_ moveInFromEdge:thing];
  }

  for (int i = 0; i < pucks_.count; i++) {
    Puck *puck = [pucks_ objectAtIndex:i];
    if (!puck.active) {
      continue;
    }
    if (puck.y < -puck.radius) {
      puck.active = NO;
      if (player1Score_.texture < WIN_SCORE && state_ == PLAY_STATE_PLAYING) {
        player1Score_.texture++;
      }
      if (player1Score_.texture == WIN_SCORE && state_ == PLAY_STATE_PLAYING) {
        [SoundPlayer playSound:kSoundScoreFinal];  
      } else {
        [SoundPlayer playSound:kSoundScore];
      }
      numPlayer1ScoresLastRound_++;
      numActivePucks_--;
    } else if (puck.y > SCREEN_HEIGHT + puck.radius) {
      puck.active = NO;
      if (player2Score_.texture < WIN_SCORE && state_ == PLAY_STATE_PLAYING) {
        player2Score_.texture++;
      }
      if (player2Score_.texture == WIN_SCORE && state_ == PLAY_STATE_PLAYING) {
        [SoundPlayer playSound:kSoundScoreFinal];  
      } else {
        [SoundPlayer playSound:kSoundScore];
      }
      
      numActivePucks_--;
    }
  }  
  
  switch (state_) {
    case PLAY_STATE_PLAYING: {      
      if (player1Score_.texture == WIN_SCORE) {
        [self finishGameWithWinner:PLAYER_1];
      } else if (player2Score_.texture == WIN_SCORE) {
        [self finishGameWithWinner:PLAYER_2];
      } else if (numActivePucks_ == 0) {
        waitTicksLeft_ = WAIT_TICKS;
        state_ = PLAY_STATE_WAITING_FOR_PUCKS;
      }
      break;
    }
    case PLAY_STATE_WAITING_FOR_PUCKS: {
      if (waitTicksLeft_-- == 0) {
        for (int i = 0; i < numPucks_; i++) {
          Puck *puck = [pucks_ objectAtIndex:i];
          puck.active = YES;
          [puck placeForPlayer:(i < numPlayer1ScoresLastRound_ ? PLAYER_2 : PLAYER_1)
                   roundThings:roundThings_
                        center:(i < numPlayer1ScoresLastRound_ ?
                                (numPlayer1ScoresLastRound_ % 2 == 1) :
                                ((numPucks_ - numPlayer1ScoresLastRound_) % 2 == 1))];
          [puck fadeIn];
        }
        numActivePucks_ = numPucks_;
        numPlayer1ScoresLastRound_ = 0;
        
        state_ = PLAY_STATE_PLAYING;        

        break;
      }
    }
  }
}

- (void)setUpNewGame {
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
  } else {
    [self.gameEngine.adEngine removeAd];
  }

  // Place paddles!
  [paddle1_ setInitialPositionForPlayer:PLAYER_1];
  [paddle2_ setInitialPositionForPlayer:PLAYER_2];
  
  // Place pucks!
  // First move them all out of the way. That way we can lay them out properly.
  // ([Puck placeForPlayer] avoids hitting other RoundThings objects.)
  for (int i = 0; i < numPucks_; i++) {
    Puck *puck = [pucks_ objectAtIndex:i];
    puck.x = 0;
    puck.y = 0;
  }
  for (int i = 0; i < numPucks_; i++) {
    Puck *puck = [pucks_ objectAtIndex:i];
    puck.active = YES;
    int playerId = (i % 2 == 0) ? giveExtraPuckToPlayer_ : 1 - giveExtraPuckToPlayer_;
    BOOL center = !((playerId == giveExtraPuckToPlayer_ &&
                      (numPucks_ == 3 || numPucks_ == 4 || numPucks_ == 7)) ||
                      (playerId == 1 - giveExtraPuckToPlayer_ &&
                      (numPucks_ == 4 || numPucks_ == 5)));
    [puck placeForPlayer:playerId roundThings:roundThings_ center:center];
  }
  
  player1Score_.texture = 0;
  player2Score_.texture = 0;
  [self removeEntity:menuBackground_];
  [self removeEntity:soundSlider_];
  [self removeEntity:rematchButton_];
  [self removeEntity:menuButton_];
  [self removeEntity:win_];
  [self removeEntity:lose_];
  
  numActivePucks_ = numPucks_;
  numPlayer1ScoresLastRound_ = 0;
  
  state_ = PLAY_STATE_GET_READY;
  getReadyTicksLeft_ = GET_READY_TICKS_TOTAL;
}

- (void)finishGameWithWinner:(int)playerId {
  state_ = PLAY_STATE_FINISHED;
  
  double loseX = (SCREEN_WIDTH - lose_.size.width)/2;
  double winX =  (SCREEN_WIDTH - win_.size.width)/2;
  double topY = 70;
  double bottomY = SCREEN_HEIGHT - topY - lose_.size.height;
  switch (playerId) {
    case PLAYER_1: {
      player1WinCount_++;

      win_.position = CGPointMake(winX, bottomY);
      win_.angle = 0;
      [self addEntity:win_];
      
      if (numPlayers_ == 2) {
        lose_.position = CGPointMake(loseX, topY);
        lose_.angle = 180;
        [self addEntity:lose_];
      }
      
      giveExtraPuckToPlayer_ = PLAYER_2;
      
      break;
    }
    case PLAYER_2: {
      player2WinCount_++;
      
      if (numPlayers_ == 2) {
        win_.position = CGPointMake(winX, topY);
        win_.angle = 180;      
        [self addEntity:win_];
      }
      
      lose_.position = CGPointMake(loseX, bottomY);
      lose_.angle = 0;
      [self addEntity:lose_];
      
      giveExtraPuckToPlayer_ = PLAYER_1;
      
      break;
    }
  }

  player1Wins_.text =
      [NSString stringWithFormat:@"%d win%@", player1WinCount_, player1WinCount_ == 1 ? @"" : @"s"];
  player2Wins_.text =
      [NSString stringWithFormat:@"%d win%@", player2WinCount_, player2WinCount_ == 1 ? @"" : @"s"];
  if (IS_FREE || UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
    [self.gameEngine addUIView:player1Wins_];
    [self.gameEngine addUIView:player2Wins_];
  }
  
  [self addEntity:menuBackground_];
  [self addEntity:soundSlider_];
  [self addEntity:rematchButton_];
  [self addEntity:menuButton_];
  
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
    [self.gameEngine.adEngine addAdAtPoint:CGPointMake((SCREEN_WIDTH - 320) / 2, 385)];
  } else {
    [self.gameEngine.adEngine addAdAtPoint:CGPointMake(0, 0)];
  }
}

- (void)rematchPressed {
  [FlurryAnalytics logEvent:@"REMATCH"];
  if (IS_FREE || UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
    [player1Wins_ removeFromSuperview];
    [player2Wins_ removeFromSuperview];
  }
  [self setUpNewGame];
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
    [self.gameEngine.adEngine removeAd];
  }  
}


- (void)menuPressed {
  [player1Wins_ removeFromSuperview];
  [player2Wins_ removeFromSuperview];
  [self.gameEngine replaceTopState:[[[MainMenuState alloc] init] autorelease]];
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
    [self.gameEngine.adEngine removeAd];
  }
}

- (void)continuePressed {
  state_ = prePauseState_;
  [self removeEntity:menuBackground_];
  [self removeEntity:soundSlider_];
  [self removeEntity:menuButton_];
  [self removeEntity:continueButton_];
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
    [self.gameEngine.adEngine removeAd];
  }
}

- (void)pausePressed {
  if (state_ != PLAY_STATE_FINISHED && state_ != PLAY_STATE_PAUSED) {
    prePauseState_ = state_;
    state_ = PLAY_STATE_PAUSED;
    [self addEntity:menuBackground_];
    [self addEntity:soundSlider_];
    [self addEntity:menuButton_];
    [self addEntity:continueButton_];
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
      [self.gameEngine.adEngine addAdAtPoint:CGPointMake((SCREEN_WIDTH - 320)/2, 385)];
    }
  }
}

- (void)touchesBegan:(Touch *[])touches numTouches:(int)numTouches {
  // When paused, only allow touches on the menu and continue buttons.
  if (state_ == PLAY_STATE_PAUSED) {
    [menuButton_ touchesBegan:touches numTouches:numTouches];
    [continueButton_ touchesBegan:touches numTouches:numTouches];
    [soundSlider_ touchesBegan:touches numTouches:numTouches];
  } else if (state_ == PLAY_STATE_GET_READY) {
    [pauseButton1_ touchesBegan:touches numTouches:numTouches];
    [pauseButton2_ touchesBegan:touches numTouches:numTouches];
  } else {
    [super touchesBegan:touches numTouches:numTouches];
  }
}

- (void)touchesMoved:(Touch *[])touches numTouches:(int)numTouches {
  // When paused, only allow touches on the menu and continue buttons.
  if (state_ == PLAY_STATE_PAUSED) {
    [soundSlider_ touchesMoved:touches numTouches:numTouches];
  } else if (state_ == PLAY_STATE_GET_READY) {
  } else {
    [super touchesMoved:touches numTouches:numTouches];
  }
}

- (void)touchesEnded:(Touch *[])touches numTouches:(int)numTouches {
  // When paused, only allow touches on the menu and continue buttons.
  if (state_ == PLAY_STATE_PAUSED) {
    [menuButton_ touchesEnded:touches numTouches:numTouches];
    [continueButton_ touchesEnded:touches numTouches:numTouches];    
    [soundSlider_ touchesEnded:touches numTouches:numTouches];
  } else if (state_ == PLAY_STATE_GET_READY) {
    [pauseButton1_ touchesEnded:touches numTouches:numTouches];
    [pauseButton2_ touchesEnded:touches numTouches:numTouches];    
  } else {
    [super touchesEnded:touches numTouches:numTouches];
  }
}

- (void)clearTouches {
  [super clearTouches];
  [self pausePressed];
}

@end
