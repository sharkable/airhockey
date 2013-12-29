//
//  play_view.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2010-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "airhockey/views/play_view.h"

#include "gameengine/entities/simple_item.h"
#include "gameengine/modules/ad_module.h"
#include "gameengine/modules/analytics_module.h"
#include "gameengine/modules/input_module.h"
#include "gameengine/modules/persistence_module.h"
#include "gameengine/game_engine.h"
#include "gameengine/platform.h"
#include "gameengine/resource_loader.h"
#include "sharksound/sound.h"
#include "sharksound/sound_controller.h"

#include "airhockey/entities/post.h"
#include "airhockey/entities/puck.h"
#include "airhockey/entities/rink.h"
#include "airhockey/entities/rink_overlay.h"
#include "airhockey/entities/round_thing.h"
#include "airhockey/views/main_menu_view.h"
#include "airhockey/views/rink_view.h"

using std::string;
using std::vector;

static const int kWaitTicks = 60;
static const int kGetReadyTicksTotal = 120;
static const int kShowGetReadyMessageTicks = 90;
static const int kShowGoMessageTicks = 30;
static const int kWinScore = 7;
static const int kFullScreenAdFrequency = 3;
static const string kLocalStoreMatchCount = "ls_match_count";

PlayView::PlayView(GameEngine &game_engine, int num_players, int num_pucks, ComputerAI difficulty,
                   PaddleSize paddle_size)
    : game_engine_(game_engine),
      pause_button_1_(NULL),
      pause_button_2_(NULL) {
  num_players_ = num_players;

  rink_ = new Rink(RinkView::RinkSizeForTextureGroup(game_engine.platform().texture_group()));
// TODO NOW  AddRenderer(rink_);

  paddle_1_ = new Paddle(game_engine, *rink_, kPlayerId1, paddle_size, true, kComputerAIBad,
                         pucks_);
  paddle_2_ = new Paddle(game_engine, *rink_, kPlayerId2, paddle_size, num_players == 2, difficulty,
                         pucks_);

  post_1_ = new Post(game_engine, rink_->GoalLeftX(), rink_->TopY());
  post_2_ = new Post(game_engine, rink_->GoalLeftX(), rink_->BottomY() + 1);
  post_3_ = new Post(game_engine, rink_->GoalRightX() + 1, rink_->TopY());
  post_4_ = new Post(game_engine, rink_->GoalRightX() + 1, rink_->BottomY() + 1);

  vector<Sprite> scoreSprites;
  for (int i = 0; i <= kWinScore; i++) {
    char pointsstr[15];
    sprintf(pointsstr, "%d_points", i);
    Sprite sprite(game_engine, pointsstr);
    // TODO: This is just to bump the use count of the texture since it's being used twice.
    // Rethink how we handle textures.
    game_engine.resource_loader().TextureWithName(pointsstr);
    scoreSprites.push_back(sprite);
  }
  player_1_score_ = new SimpleItem();
  player_1_score_->set_sprites(scoreSprites);
  player_1_score_->set_position(game_engine.position("player_1_score"));
  player_2_score_ = new SimpleItem();
  player_2_score_->set_sprites(scoreSprites);
  player_2_score_->set_position(game_engine.position("player_2_score"));
  AddRenderer(player_1_score_);
  AddSimulator(player_1_score_);
  AddRenderer(player_2_score_);

  num_pucks_ = num_pucks;
  num_active_pucks_ = num_pucks_;
  for (int i = 0; i < num_pucks_; i++) {
    pucks_.push_back(new Puck(game_engine, *rink_));
    AddRenderer(pucks_[i]);
    AddSimulator(pucks_[i]);
    round_things_.push_back(pucks_[i]);
  }

  AddRenderer(paddle_1_);
  AddSimulator(paddle_1_);
  round_things_.push_back(paddle_1_);

  AddRenderer(paddle_2_);
  AddSimulator(paddle_2_);
  round_things_.push_back(paddle_2_);

  AddRenderer(post_1_);
  AddSimulator(post_1_);
  round_things_.push_back(post_1_);

  AddRenderer(post_2_);
  AddSimulator(post_2_);
  round_things_.push_back(post_2_);

  AddRenderer(post_3_);
  AddSimulator(post_3_);
  round_things_.push_back(post_3_);

  AddRenderer(post_4_);
  AddSimulator(post_4_);
  round_things_.push_back(post_4_);

//  RinkOverlay *rink_overlay = new RinkOverlay(game_engine);
//  AddEntity(rink_overlay, false);

  Sprite winSprite(game_engine, "win");
  win_ = new SimpleItem();
  win_->add_sprite(winSprite);
  win_->set_position(kGamePointZero);

  Sprite loseSprite(game_engine, "lose");
  lose_ = new SimpleItem();
  lose_->add_sprite(loseSprite);
  lose_->set_position(kGamePointZero);

  Sprite get_ready_sprite(game_engine, "get_ready");
  GameSize rink_size = GameSize(rink_->TotalWidth(), rink_->TotalHeight());
  GameSize get_ready_size = get_ready_sprite.content_size();
  GamePoint get_ready_position = GamePoint(floorf((rink_size.width - get_ready_size.width) / 2),
                                           floorf((rink_size.height - get_ready_size.height) / 2));
  get_ready_ = new SimpleItem();
  get_ready_->add_sprite(get_ready_sprite);
  get_ready_->set_position(get_ready_position);

  Sprite go_sprite(game_engine, "go");
  GameSize go_size = go_sprite.content_size();
  GamePoint go_position = GamePoint(floorf((rink_size.width - go_size.width) / 2),
                                    floorf((rink_size.height - go_size.height) / 2));
  go_ = new SimpleItem();
  go_->add_sprite(go_sprite);
  go_->set_position(go_position);

  // For the PC version, hide the mouse during play. Rely on the keyboard to pause, and have no
  // buttons.
  if (game_engine.platform().input_group() != Platform::kInputGroupPC) {
    Sprite pause_button_sprite(game_engine, "pause_button");
    Sprite pause_button_pressed_sprite(game_engine, "pause_button_pressed");

    GameSize game_size = game_engine.screen_size_to_game_size(game_engine.screen_size());
    double y_margin = (game_size.height - rink_->TotalHeight()) / 2;
    GamePoint pause_button_pos_1 =
        GamePoint(game_size.width - pause_button_sprite.content_size().width,
                  game_size.height - pause_button_sprite.content_size().height - y_margin);
    pause_button_1_ = new Button(game_engine);
    pause_button_1_->set_normal_sprite(pause_button_sprite);
    pause_button_1_->set_pressed_sprite(pause_button_pressed_sprite);
    pause_button_1_->set_position(pause_button_pos_1);
    pause_button_1_->set_delegate(this);
    AddRenderer(pause_button_1_);
    AddSimulator(pause_button_1_);

    if (num_players == 2) {
      // TODO: This is needed because of crappy texture management.
      Sprite pause_button_sprite_2(game_engine, "pause_button");
      Sprite pause_button_pressed_sprite_2(game_engine, "pause_button_pressed");

      GamePoint pause_button_pos_2 = GamePoint(0, -y_margin);
      pause_button_2_ = new Button(game_engine);
      pause_button_2_->set_normal_sprite(pause_button_sprite_2);
      pause_button_2_->set_pressed_sprite(pause_button_pressed_sprite_2);
      pause_button_2_->set_position(pause_button_pos_2);
      pause_button_2_->set_delegate(this);
      AddRenderer(pause_button_2_);
      AddSimulator(pause_button_2_);
    }
  }

  get_ready_sound_ = game_engine.sound()->GetSound("sounds/get_ready.wav");
  go_sound_ = game_engine.sound()->GetSound("sounds/start.wav");
  score_sound_ = game_engine.sound()->GetSound("sounds/score.wav");
  score_final_sound_ = game_engine.sound()->GetSound("sounds/score_final.wav");

  give_extra_puck_to_player_ = kPlayerId1;
  player_1_win_count_ = 0;
  player_2_win_count_ = 0;
  SetUpNewGame();
}

