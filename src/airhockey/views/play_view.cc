//
//  play_view.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2010-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "airhockey/views/play_view.h"

#include "gameengine/entities/simple_item.h"
#include "gameengine/modules/ad_engine.h"
#include "gameengine/modules/analytics_engine.h"
#include "gameengine/modules/local_store.h"
#include "gameengine/modules/sound_player.h"
#include "gameengine/game_engine.h"
#include "gameengine/resource_loader.h"

#include "airhockey/entities/post.h"
#include "airhockey/entities/puck.h"
#include "airhockey/entities/rink.h"
#include "airhockey/entities/round_thing.h"
#include "airhockey/views/main_menu_view.h"

using std::string;
using std::vector;

static const int kWaitTicks = 60;
static const int kGetReadyTicksTotal = 120;
static const int kShowGetReadyMessageTicks = 90;
static const int kShowGoMessageTicks = 30;
static const int kWinScore = 7;
static const int kFullScreenAdFrequency = 4;
static const string kLocalStoreMatchCount = "ls_match_count";

PlayView::PlayView(sp<GameEngine> game_engine, int num_players, int num_pucks,
                   ComputerAI difficulty, PaddleSize paddle_size)
    : EngineView(game_engine) {
  num_players_ = num_players;

  paddle_1_.reset(new Paddle(game_engine, kPlayerId1, paddle_size, true, kComputerAIBad, pucks_));
  paddle_2_.reset(new Paddle(game_engine, kPlayerId2, paddle_size, num_players == 2, difficulty,
                             pucks_));

  post_1_.reset(new Post(game_engine, Rink::GoalLeftX(), Rink::TopY()));
  post_2_.reset(new Post(game_engine, Rink::GoalLeftX(), Rink::BottomY() + 1));
  post_3_.reset(new Post(game_engine, Rink::GoalRightX() + 1, Rink::TopY()));
  post_4_.reset(new Post(game_engine, Rink::GoalRightX() + 1, Rink::BottomY() + 1));

  rink_.reset(new Rink());
  AddEntity(rink_);

  vector<Sprite> scoreSprites;
  for (int i = 0; i <= kWinScore; i++) {
    char pointsstr[15];
    sprintf(pointsstr, "%d_points", i);
    Sprite sprite(game_engine, pointsstr);
    // TODO: This is just to bump the use count of the texture since it's being used twice.
    // Rethink how we handle textures.
    game_engine->resource_loader().TextureWithName(pointsstr);
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

  Sprite get_ready_sprite(game_engine, "get_ready");
  GameSize screen_size = game_engine->game_size();
  GameSize get_ready_size = get_ready_sprite.content_size();
  GamePoint get_ready_position =
      game_point_make(floorf((screen_size.width - get_ready_size.width) / 2),
                      floorf((screen_size.height - get_ready_size.height) / 2));
  get_ready_.reset(new SimpleItem());
  get_ready_->add_sprite(get_ready_sprite);
  get_ready_->set_position(get_ready_position);

  Sprite go_sprite(game_engine, "go");
  GameSize go_size = go_sprite.content_size();
  GamePoint go_position = game_point_make(floorf((screen_size.width - go_size.width) / 2),
                                          floorf((screen_size.height - go_size.height) / 2));
  go_.reset(new SimpleItem());
  go_->add_sprite(go_sprite);
  go_->set_position(go_position);

  // Add rink left and right pieces.
  Sprite left_rink_border_sprite(game_engine, "rink_left");
  SimpleItem *left_rink_border = new SimpleItem(left_rink_border_sprite,
                                                game_engine->position("rink_left"));
  AddEntity(left_rink_border);
  Sprite right_rink_border_sprite(game_engine, "rink_right");
  SimpleItem *right_rink_border = new SimpleItem(right_rink_border_sprite,
                                                 game_engine->position("rink_right"));
  AddEntity(right_rink_border);

  Sprite pause_button_sprite(game_engine, "pause_button");
  Sprite pause_button_pressed_sprite(game_engine, "pause_button_pressed");

  GamePoint pause_button_pos_1 =
      game_point_make(Rink::TotalWidth() - pause_button_sprite.content_size().width,
                      Rink::TotalHeight() - pause_button_sprite.content_size().height);
  pause_button_1_.reset(new Button());
  pause_button_1_->set_normal_sprite(pause_button_sprite);
  pause_button_1_->set_pressed_sprite(pause_button_pressed_sprite);
  pause_button_1_->set_position(pause_button_pos_1);
  pause_button_1_->set_delegate(this);
  AddEntity(pause_button_1_);

  if (num_players == 2) {
    // TODO: This is needed because of crappy texture management.
    Sprite pause_button_sprite_2(game_engine, "pause_button");
    Sprite pause_button_pressed_sprite_2(game_engine, "pause_button_pressed");

    GamePoint pause_button_pos_2 = game_point_make(0, 0);
    pause_button_2_.reset(new Button());
    pause_button_2_->set_normal_sprite(pause_button_sprite_2);
    pause_button_2_->set_pressed_sprite(pause_button_pressed_sprite_2);
    pause_button_2_->set_position(pause_button_pos_2);
    pause_button_2_->set_delegate(this);
    AddEntity(pause_button_2_);
  }

  give_extra_puck_to_player_ = kPlayerId1;
  player_1_win_count_ = 0;
  player_2_win_count_ = 0;
  SetUpNewGame();
}


// EngineView

void PlayView::Update() {
  if (state_ == kPlayViewStatePaused) {
    return;
  }

  EngineView::Update();

  if (state_ == kPlayViewStateGetReady) {
    get_ready_ticks_left_--;
    if (get_ready_ticks_left_ == kShowGetReadyMessageTicks) {
      GamePoint position = get_ready_->position();
      GamePoint start_position = game_point_make(position.x, position.y - 40);
      get_ready_->set_position(start_position);
      get_ready_->set_alpha(0);
      get_ready_->AnimateToPosition(position, kAnimationTypeLinear, kShowGetReadyMessageTicks);
      get_ready_->AnimateToAlpha(1, kAnimationTypeLinear, kShowGoMessageTicks);
      AddEntity(get_ready_);
      SoundPlayer::instance()->playSound(kSoundGetReady);
    } else if (get_ready_ticks_left_ == 0) {
      RemoveEntity(get_ready_);
      AddEntity(go_);
      go_->set_zoom(1);
      go_->set_alpha(1);
      go_->AnimateToZoom(5, kAnimationTypeCubicEaseOut, kShowGoMessageTicks);
      go_->AnimateToAlpha(0, kAnimationTypeLinear, kShowGoMessageTicks);
      go_ticks_left_ = kShowGoMessageTicks;
      state_ = kPlayViewStatePlaying;
      paddle_1_->SetReadyToPlay(true);
      paddle_2_->SetReadyToPlay(true);
      SoundPlayer::instance()->playSound(kSoundStart);
    }

    return;
  }

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
      // TODO make this less shitty.
      puck->set_active(false);
      float position = (puck->x() / Rink::TotalWidth() - 0.5) * 2;
      if (player_1_score_->sprite() < kWinScore && state_ == kPlayViewStatePlaying) {
        player_1_score_->set_sprite(player_1_score_->sprite() + 1);
      }
      if (player_1_score_->sprite() == kWinScore && state_ == kPlayViewStatePlaying) {
        SoundPlayer::instance()->setPosition(kSoundScoreFinal, position);
        SoundPlayer::instance()->playSound(kSoundScoreFinal);
      } else {
        SoundPlayer::instance()->setPosition(kSoundScore, position);
        SoundPlayer::instance()->playSound(kSoundScore);
      }
      num_player_1_scores_last_round_++;
      num_active_pucks_--;
    } else if (puck->y() > Rink::TotalHeight() + puck->radius()) {
      puck->set_active(false);
      float position = (puck->x() / Rink::TotalWidth() - 0.5) * 2;
      if (player_2_score_->sprite() < kWinScore && state_ == kPlayViewStatePlaying) {
        player_2_score_->set_sprite(player_2_score_->sprite() + 1);
      }
      if (player_2_score_->sprite() == kWinScore && state_ == kPlayViewStatePlaying) {
        SoundPlayer::instance()->setPosition(kSoundScoreFinal, position);
        SoundPlayer::instance()->playSound(kSoundScoreFinal);
      } else {
        SoundPlayer::instance()->setPosition(kSoundScore, position);
        SoundPlayer::instance()->playSound(kSoundScore);
      }

      num_active_pucks_--;
    }
  }

  switch (state_) {
    case kPlayViewStateShowingAd:
      if (!game_engine()->ad_engine()->IsShowingFullScreenAd()) {
        state_ = kPlayViewStateGetReady;
      }
      break;
    case kPlayViewStateGetReady:
      break;
    case kPlayViewStatePlaying: {
      if (player_1_score_->sprite() == kWinScore) {
        FinishGameWithWinner(kPlayerId1);
      } else if (player_2_score_->sprite() == kWinScore) {
        FinishGameWithWinner(kPlayerId2);
      } else if (num_active_pucks_ == 0) {
        wait_ticks_left_ = kWaitTicks;
        state_ = kPlayViewStateWaitingForPucks;
      }
      break;
    }
    case kPlayViewStateWaitingForPucks: {
      if (wait_ticks_left_-- == 0) {
        for (int i = 0; i < num_pucks_; i++) {
          Puck *puck = pucks_[i].get();
          puck->set_active(true);
          puck->PlaceForPlayer(i < num_player_1_scores_last_round_ ? kPlayerId2 : kPlayerId1,
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
  if (button == pause_button_1_.get() || button == pause_button_2_.get()) {
    PausePressed();
  } else {
    assert(false);
  }
}


// GameMenuViewDelegate

void PlayView::RematchPressed() {
  game_engine()->analytics_engine()->LogEvent("REMATCH");
  SetUpNewGame();
}

void PlayView::MenuPressed() {
  game_engine()->PopView();
  game_engine()->PushView(sp<EngineView>(new MainMenuView(game_engine())));
}

void PlayView::ContinuePressed() {
  state_ = pre_pause_state_;
}


// private

void PlayView::SetUpNewGame() {
  // Place paddles!
  paddle_1_->SetInitialPositionForPlayer(kPlayerId1);
  paddle_2_->SetInitialPositionForPlayer(kPlayerId2);
  paddle_1_->SetReadyToPlay(false);
  paddle_2_->SetReadyToPlay(false);

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
    PlayerId player_id;
    if (i % 2 == 0) {
      player_id = give_extra_puck_to_player_;
    } else {
      player_id = (give_extra_puck_to_player_ == kPlayerId1) ? kPlayerId2 : kPlayerId1;
    }
    bool center = !((player_id == give_extra_puck_to_player_ &&
                      (num_pucks_ == 3 || num_pucks_ == 4 || num_pucks_ == 7)) ||
                      (player_id == 1 - give_extra_puck_to_player_ &&
                      (num_pucks_ == 4 || num_pucks_ == 5)));
    puck->PlaceForPlayer(player_id, round_things_, center);
  }

  player_1_score_->set_sprite(0);
  player_2_score_->set_sprite(0);
  RemoveEntity(win_);
  RemoveEntity(lose_);

  num_active_pucks_ = num_pucks_;
  num_player_1_scores_last_round_ = 0;

  int num_matches = LocalStore::IntegerForKey(kLocalStoreMatchCount) + 1;
  LocalStore::SetInteger(num_matches, kLocalStoreMatchCount);
  bool show_full_screen_ad = num_matches % kFullScreenAdFrequency == 0;

  if (show_full_screen_ad && game_engine()->ad_engine()->ShowFullScreenAd()) {
    state_ = kPlayViewStateShowingAd;
  } else {
    state_ = kPlayViewStateGetReady;
  }
  get_ready_ticks_left_ = kGetReadyTicksTotal;
}

void PlayView::FinishGameWithWinner(PlayerId playerId) {
  state_ = kPlayViewStateFinished;

  double loseX = (Rink::TotalWidth() - lose_->size().width)/2;
  double winX =  (Rink::TotalWidth() - win_->size().width)/2;
  double topY = 70;
  double bottomY = Rink::TotalHeight() - topY - lose_->size().height;
  switch (playerId) {
    case kPlayerId1: {
      player_1_win_count_++;

      win_->set_position(game_point_make(winX, bottomY));
      win_->set_angle(0);
      AddEntity(win_);

      if (num_players_ == 2) {
        lose_->set_position(game_point_make(loseX, topY));
        lose_->set_angle(180);
        AddEntity(lose_);
      }

      give_extra_puck_to_player_ = kPlayerId2;

      break;
    }
    case kPlayerId2: {
      player_2_win_count_++;

      if (num_players_ == 2) {
        win_->set_position(game_point_make(winX, topY));
        win_->set_angle(180);
        AddEntity(win_);
      }

      lose_->set_position(game_point_make(loseX, bottomY));
      lose_->set_angle(0);
      AddEntity(lose_);

      give_extra_puck_to_player_ = kPlayerId1;

      break;
    }
  }
  win_->set_alpha(0);
  win_->set_zoom(4);
  win_->AnimateToAlpha(1, kAnimationTypeLinear, 2 * 60);
  win_->AnimateToZoom(1, kAnimationTypeBounceEaseOut, 2 * 60);
  lose_->set_alpha(0);
  lose_->set_zoom(4);
  lose_->AnimateToAlpha(1, kAnimationTypeLinear, 3 * 60);
  lose_->AnimateToZoom(1, kAnimationTypeBounceEaseOut, 3 * 60);

  game_engine()->PushView(sp<GameMenuView>(new GameMenuView(game_engine(), this, true)));
}

void PlayView::PausePressed() {
  if (state_ != kPlayViewStateFinished && state_ != kPlayViewStatePaused) {
    pre_pause_state_ = state_;
    state_ = kPlayViewStatePaused;
    game_engine()->PushView(sp<GameMenuView>(new GameMenuView(game_engine(), this, false)));
  }
}
