//
//  PlayState.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "PlayState.h"

#import "Post.h"
#import "game_engine.h"
#import "MainMenuState.h"
#import "ResourceLoader.h"

PlayState::PlayState(GameEngine &gameEngine, int numPlayers, int numPucks, ComputerAI difficulty,
                     PaddleSize paddleSize) : EngineState(gameEngine) {
  numPlayers_ = numPlayers;
  
  rink_ = new Rink();
  AddEntity(*rink_);
  
  vector<Texture2D> scoreTextures;
  for (int i = 0; i <= WIN_SCORE; i++) {
    char pointsstr[15];
    sprintf(pointsstr, "%d_points", i);
    Texture2D texture = ResourceLoader::instance()->getTextureWithName(pointsstr);
    scoreTextures.push_back(texture);
  }
  player1Score_ = new SimpleItem(scoreTextures, SGPointMake(662, 526));
  player2Score_ = new SimpleItem(scoreTextures, SGPointMake(662, 386));
  AddEntity(*player1Score_);
  AddEntity(*player2Score_);
  
  numPucks_ = numPucks;
  numActivePucks_ = numPucks_;
  for (int i = 0; i < numPucks_; i++) {
    Puck* puck = new Puck();
    
    AddEntity(*puck);
    pucks_.push_back(puck);
    roundThings_.push_back(puck);
  }
  
  paddle1_ = new Paddle(PLAYER_1, paddleSize, true, caiBad);
  AddEntity(*paddle1_);
  roundThings_.push_back(paddle1_);

  paddle2_ = new Paddle(PLAYER_2, paddleSize, numPlayers == 2, difficulty);
  AddEntity(*paddle2_);
  roundThings_.push_back(paddle2_);
  
  paddle1_->setPucks(pucks_);
  paddle1_->setOtherPaddle(paddle2_);
  paddle2_->setPucks(pucks_);
  paddle2_->setOtherPaddle(paddle1_);
  
  Post *post1 = new Post(GOAL_LEFT_X, RINK_TOP_Y);
  AddEntity(*post1);
  roundThings_.push_back(post1);

  Post *post2 = new Post(GOAL_LEFT_X, RINK_BOTTOM_Y + 1);
  AddEntity(*post2);
  roundThings_.push_back(post2);

  Post *post3 = new Post(GOAL_RIGHT_X + 1, RINK_TOP_Y);
  AddEntity(*post3);
  roundThings_.push_back(post3);
  
  Post *post4 = new Post(GOAL_RIGHT_X + 1, RINK_BOTTOM_Y + 1);
  AddEntity(*post4);
  roundThings_.push_back(post4);
  
  // Add rink left and right pieces.
  Texture2D leftRinkBorderTexture = ResourceLoader::instance()->getTextureWithName("rink_left");
  SimpleItem *leftRinkBorder = new SimpleItem(leftRinkBorderTexture, SGPointMake(0, 0));
  AddEntity(*leftRinkBorder);
  Texture2D rightRinkBorderTexture = ResourceLoader::instance()->getTextureWithName("rink_right");
  SGPoint leftRinkBorderPos = SGPointMake(SCREEN_WIDTH - rightRinkBorderTexture.contentSize().width,
                                          0);
  SimpleItem *rightRinkBorder = new SimpleItem(rightRinkBorderTexture, leftRinkBorderPos);
  AddEntity(*rightRinkBorder);
  
  Texture2D winTexture = ResourceLoader::instance()->getTextureWithName("win");
  win_ = new SimpleItem(winTexture, SGPointMake(0, 0));

  Texture2D loseTexture = ResourceLoader::instance()->getTextureWithName("lose");
  lose_ = new SimpleItem(loseTexture, SGPointMake(0, 0));

  Texture2D getReadyTexture = ResourceLoader::instance()->getTextureWithName("get_ready");
  SGPoint getReadyPosition =
      SGPointMake((SCREEN_WIDTH - getReadyTexture.contentSize().width) / 2, 
                  (SCREEN_HEIGHT - getReadyTexture.contentSize().height) / 2);
  getReady_ = new SimpleItem(getReadyTexture, getReadyPosition);

  Texture2D goTexture = ResourceLoader::instance()->getTextureWithName("go");
  SGPoint goPosition = SGPointMake((SCREEN_WIDTH - goTexture.contentSize().width) / 2, 
                                   (SCREEN_HEIGHT - goTexture.contentSize().height) / 2);
  go_ = new SimpleItem(goTexture, goPosition);
  
  Texture2D rematchButtonTexture =
      ResourceLoader::instance()->getTextureWithName("rematch_button");
  Texture2D rematchButtonPressedTexture =
      ResourceLoader::instance()->getTextureWithName("rematch_button_pressed");
  SGPoint rematchButtonPos =
      SGPointMake((SCREEN_WIDTH - rematchButtonTexture.contentSize().width) / 2, 441);
  rematchButton_ = new Button(rematchButtonTexture, rematchButtonPressedTexture, rematchButtonPos);
// TODO
//  rematchButton_->set_delegate(self);
//  rematchButton_->setSelector(@selector(rematchPressed));

  Texture2D menuButtonTexture = ResourceLoader::instance()->getTextureWithName("menu_button");
  Texture2D menuButtonPressedTexture =
      ResourceLoader::instance()->getTextureWithName("menu_button_pressed");
  SGPoint menuButtonPos = SGPointMake((SCREEN_WIDTH - menuButtonTexture.contentSize().width) / 2,
                                      546);
  menuButton_ = new Button(menuButtonTexture, menuButtonPressedTexture, menuButtonPos);
// TODO
//  menuButton_->set_delegate(self);
//  menuButton_->setSelector(@selector(menuPressed));

  Texture2D continueButtonTexture =
      ResourceLoader::instance()->getTextureWithName("continue_button");
  Texture2D continueButtonPressedTexture =
      ResourceLoader::instance()->getTextureWithName("continue_button_pressed");
  SGPoint continueButtonPos =
      SGPointMake((SCREEN_WIDTH - continueButtonTexture.contentSize().width) / 2, 441);
  continueButton_ = new Button(continueButtonTexture, continueButtonPressedTexture, continueButtonPos);
// TODO
//  continueButton_->set_delegate(self);
//  continueButton_->setSelector(@selector(continuePressed));
  
  soundSlider_ = new SoundSlider(SGPointMake(331, 336));
  
  Texture2D menuBackgroundTexture =
      ResourceLoader::instance()->getTextureWithName("game_menu_bg");
  SGPoint menuBackgroundPosition =
      SGPointMake((SCREEN_WIDTH - menuBackgroundTexture.contentSize().width) / 2, 306);
  menuBackground_ = new SimpleItem(menuBackgroundTexture, menuBackgroundPosition);
  
  Texture2D pauseButtonTexture = ResourceLoader::instance()->getTextureWithName("pause_button");
  Texture2D pauseButtonPressedTexture =
      ResourceLoader::instance()->getTextureWithName("pause_button_pressed");
  
  bool isIPhone = true;  // (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone);
  
  if (!isIPhone) {
    SGPoint pauseButtonPos1 = SGPointMake(0, 0);
    pauseButton1_ = new Button(pauseButtonTexture, pauseButtonPressedTexture, pauseButtonPos1);
// TODO
//    pauseButton1_->set_delegate(self);
//    pauseButton1_->setSelector(@selector(pausePressed));
    AddEntity(*pauseButton1_);
  }
  
  SGPoint pauseButtonPos2 =
      SGPointMake(SCREEN_WIDTH - pauseButtonTexture.contentSize().width,
                  SCREEN_HEIGHT - pauseButtonTexture.contentSize().height +
                      (false ? (27 * 768.0/320.0) : 0));
  pauseButton2_ = new Button(pauseButtonTexture, pauseButtonPressedTexture, pauseButtonPos2);
// TODO
//  pauseButton2_->set_delegate(self);
//  pauseButton2_->setSelector(@selector(pausePressed));
  AddEntity(*pauseButton2_);
    
//  if (isIPhone) {
//    if (IS_FREE) {
//      player1Wins_ = [[UILabel alloc] initWithFrame:CGRectMake(47, 278 + 26, 150, 35)];
//      player1Wins_.textColor = [UIColor whiteColor];
//    } else {
//      player1Wins_ = [[UILabel alloc] initWithFrame:CGRectMake(5, 448, 150, 35)];
//      player1Wins_.textColor = [UIColor grayColor];
//    }
//  } else {
//    player1Wins_ = [[UILabel alloc] initWithFrame:CGRectMake(106, 644, 150, 35)];
//    player1Wins_.textColor = [UIColor whiteColor];
//  }
//  player1Wins_.backgroundColor = [UIColor clearColor];
//  player1Wins_.textAlignment = UITextAlignmentLeft;
//  if (isIPhone) {
//    player1Wins_.font = [UIFont fontWithName:@"Helvetica-Bold" size:20];
//  } else {
//    player1Wins_.font = [UIFont fontWithName:@"Helvetica-Bold" size:35];
//  }
//  
//  if (isIPhone) {
//    if (IS_FREE) {
//      player2Wins_ = [[UILabel alloc] initWithFrame:CGRectMake(47, 155 + 26, 150, 35)];
//      player2Wins_.textColor = [UIColor whiteColor];
//    } else {
//      player2Wins_ = [[UILabel alloc] initWithFrame:CGRectMake(5, -5, 150, 35)];
//      player2Wins_.textColor = [UIColor grayColor];
//    }
//  } else {
//    player2Wins_ = [[UILabel alloc] initWithFrame:CGRectMake(106, 324, 150, 35)];
//    player2Wins_.textColor = [UIColor whiteColor];
//  }
//  player2Wins_.backgroundColor = [UIColor clearColor];
//  if (isIPhone) {
//    player2Wins_.font = [UIFont fontWithName:@"Helvetica-Bold" size:20];
//  } else {
//    player2Wins_.font = [UIFont fontWithName:@"Helvetica-Bold" size:35];
//  }
//  if (numPlayers_ == 2) {
//    player2Wins_.transform = CGAffineTransformMakeRotation(M_PI); 
//    player2Wins_.textAlignment = UITextAlignmentRight;
//  } else {
//    player2Wins_.textAlignment = UITextAlignmentLeft;
//  }
//  
//  if (!IS_FREE && isIPhone) {
//    player1Wins_.text = @"0 wins";
//    player2Wins_.text = @"0 wins";
//    getGameEngine()->addUIView(player1Wins_);
//    getGameEngine()->addUIView(player2Wins_);
//  }
  
  giveExtraPuckToPlayer_ = PLAYER_1;
  player1WinCount_ = 0;
  player2WinCount_ = 0;
  setUpNewGame();
}

