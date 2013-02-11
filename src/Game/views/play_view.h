//
//  play_view.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAME_VIEWS_PLAYVIEW_H_
#define AIRHOCKEY_GAME_VIEWS_PLAYVIEW_H_

#include <vector>
using namespace std;

#include "const.h"
#include "gameengine/engine_view.h"
#include "gameengine/entities/button.h"
#include "gameengine/entities/simple_item.h"

#include "game/entities/Paddle.h"
#include "game/entities/Post.h"
#include "game/entities/puck.h"
#include "game/entities/Rink.h"
#include "game/entities/sound_slider.h"

class GameEngine;
class RoundThing;

typedef enum {
  kPlayViewStateGetReady,
  kPlayViewStatePlaying,
  kPlayViewStateWaitingForPucks,
  kPlayViewStateFinished,
  kPlayViewStatePaused
} PlayViewState;

class PlayView : public EngineView, private ButtonDelegate {
 public:
  PlayView(sp<GameEngine> game_engine, int num_players, int num_pucks, ComputerAI difficulty,
           PaddleSize paddle_size);
  
  // EngineView
  void Update();

  // ButtonDelegate
  void ButtonPressed(Button *button);
  
 private:
  void SetUpNewGame();
  void FinishGameWithWinner(int player_id);
  void RematchPressed();
  void MenuPressed();
  void ContinuePressed();
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
  vector<sp<Puck> > pucks_;
  vector<sp<RoundThing> > round_things_;
  sp<SimpleItem> player_1_score_;
  sp<SimpleItem> player_2_score_;
  sp<SimpleItem> win_;
  sp<SimpleItem> lose_;
  sp<SimpleItem> get_ready_;
  sp<SimpleItem> go_;
  sp<SoundSlider> sound_slider_;
  sp<SimpleItem> menu_background_;
  sp<Button> rematch_button_;
  sp<Button> menu_button_;
  sp<Button> continue_button_;
  sp<Button> pause_button_1_;
  sp<Button> pause_button_2_;
  
  int wait_ticks_left_;
  int get_ready_ticks_left_;
  int go_ticks_left_;
  int state_;
  int pre_pause_state_;
  int give_extra_puck_to_player_;
  int player_1_win_count_;
  int player_2_win_count_;
  
// TODO UILabel *player1Wins_;
//  UILabel *player2Wins_;
};

#endif
