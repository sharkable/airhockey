//
//  PlayState.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAME_STATES_PLAYSTATE_H_
#define AIRHOCKEY_GAME_STATES_PLAYSTATE_H_

#import "const.h"
#import "game/entities/Paddle.h"
#import "game/entities/Puck.h"
#import "game/entities/Rink.h"
#import "game/entities/SoundSlider.h"
#import "gameengine/entities/button.h"
#import "gameengine/entities/simple_item.h"
#import "gameengine/engine_state.h"

#include <vector>
using namespace std;

typedef enum {
  kPlayStateStateGetReady,
  kPlayStateStatePlaying,
  kPlayStateStateWaitingForPucks,
  kPlayStateStateFinished,
  kPlayStateStatePaused
} PlayStateState;

class PlayState : public EngineState, private ButtonDelegate {
 public:
  PlayState(GameEngine &game_engine, int num_players, int num_pucks, ComputerAI difficulty,
            PaddleSize paddle_size);
  ~PlayState();
  
  // EngineState
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
  Rink rink_;
  Paddle paddle_1_;
  Paddle paddle_2_;
  vector<Puck *> pucks_;
  vector<RoundThing *> round_things_;
  SimpleItem player_1_score_;
  SimpleItem player_2_score_;
  SimpleItem win_;
  SimpleItem lose_;
  SimpleItem get_ready_;
  SimpleItem go_;
  SoundSlider sound_slider_;
  SimpleItem menu_background_;
  Button rematch_button_;
  Button menu_button_;
  Button continue_button_;
  Button pause_button_1_;
  Button pause_button_2_;
  
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
