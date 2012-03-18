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
    
    rink_ = new Rink();
    [self addEntity:rink_];
    
    NSMutableArray *scoreTextures = [NSMutableArray arrayWithCapacity:WIN_SCORE + 1];
    for (int i = 0; i <= WIN_SCORE; i++) {
      Texture2D *texture = [[ResourceLoader instance]
                            getTextureWithName:[NSString stringWithFormat:@"%d_points", i]];
      [scoreTextures addObject:texture];
    }
    player1Score_ = new SimpleItem(scoreTextures, CGPointMake(662, 526));
    player2Score_ = new SimpleItem(scoreTextures, CGPointMake(662, 386));
    [self addEntity:player1Score_];
    [self addEntity:player2Score_];
    
    numPucks_ = numPucks;
    numActivePucks_ = numPucks_;
    for (int i = 0; i < numPucks_; i++) {
      Puck* puck = new Puck();
      
      [self addEntity:puck];
      pucks_.push_back(puck);
      roundThings_.push_back(puck);
    }
    
    paddle1_ = new Paddle(PLAYER_1, paddleSize, true, caiBad);
    [self addEntity:paddle1_];
    roundThings_.push_back(paddle1_);

    paddle2_ = new Paddle(PLAYER_2, paddleSize, numPlayers == 2, difficulty);
    [self addEntity:paddle2_];
    roundThings_.push_back(paddle2_);
    
    paddle1_->setPucks(pucks_);
    paddle1_->setOtherPaddle(paddle2_);
    paddle2_->setPucks(pucks_);
    paddle2_->setOtherPaddle(paddle1_);
    
    Post *post1 = new Post(GOAL_LEFT_X, RINK_TOP_Y);
    [self addEntity:post1];
    roundThings_.push_back(post1);

    Post *post2 = new Post(GOAL_LEFT_X, RINK_BOTTOM_Y + 1);
    [self addEntity:post2];
    roundThings_.push_back(post2);

    Post *post3 = new Post(GOAL_RIGHT_X + 1, RINK_TOP_Y);
    [self addEntity:post3];
    roundThings_.push_back(post3);
    
    Post *post4 = new Post(GOAL_RIGHT_X + 1, RINK_BOTTOM_Y + 1);
    [self addEntity:post4];
    roundThings_.push_back(post4);
    
    // Add rink left and right pieces.
    Texture2D *leftRinkBorderTexture = [[ResourceLoader instance] getTextureWithName:@"rink_left"];
    SimpleItem *leftRinkBorder = new SimpleItem(leftRinkBorderTexture, CGPointMake(0, 0));
    [self addEntity:leftRinkBorder];
    Texture2D *rightRinkBorderTexture =
        [[ResourceLoader instance] getTextureWithName:@"rink_right"];
    CGPoint leftRinkBorderPos = CGPointMake(SCREEN_WIDTH - rightRinkBorderTexture.contentSize.width,
                                            0);
    SimpleItem *rightRinkBorder = new SimpleItem(rightRinkBorderTexture, leftRinkBorderPos);
    [self addEntity:rightRinkBorder];
    
    Texture2D *winTexture = [[ResourceLoader instance] getTextureWithName:@"win"];
    win_ = new SimpleItem(winTexture, CGPointMake(0, 0));

    Texture2D *loseTexture = [[ResourceLoader instance] getTextureWithName:@"lose"];
    lose_ = new SimpleItem(loseTexture, CGPointMake(0, 0));

    Texture2D *getReadyTexture = [[ResourceLoader instance] getTextureWithName:@"get_ready"];
    CGPoint getReadyPosition =
        CGPointMake((SCREEN_WIDTH - getReadyTexture.contentSize.width) / 2, 
                    (SCREEN_HEIGHT - getReadyTexture.contentSize.height) / 2);
    getReady_ = new SimpleItem(getReadyTexture, getReadyPosition);

    Texture2D *goTexture = [[ResourceLoader instance] getTextureWithName:@"go"];
    CGPoint goPosition = CGPointMake((SCREEN_WIDTH - goTexture.contentSize.width) / 2, 
                                     (SCREEN_HEIGHT - goTexture.contentSize.height) / 2);
    go_ = new SimpleItem(goTexture, goPosition);
    
    Texture2D *rematchButtonTexture =
        [[ResourceLoader instance] getTextureWithName:@"rematch_button"];
    Texture2D *rematchButtonPressedTexture =
        [[ResourceLoader instance] getTextureWithName:@"rematch_button_pressed"];
    CGPoint rematchButtonPos =
        CGPointMake((SCREEN_WIDTH - rematchButtonTexture.contentSize.width) / 2, 441);
    rematchButton_ = new Button(rematchButtonTexture, rematchButtonPressedTexture, rematchButtonPos);
    rematchButton_->setDelegate(self);
    rematchButton_->setSelector(@selector(rematchPressed));

    Texture2D *menuButtonTexture = [[ResourceLoader instance] getTextureWithName:@"menu_button"];
    Texture2D *menuButtonPressedTexture =
        [[ResourceLoader instance] getTextureWithName:@"menu_button_pressed"];
    CGPoint menuButtonPos = CGPointMake((SCREEN_WIDTH - menuButtonTexture.contentSize.width) / 2,
                                        546);
    menuButton_ = new Button(menuButtonTexture, menuButtonPressedTexture, menuButtonPos);
    menuButton_->setDelegate(self);
    menuButton_->setSelector(@selector(menuPressed));

    Texture2D *continueButtonTexture =
        [[ResourceLoader instance] getTextureWithName:@"continue_button"];
    Texture2D *continueButtonPressedTexture =
        [[ResourceLoader instance] getTextureWithName:@"continue_button_pressed"];
    CGPoint continueButtonPos =
        CGPointMake((SCREEN_WIDTH - continueButtonTexture.contentSize.width) / 2, 441);
    continueButton_ = new Button(continueButtonTexture, continueButtonPressedTexture, continueButtonPos);
    continueButton_->setDelegate(self);
    continueButton_->setSelector(@selector(continuePressed));
    
    soundSlider_ = new SoundSlider(CGPointMake(331, 336));
    
    Texture2D *menuBackgroundTexture =
        [[ResourceLoader instance] getTextureWithName:@"game_menu_bg"];
    CGPoint menuBackgroundPosition =
        CGPointMake((SCREEN_WIDTH - menuBackgroundTexture.contentSize.width) / 2, 306);
    menuBackground_ = new SimpleItem(menuBackgroundTexture, menuBackgroundPosition);
    
    Texture2D *pauseButtonTexture = [[ResourceLoader instance] getTextureWithName:@"pause_button"];
    Texture2D *pauseButtonPressedTexture =
        [[ResourceLoader instance] getTextureWithName:@"pause_button_pressed"];
    
    BOOL isIPhone = (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone);
    
    if (!isIPhone) {
      CGPoint pauseButtonPos1 = CGPointMake(0, 0);
      pauseButton1_ = new Button(pauseButtonTexture, pauseButtonPressedTexture, pauseButtonPos1);

      pauseButton1_->setDelegate(self);
      pauseButton1_->setSelector(@selector(pausePressed));
      [self addEntity:pauseButton1_];    
    }
    
    CGPoint pauseButtonPos2 =
        CGPointMake(SCREEN_WIDTH - pauseButtonTexture.contentSize.width,
                    SCREEN_HEIGHT - pauseButtonTexture.contentSize.height +
                        (NO ? (27 * 768.0/320.0) : 0));
    pauseButton2_ = new Button(pauseButtonTexture, pauseButtonPressedTexture, pauseButtonPos2);
    pauseButton2_->setDelegate(self);
    pauseButton2_->setSelector(@selector(pausePressed));
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
  
  paddle1_->keepInPlayerBounds();
  paddle2_->keepInPlayerBounds();
  
  for (int i = 0; i < roundThings_.size(); i++) {
    RoundThing *thing = roundThings_[i];
    if (!thing->isActive()) {
      continue;
    }
    rink_->bounceOff(thing);
    for (int j = i + 1; j < roundThings_.size(); j++) {
      RoundThing *otherThing = roundThings_[j];
      if (otherThing->isActive()) {
        thing->bounceOff(otherThing);
      }
    }
    
    thing->applyFriction();
    
    // TODO If you grab item A and push item B into a corner,
    // it only behaves if item A was added to roundsThings_
    // after item B. This is OK for Air Hockey, but should be fixed
    // for other games.
    rink_->moveInFromEdge(thing);
  }

  for (int i = 0; i < pucks_.size(); i++) {
    Puck *puck = pucks_[i];
    if (!puck->isActive()) {
      continue;
    }
    if (puck->getY() < -puck->getRadius()) {
      puck->setIsActive(false);
      if (player1Score_->getTexture() < WIN_SCORE && state_ == PLAY_STATE_PLAYING) {
        player1Score_->setTexture(player1Score_->getTexture() + 1);
      }
      if (player1Score_->getTexture() == WIN_SCORE && state_ == PLAY_STATE_PLAYING) {
        [SoundPlayer playSound:kSoundScoreFinal];  
      } else {
        [SoundPlayer playSound:kSoundScore];
      }
      numPlayer1ScoresLastRound_++;
      numActivePucks_--;
    } else if (puck->getY() > SCREEN_HEIGHT + puck->getRadius()) {
      puck->setIsActive(false);
      if (player2Score_->getTexture() < WIN_SCORE && state_ == PLAY_STATE_PLAYING) {
        player2Score_->setTexture(player2Score_->getTexture() + 1);
      }
      if (player2Score_->getTexture() == WIN_SCORE && state_ == PLAY_STATE_PLAYING) {
        [SoundPlayer playSound:kSoundScoreFinal];  
      } else {
        [SoundPlayer playSound:kSoundScore];
      }
      
      numActivePucks_--;
    }
  }  
  
  switch (state_) {
    case PLAY_STATE_PLAYING: {      
      if (player1Score_->getTexture() == WIN_SCORE) {
        [self finishGameWithWinner:PLAYER_1];
      } else if (player2Score_->getTexture() == WIN_SCORE) {
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
          Puck *puck = pucks_[i];
          puck->setIsActive(true);
          puck->placeForPlayer(i < numPlayer1ScoresLastRound_ ? PLAYER_2 : PLAYER_1,
                               roundThings_,
                                (i < numPlayer1ScoresLastRound_ ?
                                    (numPlayer1ScoresLastRound_ % 2 == 1) :
                                    ((numPucks_ - numPlayer1ScoresLastRound_) % 2 == 1)));
          puck->fadeIn();
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
  paddle1_->setInitialPositionForPlayer(PLAYER_1);
  paddle2_->setInitialPositionForPlayer(PLAYER_2);
  
  // Place pucks!
  // First move them all out of the way. That way we can lay them out properly.
  // ([Puck placeForPlayer] avoids hitting other RoundThings objects.)
  for (int i = 0; i < numPucks_; i++) {
    Puck *puck = pucks_[i];
    puck->setX(0);
    puck->setY(0);
  }
  for (int i = 0; i < numPucks_; i++) {
    Puck *puck = pucks_[i];
    puck->setIsActive(true);
    int playerId = (i % 2 == 0) ? giveExtraPuckToPlayer_ : 1 - giveExtraPuckToPlayer_;
    BOOL center = !((playerId == giveExtraPuckToPlayer_ &&
                      (numPucks_ == 3 || numPucks_ == 4 || numPucks_ == 7)) ||
                      (playerId == 1 - giveExtraPuckToPlayer_ &&
                      (numPucks_ == 4 || numPucks_ == 5)));
    puck->placeForPlayer(playerId, roundThings_, center);
  }
  
  player1Score_->setTexture(0);
  player2Score_->setTexture(0);
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
  
  double loseX = (SCREEN_WIDTH - lose_->getSize().width)/2;
  double winX =  (SCREEN_WIDTH - win_->getSize().width)/2;
  double topY = 70;
  double bottomY = SCREEN_HEIGHT - topY - lose_->getSize().height;
  switch (playerId) {
    case PLAYER_1: {
      player1WinCount_++;

      win_->setPosition(CGPointMake(winX, bottomY));
      win_->setAngle(0);
      [self addEntity:win_];
      
      if (numPlayers_ == 2) {
        lose_->setPosition(CGPointMake(loseX, topY));
        lose_->setAngle(180);
        [self addEntity:lose_];
      }
      
      giveExtraPuckToPlayer_ = PLAYER_2;
      
      break;
    }
    case PLAYER_2: {
      player2WinCount_++;
      
      if (numPlayers_ == 2) {
        win_->setPosition(CGPointMake(winX, topY));
        win_->setAngle(180);
        [self addEntity:win_];
      }
      
      lose_->setPosition(CGPointMake(loseX, bottomY));
      lose_->setAngle(0);
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
    menuButton_->touchesBegan(touches, numTouches);
    continueButton_->touchesBegan(touches, numTouches);
    soundSlider_->touchesBegan(touches, numTouches);
  } else if (state_ == PLAY_STATE_GET_READY) {
    pauseButton1_->touchesBegan(touches, numTouches);
    pauseButton2_->touchesBegan(touches, numTouches);
  } else {
    [super touchesBegan:touches numTouches:numTouches];
  }
}

- (void)touchesMoved:(Touch *[])touches numTouches:(int)numTouches {
  // When paused, only allow touches on the menu and continue buttons.
  if (state_ == PLAY_STATE_PAUSED) {
    soundSlider_->touchesMoved(touches, numTouches);
  } else if (state_ == PLAY_STATE_GET_READY) {
  } else {
    [super touchesMoved:touches numTouches:numTouches];
  }
}

- (void)touchesEnded:(Touch *[])touches numTouches:(int)numTouches {
  // When paused, only allow touches on the menu and continue buttons.
  if (state_ == PLAY_STATE_PAUSED) {
    menuButton_->touchesEnded(touches, numTouches);
    continueButton_->touchesEnded(touches, numTouches);
    soundSlider_->touchesEnded(touches, numTouches);
  } else if (state_ == PLAY_STATE_GET_READY) {
    pauseButton1_->touchesEnded(touches, numTouches);
    pauseButton2_->touchesEnded(touches, numTouches);    
  } else {
    [super touchesEnded:touches numTouches:numTouches];
  }
}

- (void)clearTouches {
  [super clearTouches];
  [self pausePressed];
}

@end
