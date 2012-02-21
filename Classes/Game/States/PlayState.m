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
#import "FlurryAnalytics.h"

@implementation PlayState

- (id) initWithNumPlayers:(int)numPlayers numPucks:(int)numPucks difficulty:(ComputerAI)difficulty paddleSize:(PaddleSize)paddleSize {
  [super init];
  
  numPlayers_ = numPlayers;
  roundThings_ = [[NSMutableArray alloc] init];
  pucks_ = [[NSMutableArray alloc] init];
  
  rink_ = [[Rink alloc] init];
  [self addEntity:rink_];
  
  NSMutableArray* scoreTextures = [NSMutableArray arrayWithCapacity:WIN_SCORE + 1];
  for (int i = 0; i <= WIN_SCORE; i++) {
    Texture2D* texture = [[ResourceLoader instance] getTextureWithName:[NSString stringWithFormat:@"%d_points", i]];
    [scoreTextures addObject:texture];
  }
  player_1Score = [[SimpleItem alloc] initWithTextures:scoreTextures position:CGPointMake(662, 526)];
  player_2Score = [[SimpleItem alloc] initWithTextures:scoreTextures position:CGPointMake(662, 386)];
  [self addEntity:player_1Score];
  [self addEntity:player_2Score];
  
  numPucks_ = numPucks;
  numActivePucks_ = numPucks_;
  for (int i = 0; i < numPucks_; i++) {
    Puck* puck = [[[Puck alloc] init] autorelease];
    
    [self addEntity:puck];
    [pucks_ addObject:puck];
    [roundThings_ addObject:puck];
  }
  
  paddle_1 = [[Paddle alloc] initWithPlayer:PLAYER_1
                     size:paddleSize
               playerControlled:YES
                    aiLevel:0];
  [self addEntity:paddle_1];
  [roundThings_ addObject:paddle_1];

  paddle_2 = [[Paddle alloc] initWithPlayer:PLAYER_2
                     size:paddleSize
               playerControlled:numPlayers == 2
                    aiLevel:difficulty];
  [self addEntity:paddle_2];
  [roundThings_ addObject:paddle_2];
  
  paddle_1.pucks = pucks_;
  paddle_1.otherPaddle = paddle_2;
  paddle_2.pucks = pucks_;
  paddle_2.otherPaddle = paddle_1;
  
  Post* post1 = [[[Post alloc] initWithX:GOAL_LEFT_X y:RINK_TOP_Y] autorelease];
  [self addEntity:post1];
  [roundThings_ addObject:post1];

  Post* post2 = [[[Post alloc] initWithX:GOAL_LEFT_X y:RINK_BOTTOM_Y + 1] autorelease];
  [self addEntity:post2];
  [roundThings_ addObject:post2];

  Post* post3 = [[[Post alloc] initWithX:GOAL_RIGHT_X + 1 y:RINK_TOP_Y] autorelease];
  [self addEntity:post3];
  [roundThings_ addObject:post3];
  
  Post* post4 = [[[Post alloc] initWithX:GOAL_RIGHT_X + 1 y:RINK_BOTTOM_Y + 1] autorelease];
  [self addEntity:post4];
  [roundThings_ addObject:post4];
  
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
  win_ = [[SimpleItem alloc] initWithTexture:winTexture
                    position:CGPointMake(0, 0)];

  Texture2D* loseTexture = [[ResourceLoader instance] getTextureWithName:@"lose"];
  lose_ = [[SimpleItem alloc] initWithTexture:loseTexture
                     position:CGPointMake(0, 0)];

  Texture2D* getReadyTexture = [[ResourceLoader instance] getTextureWithName:@"get_ready"];
  getReady_ = [[SimpleItem alloc] initWithTexture:getReadyTexture
                       position:CGPointMake((SCREEN_WIDTH - getReadyTexture.contentSize.width) / 2, 
                                (SCREEN_HEIGHT - getReadyTexture.contentSize.height) / 2)];

  Texture2D* goTexture = [[ResourceLoader instance] getTextureWithName:@"go"];
  go_ = [[SimpleItem alloc] initWithTexture:goTexture
                   position:CGPointMake((SCREEN_WIDTH - goTexture.contentSize.width) / 2, 
                              (SCREEN_HEIGHT - goTexture.contentSize.height) / 2)];
  
  Texture2D* rematchButtonTexture = [[ResourceLoader instance] getTextureWithName:@"rematch_button"];
  Texture2D* rematchButtonPressedTexture = [[ResourceLoader instance] getTextureWithName:@"rematch_button_pressed"];
  CGPoint rematchButtonPos = CGPointMake((SCREEN_WIDTH - rematchButtonTexture.contentSize.width) / 2, 441);
  rematchButton_ = [[Button alloc] initWithNormalTexture:rematchButtonTexture
                      pressedTexture:rematchButtonPressedTexture
                          position:rematchButtonPos];
  rematchButton_.delegate = self;
  rematchButton_.selector = @selector(rematchPressed);

  Texture2D* menuButtonTexture = [[ResourceLoader instance] getTextureWithName:@"menu_button"];
  Texture2D* menuButtonPressedTexture = [[ResourceLoader instance] getTextureWithName:@"menu_button_pressed"];
  CGPoint menuButtonPos = CGPointMake((SCREEN_WIDTH - menuButtonTexture.contentSize.width) / 2, 546);
  menuButton_ = [[Button alloc] initWithNormalTexture:menuButtonTexture
                     pressedTexture:menuButtonPressedTexture
                         position:menuButtonPos];
  menuButton_.delegate = self;
  menuButton_.selector = @selector(menuPressed);

  Texture2D* continueButtonTexture = [[ResourceLoader instance] getTextureWithName:@"continue_button"];
  Texture2D* continueButtonPressedTexture = [[ResourceLoader instance] getTextureWithName:@"continue_button_pressed"];
  CGPoint continueButtonPos = CGPointMake((SCREEN_WIDTH - continueButtonTexture.contentSize.width) / 2, 441);
  continueButton_ = [[Button alloc] initWithNormalTexture:continueButtonTexture
                       pressedTexture:continueButtonPressedTexture
                           position:continueButtonPos];
  continueButton_.delegate = self;
  continueButton_.selector = @selector(continuePressed);
  
  soundSlider_ = [[SoundSlider alloc] initWithPosition:CGPointMake(331, 336)];
  
  Texture2D* menuBackgroundTexture = [[ResourceLoader instance] getTextureWithName:@"game_menu_bg"];
  menuBackground_ = [[SimpleItem alloc] initWithTexture:menuBackgroundTexture
                         position:CGPointMake((SCREEN_WIDTH - menuBackgroundTexture.contentSize.width) / 2, 306)];
  
  Texture2D* pauseButtonTexture = [[ResourceLoader instance] getTextureWithName:@"pause_button"];
  Texture2D* pauseButtonPressedTexture = [[ResourceLoader instance] getTextureWithName:@"pause_button_pressed"];
  
  BOOL isIPhone = (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone);
  
  if (!isIPhone) {
    CGPoint pauseButtonPos1 = CGPointMake(0, 0);
    pauseButton_1 = [[Button alloc] initWithNormalTexture:pauseButtonTexture
                         pressedTexture:pauseButtonPressedTexture
                           position:pauseButtonPos1];

    pauseButton_1.delegate = self;
    pauseButton_1.selector = @selector(pausePressed);
    [self addEntity:pauseButton_1];    
  }
  
  CGPoint pauseButtonPos2 = CGPointMake(SCREEN_WIDTH - pauseButtonTexture.contentSize.width,
                      SCREEN_HEIGHT - pauseButtonTexture.contentSize.height + (NO ? (27 * 768.0/320.0) : 0));
  pauseButton_2 = [[Button alloc] initWithNormalTexture:pauseButtonTexture
                       pressedTexture:pauseButtonPressedTexture
                         position:pauseButtonPos2];
  pauseButton_2.delegate = self;
  pauseButton_2.selector = @selector(pausePressed);
  [self addEntity:pauseButton_2];
    
  if (isIPhone) {
    if (IS_FREE) {
      player_1Wins = [[UILabel alloc] initWithFrame:CGRectMake(47, 278 + 26, 150, 35)];
      player_1Wins.textColor = [UIColor whiteColor];
    } else {
      player_1Wins = [[UILabel alloc] initWithFrame:CGRectMake(5, 448, 150, 35)];
      player_1Wins.textColor = [UIColor grayColor];
    }
  } else {
    player_1Wins = [[UILabel alloc] initWithFrame:CGRectMake(106, 644, 150, 35)];
    player_1Wins.textColor = [UIColor whiteColor];
  }
  player_1Wins.backgroundColor = [UIColor clearColor];
  player_1Wins.textAlignment = UITextAlignmentLeft;
  if (isIPhone) {
    player_1Wins.font = [UIFont fontWithName:@"Helvetica-Bold" size:20];
  } else {
    player_1Wins.font = [UIFont fontWithName:@"Helvetica-Bold" size:35];
  }
  
  if (isIPhone) {
    if (IS_FREE) {
      player_2Wins = [[UILabel alloc] initWithFrame:CGRectMake(47, 155 + 26, 150, 35)];
      player_2Wins.textColor = [UIColor whiteColor];
    } else {
      player_2Wins = [[UILabel alloc] initWithFrame:CGRectMake(5, -5, 150, 35)];
      player_2Wins.textColor = [UIColor grayColor];
    }
  } else {
    player_2Wins = [[UILabel alloc] initWithFrame:CGRectMake(106, 324, 150, 35)];
    player_2Wins.textColor = [UIColor whiteColor];
  }
  player_2Wins.backgroundColor = [UIColor clearColor];
  if (isIPhone) {
    player_2Wins.font = [UIFont fontWithName:@"Helvetica-Bold" size:20];
  } else {
    player_2Wins.font = [UIFont fontWithName:@"Helvetica-Bold" size:35];
  }
  if (numPlayers_ == 2) {
    player_2Wins.transform = CGAffineTransformMakeRotation(M_PI); 
    player_2Wins.textAlignment = UITextAlignmentRight;
  } else {
    player_2Wins.textAlignment = UITextAlignmentLeft;
  }
  
  if (!IS_FREE && isIPhone) {
    player_1Wins.text = @"0 wins";
    player_2Wins.text = @"0 wins";
    [EAGLView addUIView:player_1Wins];    
    [EAGLView addUIView:player_2Wins];
  }
  
  giveExtraPuckToPlayer_ = PLAYER_1;
  player_1WinCount = 0;
  player_2WinCount = 0;
  [self setUpNewGame];
  
  return self;
}

