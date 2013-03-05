//
//  play_view.cc
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "airhockey/views/play_view.h"

#include "airhockey/entities/puck.h"
#include "airhockey/views/main_menu_view.h"
#include "gameengine/game_engine.h"
#include "gameengine/resource_loader.h"
#include "soundengine/sound_player.h"

PlayView::PlayView(sp<GameEngine> game_engine, int num_players, int num_pucks, ComputerAI difficulty,
                   PaddleSize paddle_size)
    : EngineView(game_engine) {
  num_players_ = num_players;
  
  paddle_1_.reset(new Paddle(game_engine, PLAYER_1, paddle_size, true, caiBad, pucks_));
  paddle_2_.reset(new Paddle(game_engine, PLAYER_2, paddle_size, num_players == 2, difficulty,
                             pucks_));

  post_1_.reset(new Post(game_engine, GOAL_LEFT_X, RINK_TOP_Y));
  post_2_.reset(new Post(game_engine, GOAL_LEFT_X, RINK_BOTTOM_Y + 1));
  post_3_.reset(new Post(game_engine, GOAL_RIGHT_X + 1, RINK_TOP_Y));
  post_4_.reset(new Post(game_engine, GOAL_RIGHT_X + 1, RINK_BOTTOM_Y + 1));

  sound_slider_.reset(new SoundSlider(game_engine, game_point_make(331, 336)));

  rink_.reset(new Rink());
  AddEntity(rink_);
  
  vector<Sprite> scoreSprites;
  for (int i = 0; i <= WIN_SCORE; i++) {
    char pointsstr[15];
    sprintf(pointsstr, "%d_points", i);
    Sprite sprite(game_engine, pointsstr);
    // TODO: This is just to bump the use count of the texture since it's being used twice.
    // Rethink how we handle textures.
    ResourceLoader::Instance().TextureWithName(pointsstr);
    scoreSprites.push_back(sprite);
  }
  player_1_score_.reset(new SimpleItem());
  player_1_score_->set_sprites(scoreSprites);
  player_1_score_->set_position(game_engine->position("player_1_score"));
  player_2_score_.reset(new SimpleItem());
  player_2_score_->set_sprites(scoreSprites);
  player_2_score_->set_position(game_engine->position("player_2_score"));
  AddEntity(player_1_score_);
  AddEntity(player_2_score_);
  
  num_pucks_ = num_pucks;
  num_active_pucks_ = num_pucks_;
  for (int i = 0; i < num_pucks_; i++) {
    pucks_.push_back(sp<Puck>(new Puck(game_engine)));
    AddEntity(pucks_[i]);
    round_things_.push_back(pucks_[i]);
  }
  
  AddEntity(paddle_1_);
  round_things_.push_back(paddle_1_);

  AddEntity(paddle_2_);
  round_things_.push_back(paddle_2_);
  
  AddEntity(post_1_);
  round_things_.push_back(post_1_);

  AddEntity(post_2_);
  round_things_.push_back(post_2_);

  AddEntity(post_3_);
  round_things_.push_back(post_3_);
  
  AddEntity(post_4_);
  round_things_.push_back(post_4_);
  
  Sprite winSprite(game_engine, "win");
  win_.reset(new SimpleItem());
  win_->add_sprite(winSprite);
  win_->set_position(game_point_make(0, 0));

  Sprite loseSprite(game_engine, "lose");
  lose_.reset(new SimpleItem());
  lose_->add_sprite(loseSprite);
  lose_->set_position(game_point_make(0, 0));

  Sprite getReadySprite(game_engine, "get_ready");
  GamePoint getReadyPosition = game_engine->position("get_ready");
  get_ready_.reset(new SimpleItem());
  get_ready_->add_sprite(getReadySprite);
  get_ready_->set_position(getReadyPosition);

  Sprite goSprite(game_engine, "go");
  GamePoint goPosition = game_engine->position("go");
  go_.reset(new SimpleItem());
  go_->add_sprite(goSprite);
  go_->set_position(goPosition);

  // Add rink left and right pieces.
  Sprite left_rink_border_sprite(game_engine, "rink_left");
  SimpleItem *left_rink_border = new SimpleItem(left_rink_border_sprite,
                                                game_engine->position("rink_left"));
  AddEntity(left_rink_border);
  Sprite right_rink_border_sprite(game_engine, "rink_right");
  SimpleItem *right_rink_border = new SimpleItem(right_rink_border_sprite,
                                                 game_engine->position("rink_right"));
  AddEntity(right_rink_border);
      
  Sprite rematch_button_sprite(game_engine, "rematch_button");
  Sprite rematch_button_pressed_sprite(game_engine, "rematch_button_pressed");
  GamePoint rematch_button_pos =
      game_point_make((SCREEN_WIDTH - rematch_button_sprite.content_size().width) / 2, 441);
  rematch_button_.reset(new Button());
  rematch_button_->set_normal_sprite(rematch_button_sprite);
  rematch_button_->set_pressed_sprite(rematch_button_pressed_sprite);
  rematch_button_->set_position(rematch_button_pos);
  rematch_button_->set_delegate(this);

  Sprite menu_button_sprite(game_engine, "menu_button");
  Sprite menu_button_pressed_sprite(game_engine, "menu_button_pressed");
  GamePoint menu_button_pos =
      game_point_make((SCREEN_WIDTH - menu_button_sprite.content_size().width) / 2, 546);
  menu_button_.reset(new Button());
  menu_button_->set_normal_sprite(menu_button_sprite);
  menu_button_->set_pressed_sprite(menu_button_pressed_sprite);
  menu_button_->set_position(menu_button_pos);
  menu_button_->set_delegate(this);

  Sprite continue_button_sprite(game_engine, "continue_button");
  Sprite continue_button_pressed_sprite(game_engine, "continue_button_pressed");
  GamePoint continue_button_pos =
      game_point_make((SCREEN_WIDTH - continue_button_sprite.content_size().width) / 2, 441);
  continue_button_.reset(new Button());
  continue_button_->set_normal_sprite(continue_button_sprite);
  continue_button_->set_pressed_sprite(continue_button_pressed_sprite);
  continue_button_->set_position(continue_button_pos);
  continue_button_->set_delegate(this);

  Sprite menu_background_sprite(game_engine, "game_menu_bg");
  GamePoint menu_background_position =
      game_point_make((SCREEN_WIDTH - menu_background_sprite.content_size().width) / 2, 306);
  menu_background_.reset(new SimpleItem());
  menu_background_->add_sprite(menu_background_sprite);
  menu_background_->set_position(menu_background_position);

  Sprite pause_button_sprite(game_engine, "pause_button");
  Sprite pause_button_pressed_sprite(game_engine, "pause_button_pressed");

  bool is_iphone = true;  // (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone);

  if (!is_iphone) {
    GamePoint pause_button_pos_1 = game_point_make(0, 0);
    pause_button_1_.reset(new Button());
    pause_button_1_->set_normal_sprite(pause_button_sprite);
    pause_button_1_->set_pressed_sprite(pause_button_pressed_sprite);
    pause_button_1_->set_position(pause_button_pos_1);
    pause_button_1_->set_delegate(this);
    AddEntity(pause_button_1_);
  }

  GamePoint pause_button_pos_2 =
      game_point_make(SCREEN_WIDTH - pause_button_sprite.content_size().width,
                      SCREEN_HEIGHT - pause_button_sprite.content_size().height +
                          (false ? (27 * 768.0/320.0) : 0));
  pause_button_2_.reset(new Button());
  pause_button_2_->set_normal_sprite(pause_button_sprite);
  pause_button_2_->set_pressed_sprite(pause_button_pressed_sprite);
  pause_button_2_->set_position(pause_button_pos_2);
  pause_button_2_->set_delegate(this);
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


// EngineView

void PlayView::Update() {
  if (state_ == kPlayViewStatePaused) {
    return;
  } else if (state_ == kPlayViewStateGetReady) {
    get_ready_ticks_left_--;
    if (get_ready_ticks_left_ == SHOW_GET_READY_MESSAGE_TICKS) {
      AddEntity(get_ready_);
      SoundPlayer::instance()->playSound(kSoundGetReady);
    } else if (get_ready_ticks_left_ == 0) {
      RemoveEntity(get_ready_);
      AddEntity(go_);
      go_ticks_left_ = SHOW_GO_MESSAGE_TICKS;
      state_ = kPlayViewStatePlaying;
      paddle_1_->set_ready_to_play(true);
      paddle_2_->set_ready_to_play(true);
      SoundPlayer::instance()->playSound(kSoundStart);
    }
    
    return;
  }

  EngineView::Update();
  
  if (go_ticks_left_ > 0) {
    go_ticks_left_--;
    if (go_ticks_left_ == 0) {
      RemoveEntity(go_);
    }
  }
  
  paddle_1_->KeepInPlayerBounds();
  paddle_2_->KeepInPlayerBounds();
  
  for (int i = 0; i < round_things_.size(); i++) {
    RoundThing *thing = round_things_[i].get();
    if (!thing->is_active()) {
      continue;
    }
    rink_->BounceOff(thing);
    for (int j = i + 1; j < round_things_.size(); j++) {
      RoundThing *otherThing = round_things_[j].get();
      if (otherThing->is_active()) {
        thing->MaybeBounceOff(otherThing);
      }
    }
    
    thing->ApplyFriction();
    
    // TODO If you grab item A and push item B into a corner,
    // it only behaves if item A was added to roundsThings_
    // after item B. This is OK for Air Hockey, but should be fixed
    // for other games.
    rink_->MoveInFromEdge(thing);
  }

  for (int i = 0; i < pucks_.size(); i++) {
    Puck *puck = pucks_[i].get();
    if (!puck->is_active()) {
      continue;
    }
    if (puck->y() < -puck->radius()) {
      puck->set_active(false);
      if (player_1_score_->sprite() < WIN_SCORE && state_ == kPlayViewStatePlaying) {
        player_1_score_->set_sprite(player_1_score_->sprite() + 1);
      }
      if (player_1_score_->sprite() == WIN_SCORE && state_ == kPlayViewStatePlaying) {
        SoundPlayer::instance()->playSound(kSoundScoreFinal);
      } else {
        SoundPlayer::instance()->playSound(kSoundScore);
      }
      num_player_1_scores_last_round_++;
      num_active_pucks_--;
    } else if (puck->y() > SCREEN_HEIGHT + puck->radius()) {
      puck->set_active(false);
      if (player_2_score_->sprite() < WIN_SCORE && state_ == kPlayViewStatePlaying) {
        player_2_score_->set_sprite(player_2_score_->sprite() + 1);
      }
      if (player_2_score_->sprite() == WIN_SCORE && state_ == kPlayViewStatePlaying) {
        SoundPlayer::instance()->playSound(kSoundScoreFinal);
      } else {
        SoundPlayer::instance()->playSound(kSoundScore);
      }
      
      num_active_pucks_--;
    }
  }  
  
  switch (state_) {
    case kPlayViewStateGetReady:
      break;
    case kPlayViewStatePlaying: {      
      if (player_1_score_->sprite() == WIN_SCORE) {
        FinishGameWithWinner(PLAYER_1);
      } else if (player_2_score_->sprite() == WIN_SCORE) {
        FinishGameWithWinner(PLAYER_2);
      } else if (num_active_pucks_ == 0) {
        wait_ticks_left_ = WAIT_TICKS;
        state_ = kPlayViewStateWaitingForPucks;
      }
      break;
    }
    case kPlayViewStateWaitingForPucks: {
      if (wait_ticks_left_-- == 0) {
        for (int i = 0; i < num_pucks_; i++) {
          Puck *puck = pucks_[i].get();
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
        
        state_ = kPlayViewStatePlaying;        

        break;
      }
    }
    case kPlayViewStateFinished:
      break;
    case kPlayViewStatePaused:
      break;
  }
}


// ButtonDelegate

void PlayView::ButtonPressed(Button *button) {
  if (button == rematch_button_.get()) {
    RematchPressed();
  } else if (button == menu_button_.get()) {
    MenuPressed();
  } else if (button == continue_button_.get()) {
    ContinuePressed();
  } else if (button == pause_button_1_.get() || button == pause_button_2_.get()) {
    PausePressed();
  }
}


// private

void PlayView::SetUpNewGame() {
//  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
//  } else {
//    [getGameEngine()->adEngine() removeAd];
//  }

  // Place paddles!
  paddle_1_->SetInitialPositionForPlayer(PLAYER_1);
  paddle_2_->SetInitialPositionForPlayer(PLAYER_2);
  paddle_1_->set_ready_to_play(false);
  paddle_2_->set_ready_to_play(false);

  // Place pucks!
  // First move them all out of the way. That way we can lay them out properly.
  // ([Puck placeForPlayer] avoids hitting other RoundThings objects.)
  for (int i = 0; i < num_pucks_; i++) {
    Puck *puck = pucks_[i].get();
    puck->set_x(0);
    puck->set_y(0);
  }
  for (int i = 0; i < num_pucks_; i++) {
    Puck *puck = pucks_[i].get();
    puck->set_active(true);
    int playerId = (i % 2 == 0) ? give_extra_puck_to_player_ : 1 - give_extra_puck_to_player_;
    bool center = !((playerId == give_extra_puck_to_player_ &&
                      (num_pucks_ == 3 || num_pucks_ == 4 || num_pucks_ == 7)) ||
                      (playerId == 1 - give_extra_puck_to_player_ &&
                      (num_pucks_ == 4 || num_pucks_ == 5)));
    puck->PlaceForPlayer(playerId, round_things_, center);
  }

  player_1_score_->set_sprite(0);
  player_2_score_->set_sprite(0);
  RemoveEntity(menu_background_);
  RemoveEntity(sound_slider_);
  RemoveEntity(rematch_button_);
  RemoveEntity(continue_button_);
  RemoveEntity(win_);
  RemoveEntity(lose_);
  
  num_active_pucks_ = num_pucks_;
  num_player_1_scores_last_round_ = 0;
  
  state_ = kPlayViewStateGetReady;
  get_ready_ticks_left_ = GET_READY_TICKS_TOTAL;
}

void PlayView::FinishGameWithWinner(int playerId) {
  state_ = kPlayViewStateFinished;

  double loseX = (SCREEN_WIDTH - lose_->size().width)/2;
  double winX =  (SCREEN_WIDTH - win_->size().width)/2;
  double topY = 70;
  double bottomY = SCREEN_HEIGHT - topY - lose_->size().height;
  switch (playerId) {
    case PLAYER_1: {
      player_1_win_count_++;

      win_->set_position(game_point_make(winX, bottomY));
      win_->set_angle(0);
      AddEntity(win_);
      
      if (num_players_ == 2) {
        lose_->set_position(game_point_make(loseX, topY));
        lose_->set_angle(180);
        AddEntity(lose_);
      }
      
      give_extra_puck_to_player_ = PLAYER_2;
      
      break;
    }
    case PLAYER_2: {
      player_2_win_count_++;
      
      if (num_players_ == 2) {
        win_->set_position(game_point_make(winX, topY));
        win_->set_angle(180);
        AddEntity(win_);
      }
      
      lose_->set_position(game_point_make(loseX, bottomY));
      lose_->set_angle(0);
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
//    [getGameEngine()->adEngine() addAdAtPoint:screen_point_make((SCREEN_WIDTH - 320) / 2, 385)];
//  } else {
//    [getGameEngine()->adEngine() addAdAtPoint:screen_point_make(0, 0)];
//  }
}

void PlayView::RematchPressed() {
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


void PlayView::MenuPressed() {
//  [player1Wins_ removeFromSuperview];
//  [player2Wins_ removeFromSuperview];
  game_engine()->PopView();
  game_engine()->PushView(sp<EngineView>(new MainMenuView(game_engine())));
//  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//    [getGameEngine()->adEngine() removeAd];
//  }
}

void PlayView::ContinuePressed() {
  state_ = pre_pause_state_;
  RemoveEntity(menu_background_);
  RemoveEntity(sound_slider_);
  RemoveEntity(menu_button_);
  RemoveEntity(continue_button_);
//  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//    [getGameEngine()->adEngine() removeAd];
//  }
}

void PlayView::PausePressed() {
  if (state_ != kPlayViewStateFinished && state_ != kPlayViewStatePaused) {
    pre_pause_state_ = state_;
    state_ = kPlayViewStatePaused;
    AddEntity(menu_background_);
    AddEntity(sound_slider_);
    AddEntity(menu_button_);
    AddEntity(continue_button_);
//    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//      [getGameEngine()->adEngine() addAdAtPoint:screen_point_make((SCREEN_WIDTH - 320)/2, 385)];
//    }
  }
}
