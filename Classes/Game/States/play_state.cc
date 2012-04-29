//
//  play_state.cc
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "game/states/play_state.h"

#include "game/entities/puck.h"
#include "game/states/main_menu_state.h"
#include "gameengine/game_engine.h"
#include "gameengine/ResourceLoader.h"

PlayState::PlayState(GameEngine &game_engine, int num_players, int num_pucks, ComputerAI difficulty,
                     PaddleSize paddle_size)
    : EngineState(game_engine),
      paddle_1_(PLAYER_1, paddle_size, true, caiBad, pucks_),
      post_1_(GOAL_LEFT_X, RINK_TOP_Y),
      post_2_(GOAL_LEFT_X, RINK_BOTTOM_Y + 1),
      post_3_(GOAL_RIGHT_X + 1, RINK_TOP_Y),
      post_4_(GOAL_RIGHT_X + 1, RINK_BOTTOM_Y + 1),
      paddle_2_(PLAYER_2, paddle_size, num_players == 2, difficulty, pucks_),
      sound_slider_(SGPointMake(331, 336)),
      pucks_(num_pucks) {
  num_players_ = num_players;
  
  AddEntity(rink_);
  
  vector<Texture2D> scoreTextures;
  for (int i = 0; i <= WIN_SCORE; i++) {
    char pointsstr[15];
    sprintf(pointsstr, "%d_points", i);
    Texture2D texture = ResourceLoader::Instance().TextureWithName(pointsstr);
    scoreTextures.push_back(texture);
  }
  player_1_score_.set_textures(scoreTextures);
  player_1_score_.set_position(SGPointMake(662, 526));
  player_2_score_.set_textures(scoreTextures);
  player_2_score_.set_position(SGPointMake(662, 386));
  AddEntity(player_1_score_);
  AddEntity(player_2_score_);
  
  num_pucks_ = num_pucks;
  num_active_pucks_ = num_pucks_;
  for (int i = 0; i < num_pucks_; i++) {
    AddEntity(pucks_[i]);
    round_things_.push_back(&pucks_[i]);
  }
  
  AddEntity(paddle_1_);
  round_things_.push_back(&paddle_1_);

  AddEntity(paddle_2_);
  round_things_.push_back(&paddle_2_);
  
  paddle_1_.set_other_paddle(&paddle_2_);
  paddle_2_.set_other_paddle(&paddle_1_);
  
  AddEntity(post_1_);
  round_things_.push_back(&post_1_);

  AddEntity(post_2_);
  round_things_.push_back(&post_2_);

  AddEntity(post_3_);
  round_things_.push_back(&post_3_);
  
  AddEntity(post_4_);
  round_things_.push_back(&post_4_);
  
  // Add rink left and right pieces.
  Texture2D leftRinkBorderTexture = ResourceLoader::Instance().TextureWithName("rink_left");
  SimpleItem *leftRinkBorder = new SimpleItem(leftRinkBorderTexture, SGPointMake(0, 0));
  AddEntity(*leftRinkBorder);
  Texture2D rightRinkBorderTexture = ResourceLoader::Instance().TextureWithName("rink_right");
  SGPoint leftRinkBorderPos = SGPointMake(SCREEN_WIDTH - rightRinkBorderTexture.content_size().width,
                                          0);
  SimpleItem *rightRinkBorder = new SimpleItem(rightRinkBorderTexture, leftRinkBorderPos);
  AddEntity(*rightRinkBorder);
  
  Texture2D winTexture = ResourceLoader::Instance().TextureWithName("win");
  win_.add_texture(winTexture);
  win_.set_position(SGPointMake(0, 0));

  Texture2D loseTexture = ResourceLoader::Instance().TextureWithName("lose");
  lose_.add_texture(loseTexture);
  lose_.set_position(SGPointMake(0, 0));

  Texture2D getReadyTexture = ResourceLoader::Instance().TextureWithName("get_ready");
  SGPoint getReadyPosition =
      SGPointMake((SCREEN_WIDTH - getReadyTexture.content_size().width) / 2, 
                  (SCREEN_HEIGHT - getReadyTexture.content_size().height) / 2);
  get_ready_.add_texture(getReadyTexture);
  get_ready_.set_position(getReadyPosition);

  Texture2D goTexture = ResourceLoader::Instance().TextureWithName("go");
  SGPoint goPosition = SGPointMake((SCREEN_WIDTH - goTexture.content_size().width) / 2, 
                                   (SCREEN_HEIGHT - goTexture.content_size().height) / 2);
  go_.add_texture(goTexture);
  go_.set_position(goPosition);
  
  Texture2D rematchButtonTexture =
      ResourceLoader::Instance().TextureWithName("rematch_button");
  Texture2D rematchButtonPressedTexture =
      ResourceLoader::Instance().TextureWithName("rematch_button_pressed");
  SGPoint rematchButtonPos =
      SGPointMake((SCREEN_WIDTH - rematchButtonTexture.content_size().width) / 2, 441);
  rematch_button_.set_normal_texture(rematchButtonTexture);
  rematch_button_.set_pressed_texture(rematchButtonPressedTexture);
  rematch_button_.set_position(rematchButtonPos);
  rematch_button_.set_delegate(this);

  Texture2D menuButtonTexture = ResourceLoader::Instance().TextureWithName("menu_button");
  Texture2D menuButtonPressedTexture =
      ResourceLoader::Instance().TextureWithName("menu_button_pressed");
  SGPoint menuButtonPos = SGPointMake((SCREEN_WIDTH - menuButtonTexture.content_size().width) / 2,
                                      546);
  continue_button_.set_normal_texture(menuButtonTexture);
  continue_button_.set_pressed_texture(menuButtonPressedTexture);
  continue_button_.set_position(menuButtonPos);
  continue_button_.set_delegate(this);

  Texture2D continueButtonTexture =
      ResourceLoader::Instance().TextureWithName("continue_button");
  Texture2D continueButtonPressedTexture =
      ResourceLoader::Instance().TextureWithName("continue_button_pressed");
  SGPoint continueButtonPos =
      SGPointMake((SCREEN_WIDTH - continueButtonTexture.content_size().width) / 2, 441);
  continue_button_.set_normal_texture(continueButtonTexture);
  continue_button_.set_pressed_texture(continueButtonPressedTexture);
  continue_button_.set_position(continueButtonPos);
  continue_button_.set_delegate(this);
  
  Texture2D menuBackgroundTexture =
      ResourceLoader::Instance().TextureWithName("game_menu_bg");
  SGPoint menuBackgroundPosition =
      SGPointMake((SCREEN_WIDTH - menuBackgroundTexture.content_size().width) / 2, 306);
  menu_background_.add_texture(menuBackgroundTexture);
  menu_background_.set_position(menuBackgroundPosition);
  
  Texture2D pauseButtonTexture = ResourceLoader::Instance().TextureWithName("pause_button");
  Texture2D pauseButtonPressedTexture =
      ResourceLoader::Instance().TextureWithName("pause_button_pressed");
  
  bool is_iphone = true;  // (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone);
  
  if (!is_iphone) {
    SGPoint pauseButtonPos1 = SGPointMake(0, 0);
    pause_button_1_.set_normal_texture(pauseButtonTexture);
    pause_button_1_.set_pressed_texture(pauseButtonPressedTexture);
    pause_button_1_.set_position(pauseButtonPos1);
    pause_button_1_.set_delegate(this);
    AddEntity(pause_button_1_);
  }
  
  SGPoint pauseButtonPos2 =
      SGPointMake(SCREEN_WIDTH - pauseButtonTexture.content_size().width,
                  SCREEN_HEIGHT - pauseButtonTexture.content_size().height +
                      (false ? (27 * 768.0/320.0) : 0));
  pause_button_2_.set_normal_texture(pauseButtonTexture);
  pause_button_2_.set_pressed_texture(pauseButtonPressedTexture);
  pause_button_2_.set_position(pauseButtonPos2);
  pause_button_2_.set_delegate(this);
  AddEntity(pause_button_2_);
    
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
//  if (num_players_ == 2) {
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
  
  give_extra_puck_to_player_ = PLAYER_1;
  player_1_win_count_ = 0;
  player_2_win_count_ = 0;
  SetUpNewGame();
}