- (void) dealloc {
  [rink_           release];
  [paddle_1        release];
  [paddle_2        release];
  [pucks_          release];
  [roundThings_    release];
  [player_1Score   release];
  [player_2Score   release];
  [win_            release];
  [lose_           release];
  [getReady_       release];
  [go_             release];
  [rematchButton_  release];
  [menuButton_     release];
  [soundSlider_    release];
  [menuBackground_ release];
  [continueButton_ release];
  [pauseButton_1   release];
  [pauseButton_2   release];
  [player_1Wins    release];
  [player_2Wins    release];
  
  [super dealloc];
}

- (void) update {
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
  
  [paddle_1 keepInPlayerBounds];
  [paddle_2 keepInPlayerBounds];
  
  for (int i = 0; i < roundThings_.count; i++) {
    RoundThing* thing = [roundThings_ objectAtIndex:i];
    if (!thing.active) {
      continue;
    }
    [rink_ bounceOff:thing];
    for (int j = i + 1; j < roundThings_.count; j++) {
      RoundThing* otherThing = [roundThings_ objectAtIndex:j];;
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
    Puck* puck = [pucks_ objectAtIndex:i];
    if (!puck.active) {
      continue;
    }
    if (puck.y < -puck.radius) {
      puck.active = NO;
      if (player_1Score.texture < WIN_SCORE && state_ == PLAY_STATE_PLAYING) {
        player_1Score.texture++;
      }
      if (player_1Score.texture == WIN_SCORE && state_ == PLAY_STATE_PLAYING) {
        [SoundPlayer playSound:kSoundScoreFinal];  
      } else {
        [SoundPlayer playSound:kSoundScore];
      }
      numPlayer_1ScoresLastRound++;
      numActivePucks_--;
    } else if (puck.y > SCREEN_HEIGHT + puck.radius) {
      puck.active = NO;
      if (player_2Score.texture < WIN_SCORE && state_ == PLAY_STATE_PLAYING) {
        player_2Score.texture++;
      }
      if (player_2Score.texture == WIN_SCORE && state_ == PLAY_STATE_PLAYING) {
        [SoundPlayer playSound:kSoundScoreFinal];  
      } else {
        [SoundPlayer playSound:kSoundScore];
      }
      
      numActivePucks_--;
    }
  }  
  
  switch (state_) {
    case PLAY_STATE_PLAYING: {      
      if (player_1Score.texture == WIN_SCORE) {
        [self finishGameWithWinner:PLAYER_1];
      } else if (player_2Score.texture == WIN_SCORE) {
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
          Puck* puck = [pucks_ objectAtIndex:i];
          puck.active = YES;
          [puck placeForPlayer:(i < numPlayer_1ScoresLastRound ? PLAYER_2 : PLAYER_1)
               roundThings:roundThings_
                  center:(i < numPlayer_1ScoresLastRound ? (numPlayer_1ScoresLastRound % 2 == 1)
                      : ((numPucks_ - numPlayer_1ScoresLastRound) % 2 == 1))];
          [puck fadeIn];
        }
        numActivePucks_ = numPucks_;
        numPlayer_1ScoresLastRound = 0;
        
        state_ = PLAY_STATE_PLAYING;        

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
  [paddle_1 setInitialPositionForPlayer:PLAYER_1];
  [paddle_2 setInitialPositionForPlayer:PLAYER_2];
  
  // Place pucks!
  // First move them all out of the way. That way we can lay them out properly.
  // ([Puck placeForPlayer] avoids hitting other RoundThings objects.)
  for (int i = 0; i < numPucks_; i++) {
    Puck* puck = [pucks_ objectAtIndex:i];
    puck.x = 0;
    puck.y = 0;
  }
  for (int i = 0; i < numPucks_; i++) {
    Puck* puck = [pucks_ objectAtIndex:i];
    puck.active = YES;
    int playerId = (i % 2 == 0) ? giveExtraPuckToPlayer_ : 1 - giveExtraPuckToPlayer_;
    BOOL center = !((playerId == giveExtraPuckToPlayer_ && (numPucks_ == 3 || numPucks_ == 4 || numPucks_ == 7)) ||
            (playerId == 1 - giveExtraPuckToPlayer_ && (numPucks_ == 4 || numPucks_ == 5)));
    [puck placeForPlayer:playerId roundThings:roundThings_ center:center];
  }
  
  player_1Score.texture = 0;
  player_2Score.texture = 0;
  [self removeEntity:menuBackground_];
  [self removeEntity:soundSlider_];
  [self removeEntity:rematchButton_];
  [self removeEntity:menuButton_];
  [self removeEntity:win_];
  [self removeEntity:lose_];
  
  numActivePucks_ = numPucks_;
  numPlayer_1ScoresLastRound = 0;
  
  state_ = PLAY_STATE_GET_READY;
  getReadyTicksLeft_ = GET_READY_TICKS_TOTAL;
}

- (void) finishGameWithWinner:(int)playerId {
  state_ = PLAY_STATE_FINISHED;
  
  double loseX = (SCREEN_WIDTH - lose_.size.width)/2;
  double winX =  (SCREEN_WIDTH - win_.size.width)/2;
  double topY = 70;
  double bottomY = SCREEN_HEIGHT - topY - lose_.size.height;
  switch (playerId) {
    case PLAYER_1: {
      player_1WinCount++;

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
      player_2WinCount++;
      
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

  player_1Wins.text = [NSString stringWithFormat:@"%d win%@", player_1WinCount, player_1WinCount == 1 ? @"" : @"s"];
  player_2Wins.text = [NSString stringWithFormat:@"%d win%@", player_2WinCount, player_2WinCount == 1 ? @"" : @"s"];
  if (IS_FREE || UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
    [EAGLView addUIView:player_1Wins];
    [EAGLView addUIView:player_2Wins];
  }
  
  [self addEntity:menuBackground_];
  [self addEntity:soundSlider_];
  [self addEntity:rematchButton_];
  [self addEntity:menuButton_];
  
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
    [EAGLView addAdAtPoint:CGPointMake((SCREEN_WIDTH - 320)/2, 385)];
  } else {
    [EAGLView addAdAtPoint:CGPointMake(0, 0)];
  }
}

- (void) rematchPressed {
  [FlurryAnalytics logEvent:@"REMATCH"];
  if (IS_FREE || UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
    [EAGLView removeUIView:player_1Wins];
    [EAGLView removeUIView:player_2Wins];
  }
  [self setUpNewGame];
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
    [EAGLView removeAd];
  }  
}


- (void) menuPressed {
  [EAGLView removeUIView:player_1Wins];
  [EAGLView removeUIView:player_2Wins];
  [[GameEngine instance] replaceTopState:[[[MainMenuState alloc] init] autorelease]];
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
    [EAGLView removeAd];
  }
}

- (void) continuePressed {
  state_ = prePauseState_;
  [self removeEntity:menuBackground_];
  [self removeEntity:soundSlider_];
  [self removeEntity:menuButton_];
  [self removeEntity:continueButton_];
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
    [EAGLView removeAd];
  }
}

- (void) pausePressed {
  if (state_ != PLAY_STATE_FINISHED && state_ != PLAY_STATE_PAUSED) {
    prePauseState_ = state_;
    state_ = PLAY_STATE_PAUSED;
    [self addEntity:menuBackground_];
    [self addEntity:soundSlider_];
    [self addEntity:menuButton_];
    [self addEntity:continueButton_];
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
      [EAGLView addAdAtPoint:CGPointMake((SCREEN_WIDTH - 320)/2, 385)];
    }
  }
}

