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

#include "sharkengine/entities/button.h"
#include "sharkengine/input/input_handler.h"
#include "sharkengine/render/group_renderer.h"
#include "sharkengine/simulation/group_simulator.h"

#include "airhockey/entities/paddle.h"
#include "airhockey/views/game_menu_view.h"

class GameView;
class Post;
class Puck;
class Rink;
class RoundThing;
class SharkEngine;
class SimpleItem;
namespace SharkSound {
  class Sound;
}

typedef enum {
  kPlayViewStateGetReady,
  kPlayViewStatePlaying,
  kPlayViewStateWaitingForPucks,
  kPlayViewStateFinished,
  kPlayViewStatePaused
} PlayViewState;

class PlayView : public GroupSimulator, public GroupRenderer, public InputHandler,
    private ButtonDelegate, GameMenuViewDelegate {
 public:
  PlayView(SharkEngine &shark_engine, GameView &game_view, int num_players, int num_pucks,
           ComputerAI difficulty, PaddleSize paddle_size);
  ~PlayView();

  // EngineView
//  void NotifyPause();
//  bool HandleBackButton();
//  void HandlePauseButton();
//  void KeysPressed(std::vector<int> const &keys);

  // GroupSimulator
  virtual void SimulateStep();

  // InputHandler
  bool HandleInputEvent(InputEvent const &event, CoordinateSystem const &coordinate_system);

  // ButtonDelegate
  void ButtonUp(Button *button);

  // GameMenuViewDelegate
  void RematchPressed();
  void MenuPressed();
  void ContinuePressed();

 private:
  void SetUpNewGame();
  void FinishGameWithWinner(PlayerId player_id);
  void PausePressed();

  SharkEngine &shark_engine_;
  GameView &game_view_;

  GameMenuView *game_menu_view_;

  int num_players_;
  int num_pucks_;
  int num_active_pucks_;
  int num_player_1_scores_last_round_;
  Rink *rink_;
  Post *post_1_;
  Post *post_2_;
  Post *post_3_;
  Post *post_4_;
  Paddle *paddle_1_;
  Paddle *paddle_2_;
  std::vector<Puck *> pucks_;
  std::vector<RoundThing *> round_things_;
  SimpleItem *player_1_score_;
  SimpleItem *player_2_score_;
  SimpleItem *win_;
  SimpleItem *lose_;
  SimpleItem *get_ready_;
  SimpleItem *go_;
  Button *pause_button_1_;
  Button *pause_button_2_;

  SharkSound::Sound *get_ready_sound_;
  SharkSound::Sound *go_sound_;
  SharkSound::Sound *score_sound_;
  SharkSound::Sound *score_final_sound_;

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