PlayState::~PlayState() {
//  [player1Wins_ release];
//  [player2Wins_ release];
}

void PlayState::Update() {
  if (state_ == PLAY_STATE_PAUSED) {
    return;
  } else if (state_ == PLAY_STATE_GET_READY) {
    getReadyTicksLeft_--;
    if (getReadyTicksLeft_ == SHOW_GET_READY_MESSAGE_TICKS) {
      AddEntity(*getReady_);
      // TODO [SoundPlayer playSound:kSoundGetReady];
    } else if (getReadyTicksLeft_ == 0) {
    RemoveEntity(*getReady_);
      AddEntity(*go_);
      goTicksLeft_ = SHOW_GO_MESSAGE_TICKS;
      state_ = PLAY_STATE_PLAYING;
      // TODO [SoundPlayer playSound:kSoundStart];
    }
    
    return;
  }

  EngineState::Update();
  
  if (goTicksLeft_ > 0) {
    goTicksLeft_--;
    if (goTicksLeft_ == 0) {
      RemoveEntity(*go_);
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
      if (player1Score_->texture() < WIN_SCORE && state_ == PLAY_STATE_PLAYING) {
        player1Score_->set_texture(player1Score_->texture() + 1);
      }
      if (player1Score_->texture() == WIN_SCORE && state_ == PLAY_STATE_PLAYING) {
        // TODO [SoundPlayer playSound:kSoundScoreFinal];  
      } else {
        // TODO [SoundPlayer playSound:kSoundScore];
      }
      numPlayer1ScoresLastRound_++;
      numActivePucks_--;
    } else if (puck->getY() > SCREEN_HEIGHT + puck->getRadius()) {
      puck->setIsActive(false);
      if (player2Score_->texture() < WIN_SCORE && state_ == PLAY_STATE_PLAYING) {
        player2Score_->set_texture(player2Score_->texture() + 1);
      }
      if (player2Score_->texture() == WIN_SCORE && state_ == PLAY_STATE_PLAYING) {
        // TODO [SoundPlayer playSound:kSoundScoreFinal];  
      } else {
        // TODO [SoundPlayer playSound:kSoundScore];
      }
      
      numActivePucks_--;
    }
  }  
  
  switch (state_) {
    case PLAY_STATE_PLAYING: {      
      if (player1Score_->texture() == WIN_SCORE) {
        finishGameWithWinner(PLAYER_1);
      } else if (player2Score_->texture() == WIN_SCORE) {
        finishGameWithWinner(PLAYER_2);
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

void PlayState::setUpNewGame() {
//  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
//  } else {
//    [getGameEngine()->adEngine() removeAd];
//  }

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
    bool center = !((playerId == giveExtraPuckToPlayer_ &&
                      (numPucks_ == 3 || numPucks_ == 4 || numPucks_ == 7)) ||
                      (playerId == 1 - giveExtraPuckToPlayer_ &&
                      (numPucks_ == 4 || numPucks_ == 5)));
    puck->placeForPlayer(playerId, roundThings_, center);
  }
  
  player1Score_->set_texture(0);
  player2Score_->set_texture(0);
  RemoveEntity(*menuBackground_);
  RemoveEntity(*soundSlider_);
  RemoveEntity(*rematchButton_);
  RemoveEntity(*menuButton_);
  RemoveEntity(*win_);
  RemoveEntity(*lose_);
  
  numActivePucks_ = numPucks_;
  numPlayer1ScoresLastRound_ = 0;
  
  state_ = PLAY_STATE_GET_READY;
  getReadyTicksLeft_ = GET_READY_TICKS_TOTAL;
}

void PlayState::finishGameWithWinner(int playerId) {
  state_ = PLAY_STATE_FINISHED;
  
  double loseX = (SCREEN_WIDTH - lose_->size().width)/2;
  double winX =  (SCREEN_WIDTH - win_->size().width)/2;
  double topY = 70;
  double bottomY = SCREEN_HEIGHT - topY - lose_->size().height;
  switch (playerId) {
    case PLAYER_1: {
      player1WinCount_++;

      win_->set_position(SGPointMake(winX, bottomY));
      win_->set_angle(0);
      AddEntity(*win_);
      
      if (numPlayers_ == 2) {
        lose_->set_position(SGPointMake(loseX, topY));
        lose_->set_angle(180);
        AddEntity(*lose_);
      }
      
      giveExtraPuckToPlayer_ = PLAYER_2;
      
      break;
    }
    case PLAYER_2: {
      player2WinCount_++;
      
      if (numPlayers_ == 2) {
        win_->set_position(SGPointMake(winX, topY));
        win_->set_angle(180);
        AddEntity(*win_);
      }
      
      lose_->set_position(SGPointMake(loseX, bottomY));
      lose_->set_angle(0);
      AddEntity(*lose_);
      
      giveExtraPuckToPlayer_ = PLAYER_1;
      
      break;
    }
  }

//  player1Wins_.text =
//      [NSString stringWithFormat:@"%d win%@", player1WinCount_, player1WinCount_ == 1 ? @"" : @"s"];
//  player2Wins_.text =
//      [NSString stringWithFormat:@"%d win%@", player2WinCount_, player2WinCount_ == 1 ? @"" : @"s"];
//  if (IS_FREE || UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//    getGameEngine()->addUIView(player1Wins_);
//    getGameEngine()->addUIView(player2Wins_);
//  }
  
  AddEntity(*menuBackground_);
  AddEntity(*soundSlider_);
  AddEntity(*rematchButton_);
  AddEntity(*menuButton_);
  
//  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//    [getGameEngine()->adEngine() addAdAtPoint:SGPointMake((SCREEN_WIDTH - 320) / 2, 385)];
//  } else {
//    [getGameEngine()->adEngine() addAdAtPoint:SGPointMake(0, 0)];
//  }
}

void PlayState::rematchPressed() {
//  [FlurryAnalytics logEvent:@"REMATCH"];
//  if (IS_FREE || UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//    [player1Wins_ removeFromSuperview];
//    [player2Wins_ removeFromSuperview];
//  }
  setUpNewGame();
//  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//    [getGameEngine()->adEngine() removeAd];
//  }  
}


void PlayState::menuPressed() {
//  [player1Wins_ removeFromSuperview];
//  [player2Wins_ removeFromSuperview];
  game_engine().ReplaceTopState(new MainMenuState(game_engine()));
//  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//    [getGameEngine()->adEngine() removeAd];
//  }
}

void PlayState::continuePressed() {
  state_ = prePauseState_;
  RemoveEntity(*menuBackground_);
  RemoveEntity(*soundSlider_);
  RemoveEntity(*menuButton_);
  RemoveEntity(*continueButton_);
//  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//    [getGameEngine()->adEngine() removeAd];
//  }
}

void PlayState::pausePressed() {
  if (state_ != PLAY_STATE_FINISHED && state_ != PLAY_STATE_PAUSED) {
    prePauseState_ = state_;
    state_ = PLAY_STATE_PAUSED;
    AddEntity(*menuBackground_);
    AddEntity(*soundSlider_);
    AddEntity(*menuButton_);
    AddEntity(*continueButton_);
//    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//      [getGameEngine()->adEngine() addAdAtPoint:SGPointMake((SCREEN_WIDTH - 320)/2, 385)];
//    }
  }
}

void PlayState::TouchesBegan(vector<Touch> touches) {
  // When paused, only allow touches on the menu and continue buttons.
  if (state_ == PLAY_STATE_PAUSED) {
    menuButton_->TouchesBegan(touches);
    continueButton_->TouchesBegan(touches);
    soundSlider_->TouchesBegan(touches);
  } else if (state_ == PLAY_STATE_GET_READY) {
    pauseButton1_->TouchesBegan(touches);
    pauseButton2_->TouchesBegan(touches);
  } else {
    EngineState::TouchesBegan(touches);
  }
}

void PlayState::TouchesMoved(vector<Touch> touches) {
  // When paused, only allow touches on the menu and continue buttons.
  if (state_ == PLAY_STATE_PAUSED) {
    soundSlider_->TouchesMoved(touches);
  } else if (state_ == PLAY_STATE_GET_READY) {
  } else {
    EngineState::TouchesMoved(touches);
  }
}

void PlayState::TouchesEnded(vector<Touch> touches) {
  // When paused, only allow touches on the menu and continue buttons.
  if (state_ == PLAY_STATE_PAUSED) {
    menuButton_->TouchesEnded(touches);
    continueButton_->TouchesEnded(touches);
    soundSlider_->TouchesEnded(touches);
  } else if (state_ == PLAY_STATE_GET_READY) {
    pauseButton1_->TouchesEnded(touches);
    pauseButton2_->TouchesEnded(touches);
  } else {
    EngineState::TouchesEnded(touches);
  }
}

void PlayState::ClearTouches() {
  EngineState::ClearTouches();
  pausePressed();
}