PlayState::~PlayState() {
//  [player1Wins_ release];
//  [player2Wins_ release];
}


// EngineState

void PlayState::Update() {
  if (state_ == kPlayStateStatePaused) {
    return;
  } else if (state_ == kPlayStateStateGetReady) {
    get_ready_ticks_left_--;
    if (get_ready_ticks_left_ == SHOW_GET_READY_MESSAGE_TICKS) {
      AddEntity(get_ready_);
      // TODO [SoundPlayer playSound:kSoundGetReady];
    } else if (get_ready_ticks_left_ == 0) {
    RemoveEntity(get_ready_);
      AddEntity(go_);
      go_ticks_left_ = SHOW_GO_MESSAGE_TICKS;
      state_ = kPlayStateStatePlaying;
      // TODO [SoundPlayer playSound:kSoundStart];
    }
    
    return;
  }

  EngineState::Update();
  
  if (go_ticks_left_ > 0) {
    go_ticks_left_--;
    if (go_ticks_left_ == 0) {
      RemoveEntity(go_);
    }
  }
  
  paddle_1_.KeepInPlayerBounds();
  paddle_2_.KeepInPlayerBounds();
  
  for (int i = 0; i < round_things_.size(); i++) {
    RoundThing *thing = round_things_[i];
    if (!thing->is_active()) {
      continue;
    }
    rink_.BounceOff(thing);
    for (int j = i + 1; j < round_things_.size(); j++) {
      RoundThing *otherThing = round_things_[j];
      if (otherThing->is_active()) {
        thing->BounceOff(otherThing);
      }
    }
    
    thing->ApplyFriction();
    
    // TODO If you grab item A and push item B into a corner,
    // it only behaves if item A was added to roundsThings_
    // after item B. This is OK for Air Hockey, but should be fixed
    // for other games.
    rink_.MoveInFromEdge(thing);
  }

  for (int i = 0; i < pucks_.size(); i++) {
    Puck *puck = &pucks_[i];
    if (!puck->is_active()) {
      continue;
    }
    if (puck->y() < -puck->radius()) {
      puck->set_active(false);
      if (player_1_score_.texture() < WIN_SCORE && state_ == kPlayStateStatePlaying) {
        player_1_score_.set_texture(player_1_score_.texture() + 1);
      }
      if (player_1_score_.texture() == WIN_SCORE && state_ == kPlayStateStatePlaying) {
        // TODO [SoundPlayer playSound:kSoundScoreFinal];  
      } else {
        // TODO [SoundPlayer playSound:kSoundScore];
      }
      num_player_1_scores_last_round_++;
      num_active_pucks_--;
    } else if (puck->y() > SCREEN_HEIGHT + puck->radius()) {
      puck->set_active(false);
      if (player_2_score_.texture() < WIN_SCORE && state_ == kPlayStateStatePlaying) {
        player_2_score_.set_texture(player_2_score_.texture() + 1);
      }
      if (player_2_score_.texture() == WIN_SCORE && state_ == kPlayStateStatePlaying) {
        // TODO [SoundPlayer playSound:kSoundScoreFinal];  
      } else {
        // TODO [SoundPlayer playSound:kSoundScore];
      }
      
      num_active_pucks_--;
    }
  }  
  
  switch (state_) {
    case kPlayStateStatePlaying: {      
      if (player_1_score_.texture() == WIN_SCORE) {
        FinishGameWithWinner(PLAYER_1);
      } else if (player_2_score_.texture() == WIN_SCORE) {
        FinishGameWithWinner(PLAYER_2);
      } else if (num_active_pucks_ == 0) {
        wait_ticks_left_ = WAIT_TICKS;
        state_ = kPlayStateStateWaitingForPucks;
      }
      break;
    }
    case kPlayStateStateWaitingForPucks: {
      if (wait_ticks_left_-- == 0) {
        for (int i = 0; i < num_pucks_; i++) {
          Puck *puck = &pucks_[i];
          puck->set_active(true);
          puck->PlaceForPlayer(i < num_player_1_scores_last_round_ ? PLAYER_2 : PLAYER_1,
                               round_things_,
                                (i < num_player_1_scores_last_round_ ?
                                    (num_player_1_scores_last_round_ % 2 == 1) :
                                    ((num_pucks_ - num_player_1_scores_last_round_) % 2 == 1)));
          puck->FadeIn();
        }
        num_active_pucks_ = num_pucks_;
        num_player_1_scores_last_round_ = 0;
        
        state_ = kPlayStateStatePlaying;        

        break;
      }
    }
  }
}


