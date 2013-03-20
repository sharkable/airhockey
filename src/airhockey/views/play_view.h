//
//  play_view.h
//  AirHockey
//
//  Created by Jon Sharkey on 2010-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_VIEWS_PLAYVIEW_H_
#define AIRHOCKEY_VIEWS_PLAYVIEW_H_

#include <vector>

#include "gameengine/engine_view.h"
#include "gameengine/entities/button.h"

#include "airhockey/entities/paddle.h"
#include "airhockey/views/game_menu_view.h"

class GameEngine;
class Post;
class Puck;
class Rink;
class RoundThing;
class SimpleItem;

typedef enum {
  kPlayViewStateGetReady,
  kPlayViewStatePlaying,
  kPlayViewStateWaitingForPucks,
  kPlayViewStateFinished,
  kPlayViewStatePaused
} PlayViewState;

class PlayView : public EngineView, private ButtonDelegate, GameMenuViewDelegate {
 public:
  PlayView(sp<GameEngine> game_engine, int num_players, int num_pucks, ComputerAI difficulty,
           PaddleSize paddle_size);

  // EngineView
  void Update();

  // ButtonDelegate
  void ButtonPressed(Button *button);

  // GameMenuViewDelegate
  void RematchPressed();
  void MenuPressed();
  void ContinuePressed();

 private:
  void SetUpNewGame();
  void FinishGameWithWinner(PlayerId player_id);
  void PausePressed();

  int num_players_;
  int num_pucks_;
  int num_active_pucks_;
  int num_player_1_scores_last_round_;
  sp<Rink> rink_;
  sp<Post> post_1_;
  sp<Post> post_2_;
  sp<Post> post_3_;
  sp<Post> post_4_;
  sp<Paddle> paddle_1_;
  sp<Paddle> paddle_2_;
  std::vector<sp<Puck> > pucks_;
  std::vector<sp<RoundThing> > round_things_;
  sp<SimpleItem> player_1_score_;
  sp<SimpleItem> player_2_score_;
  sp<SimpleItem> win_;
  sp<SimpleItem> lose_;
  sp<SimpleItem> get_ready_;
  sp<SimpleItem> go_;
  sp<Button> pause_button_1_;
  sp<Button> pause_button_2_;

  int wait_ticks_left_;
  int get_ready_ticks_left_;
  int go_ticks_left_;
  PlayViewState state_;
  PlayViewState pre_pause_state_;
  PlayerId give_extra_puck_to_player_;
  int player_1_win_count_;
  int player_2_win_count_;
};

#endif