- (void) touchesBegan:(Touch*[])touches numTouches:(int)numTouches {
  // When paused, only allow touches on the menu and continue buttons.
  if (state_ == PLAY_STATE_PAUSED) {
    [menuButton_ touchesBegan:touches numTouches:numTouches];
    [continueButton_ touchesBegan:touches numTouches:numTouches];
    [soundSlider_ touchesBegan:touches numTouches:numTouches];
  } else if (state_ == PLAY_STATE_GET_READY) {
    [pauseButton_1 touchesBegan:touches numTouches:numTouches];
    [pauseButton_2 touchesBegan:touches numTouches:numTouches];
  } else {
    [super touchesBegan:touches numTouches:numTouches];
  }
}

- (void) touchesMoved:(Touch*[])touches numTouches:(int)numTouches {
  // When paused, only allow touches on the menu and continue buttons.
  if (state_ == PLAY_STATE_PAUSED) {
    [soundSlider_ touchesMoved:touches numTouches:numTouches];
  } else if (state_ == PLAY_STATE_GET_READY) {
  } else {
    [super touchesMoved:touches numTouches:numTouches];
  }
}

- (void) touchesEnded:(Touch*[])touches numTouches:(int)numTouches {
  // When paused, only allow touches on the menu and continue buttons.
  if (state_ == PLAY_STATE_PAUSED) {
    [menuButton_ touchesEnded:touches numTouches:numTouches];
    [continueButton_ touchesEnded:touches numTouches:numTouches];    
    [soundSlider_ touchesEnded:touches numTouches:numTouches];
  } else if (state_ == PLAY_STATE_GET_READY) {
    [pauseButton_1 touchesEnded:touches numTouches:numTouches];
    [pauseButton_2 touchesEnded:touches numTouches:numTouches];    
  } else {
    [super touchesEnded:touches numTouches:numTouches];
  }
}

- (void) clearTouches {
  [super clearTouches];
  [self pausePressed];
}

@end