// ButtonDelegate

void PlayState::ButtonPressed(Button *button) {
}


// private

void PlayState::SetUpNewGame() {
//  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
//  } else {
//    [getGameEngine()->adEngine() removeAd];
//  }

  // Place paddles!
  paddle_1_.SetInitialPositionForPlayer(PLAYER_1);
  paddle_2_.SetInitialPositionForPlayer(PLAYER_2);
  
  // Place pucks!
  // First move them all out of the way. That way we can lay them out properly.
  // ([Puck placeForPlayer] avoids hitting other RoundThings objects.)
  for (int i = 0; i < num_pucks_; i++) {
    Puck *puck = &pucks_[i];
    puck->set_x(0);
    puck->set_y(0);
  }
  for (int i = 0; i < num_pucks_; i++) {
    Puck *puck = &pucks_[i];
    puck->set_active(true);
    int playerId = (i % 2 == 0) ? give_extra_puck_to_player_ : 1 - give_extra_puck_to_player_;
    bool center = !((playerId == give_extra_puck_to_player_ &&
                      (num_pucks_ == 3 || num_pucks_ == 4 || num_pucks_ == 7)) ||
                      (playerId == 1 - give_extra_puck_to_player_ &&
                      (num_pucks_ == 4 || num_pucks_ == 5)));
    puck->PlaceForPlayer(playerId, round_things_, center);
  }
  
  player_1_score_.set_texture(0);
  player_2_score_.set_texture(0);
  RemoveEntity(menu_background_);
  RemoveEntity(sound_slider_);
  RemoveEntity(rematch_button_);
  RemoveEntity(continue_button_);
  RemoveEntity(win_);
  RemoveEntity(lose_);
  
  num_active_pucks_ = num_pucks_;
  num_player_1_scores_last_round_ = 0;
  
  state_ = kPlayStateStateGetReady;
  get_ready_ticks_left_ = GET_READY_TICKS_TOTAL;
}