PlayView::~PlayView() {
  delete rink_;
  delete post_1_;
  delete post_2_;
  delete post_3_;
  delete post_4_;
  delete paddle_1_;
  delete paddle_2_;
  delete player_1_score_;
  delete player_2_score_;
  delete win_;
  delete lose_;
  delete get_ready_;
  delete go_;
  if (pause_button_1_) {
    delete pause_button_1_;
  }
  if (pause_button_2_) {
    delete pause_button_2_;
  }
}

// TODO NOW
//void PlayView::ViewDidGainFocus() {
//  game_engine().input_module()->HidePointer();
//}
//
//void PlayView::ViewDidLoseFocus() {
//  game_engine().input_module()->ShowPointer();
//}


#pragma mark - GroupSimulator

void PlayView::SimulateStep() {
  if (state_ == kPlayViewStatePaused) {
    return;
  }

  GroupSimulator::SimulateStep();

  if (state_ == kPlayViewStateGetReady) {
    get_ready_ticks_left_--;
    if (get_ready_ticks_left_ == kShowGetReadyMessageTicks) {
      GamePoint position = get_ready_->position();
      GamePoint start_position(position.x, position.y - 40);
      get_ready_->set_position(start_position);
      get_ready_->set_alpha(0);
      get_ready_->AnimateToPosition(position, kAnimationTypeLinear, kShowGetReadyMessageTicks);
      get_ready_->AnimateToAlpha(1, kAnimationTypeLinear, kShowGoMessageTicks);
      AddRenderer(get_ready_);
      AddSimulator(get_ready_);
      get_ready_sound_->Play();
    } else if (get_ready_ticks_left_ == 0) {
      RemoveSimulator(get_ready_);
      RemoveRenderer(get_ready_);
      AddSimulator(go_);
      AddRenderer(go_);
      go_->set_zoom(1);
      go_->set_alpha(1);
      go_->AnimateToZoom(5, kAnimationTypeCubicEaseOut, kShowGoMessageTicks);
      go_->AnimateToAlpha(0, kAnimationTypeLinear, kShowGoMessageTicks);
      go_ticks_left_ = kShowGoMessageTicks;
      state_ = kPlayViewStatePlaying;
      paddle_1_->SetReadyToPlay(true);
      paddle_2_->SetReadyToPlay(true);
      go_sound_->Play();
    }

    return;
  }

  if (go_ticks_left_ > 0) {
    go_ticks_left_--;
    if (go_ticks_left_ == 0) {
      RemoveSimulator(go_);
      RemoveRenderer(go_);
    }
  }

  paddle_1_->KeepInPlayerBounds();
  paddle_2_->KeepInPlayerBounds();

  for (int i = 0; i < round_things_.size(); i++) {
    RoundThing *thing = round_things_[i];
    if (!thing->is_active()) {
      continue;
    }
    rink_->BounceOff(thing);
    for (int j = i + 1; j < round_things_.size(); j++) {
      RoundThing *otherThing = round_things_[j];
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
    Puck *puck = pucks_[i];
    if (!puck->is_active()) {
      continue;
    }
    if (puck->y() < -puck->radius()) {
      // TODO make this less shitty.
      puck->set_active(false);
      float position = (puck->x() / rink_->TotalWidth() - 0.5) * 2;
      if (player_1_score_->sprite() < kWinScore && state_ == kPlayViewStatePlaying) {
        player_1_score_->set_sprite(player_1_score_->sprite() + 1);
      }
      if (player_1_score_->sprite() == kWinScore && state_ == kPlayViewStatePlaying) {
        score_final_sound_->Play(1.f, position);
      } else {
        score_sound_->Play(1.f, position);
      }
      num_player_1_scores_last_round_++;
      num_active_pucks_--;
    } else if (puck->y() > rink_->TotalHeight() + puck->radius()) {
      puck->set_active(false);
      float position = (puck->x() / rink_->TotalWidth() - 0.5) * 2;
      if (player_2_score_->sprite() < kWinScore && state_ == kPlayViewStatePlaying) {
        player_2_score_->set_sprite(player_2_score_->sprite() + 1);
      }
      if (player_2_score_->sprite() == kWinScore && state_ == kPlayViewStatePlaying) {
        score_final_sound_->Play(1.f, position);
      } else {
        score_sound_->Play(1.f, position);
      }

      num_active_pucks_--;
    }
  }

  switch (state_) {
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
          Puck *puck = pucks_[i];
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


#pragma mark - InputHandler

bool PlayView::HandleEvent(InputEvent const &event) {
  paddle_1_->HandleEvent(event);
  if (event.IsKey()) {
    PausePressed();
  }
  return true;
}

// TODO NOW
//void PlayView::NotifyPause() {
//  PausePressed();
//}
//
//bool PlayView::HandleBackButton() {
//  PausePressed();
//  return true;
//}
//
// TODO NOW
//void PlayView::HandlePauseButton() {
//  PausePressed();
//}
//
//void PlayView::KeysPressed(std::vector<int> const &keys) {
//  PausePressed();
//}


#pragma mark - ButtonDelegate

void PlayView::ButtonUp(Button *button) {
  if (button == pause_button_1_ || button == pause_button_2_) {
    PausePressed();
  } else {
    assert(false);
  }
}


#pragma mark - GameMenuViewDelegate

void PlayView::RematchPressed() {
  game_engine_.analytics_module()->LogEvent("REMATCH");
  SetUpNewGame();
}

void PlayView::MenuPressed() {
  game_engine_.PopView();
// TODO NOW  game_engine().PushView(new MainMenuView(game_engine()));
}

void PlayView::ContinuePressed() {
  state_ = pre_pause_state_;
}


#pragma mark - private

void PlayView::SetUpNewGame() {
  // Place paddles!
  paddle_1_->SetInitialPosition();
  paddle_2_->SetInitialPosition();
  paddle_1_->SetReadyToPlay(false);
  paddle_2_->SetReadyToPlay(false);

  // Place pucks!
  // First move them all out of the way. That way we can lay them out properly.
  // ([Puck placeForPlayer] avoids hitting other RoundThings objects.)
  for (int i = 0; i < num_pucks_; i++) {
    Puck *puck = pucks_[i];
    puck->set_x(0);
    puck->set_y(0);
  }
  for (int i = 0; i < num_pucks_; i++) {
    Puck *puck = pucks_[i];
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
  RemoveSimulator(win_);
  RemoveRenderer(win_);
  RemoveSimulator(lose_);
  RemoveRenderer(lose_);

  num_active_pucks_ = num_pucks_;
  num_player_1_scores_last_round_ = 0;

  bool app_upgraded = game_engine_.persistence_module()->BoolForKey(kLocalStoreUpgraded);
  int num_matches = game_engine_.persistence_module()->IntegerForKey(kLocalStoreMatchCount) + 1;
  game_engine_.persistence_module()->SetInteger(num_matches, kLocalStoreMatchCount);
  bool show_full_screen_ad = !app_upgraded && (num_matches % kFullScreenAdFrequency == 0);

  if (show_full_screen_ad) {
    game_engine_.ad_module()->ShowFullScreenAd();
  }
  state_ = kPlayViewStateGetReady;
  get_ready_ticks_left_ = kGetReadyTicksTotal;
}

void PlayView::FinishGameWithWinner(PlayerId playerId) {
  state_ = kPlayViewStateFinished;

  double loseX = (rink_->TotalWidth() - lose_->size().width)/2;
  double winX =  (rink_->TotalWidth() - win_->size().width)/2;
  double topY = 70;
  double bottomY = rink_->TotalHeight() - topY - lose_->size().height;
  switch (playerId) {
    case kPlayerId1: {
      player_1_win_count_++;

      win_->set_position(GamePoint(winX, bottomY));
      win_->set_angle(0);
      AddSimulator(win_);
      AddRenderer(win_);

      if (num_players_ == 2) {
        lose_->set_position(GamePoint(loseX, topY));
        lose_->set_angle(180);
        AddSimulator(lose_);
        AddRenderer(lose_);
      }

      give_extra_puck_to_player_ = kPlayerId2;

      break;
    }
    case kPlayerId2: {
      player_2_win_count_++;

      if (num_players_ == 2) {
        win_->set_position(GamePoint(winX, topY));
        win_->set_angle(180);
        AddSimulator(win_);
        AddRenderer(win_);
      }

      lose_->set_position(GamePoint(loseX, bottomY));
      lose_->set_angle(0);
      AddSimulator(lose_);
      AddRenderer(lose_);

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

  game_engine_.PushView(new GameMenuView(game_engine_, this, true));
}

void PlayView::PausePressed() {
  if (state_ != kPlayViewStateFinished && state_ != kPlayViewStatePaused) {
    pre_pause_state_ = state_;
    state_ = kPlayViewStatePaused;
    game_engine_.PushView(new GameMenuView(game_engine_, this, false));
  }
}