void PlayState::FinishGameWithWinner(int playerId) {
  state_ = kPlayStateStateFinished;
  
  double loseX = (SCREEN_WIDTH - lose_.size().width)/2;
  double winX =  (SCREEN_WIDTH - win_.size().width)/2;
  double topY = 70;
  double bottomY = SCREEN_HEIGHT - topY - lose_.size().height;
  switch (playerId) {
    case PLAYER_1: {
      player_1_win_count_++;

      win_.set_position(SGPointMake(winX, bottomY));
      win_.set_angle(0);
      AddEntity(win_);
      
      if (num_players_ == 2) {
        lose_.set_position(SGPointMake(loseX, topY));
        lose_.set_angle(180);
        AddEntity(lose_);
      }
      
      give_extra_puck_to_player_ = PLAYER_2;
      
      break;
    }
    case PLAYER_2: {
      player_2_win_count_++;
      
      if (num_players_ == 2) {
        win_.set_position(SGPointMake(winX, topY));
        win_.set_angle(180);
        AddEntity(win_);
      }
      
      lose_.set_position(SGPointMake(loseX, bottomY));
      lose_.set_angle(0);
      AddEntity(lose_);
      
      give_extra_puck_to_player_ = PLAYER_1;
      
      break;
    }
  }

//  player1Wins_.text =
//      [NSString stringWithFormat:@"%d win%@", player_1_win_count_, player_1_win_count_ == 1 ? @"" : @"s"];
//  player2Wins_.text =
//      [NSString stringWithFormat:@"%d win%@", player_2_win_count_, player_2_win_count_ == 1 ? @"" : @"s"];
//  if (IS_FREE || UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//    getGameEngine()->addUIView(player1Wins_);
//    getGameEngine()->addUIView(player2Wins_);
//  }
  
  AddEntity(menu_background_);
  AddEntity(sound_slider_);
  AddEntity(rematch_button_);
  AddEntity(continue_button_);
  
//  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//    [getGameEngine()->adEngine() addAdAtPoint:SGPointMake((SCREEN_WIDTH - 320) / 2, 385)];
//  } else {
//    [getGameEngine()->adEngine() addAdAtPoint:SGPointMake(0, 0)];
//  }
}

void PlayState::RematchPressed() {
//  [FlurryAnalytics logEvent:@"REMATCH"];
//  if (IS_FREE || UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//    [player1Wins_ removeFromSuperview];
//    [player2Wins_ removeFromSuperview];
//  }
  SetUpNewGame();
//  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//    [getGameEngine()->adEngine() removeAd];
//  }  
}


void PlayState::MenuPressed() {
//  [player1Wins_ removeFromSuperview];
//  [player2Wins_ removeFromSuperview];
  game_engine().ReplaceTopState(new MainMenuState(game_engine()));
//  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//    [getGameEngine()->adEngine() removeAd];
//  }
}

void PlayState::ContinuePressed() {
  state_ = pre_pause_state_;
  RemoveEntity(menu_background_);
  RemoveEntity(sound_slider_);
  RemoveEntity(continue_button_);
  RemoveEntity(continue_button_);
//  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//    [getGameEngine()->adEngine() removeAd];
//  }
}

void PlayState::PausePressed() {
  if (state_ != kPlayStateStateFinished && state_ != kPlayStateStatePaused) {
    pre_pause_state_ = state_;
    state_ = kPlayStateStatePaused;
    AddEntity(menu_background_);
    AddEntity(sound_slider_);
    AddEntity(continue_button_);
    AddEntity(continue_button_);
//    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//      [getGameEngine()->adEngine() addAdAtPoint:SGPointMake((SCREEN_WIDTH - 320)/2, 385)];
//    }
  }
}
