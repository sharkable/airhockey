//
//  puck.cc
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "Puck.h"

#include <cmath>
#include <vector>
using namespace std;

#include "const.h"
#include "gameengine/resource_loader.h"
#include "soundengine/sound_player.h"

#include "airhockey/entities/paddle.h"
#include "airhockey/entities/post.h"
#include "airhockey/entities/puck.h"
#include "airhockey/entities/rink.h"

Puck::Puck(sp<GameEngine> game_engine) : RoundThing(game_engine, "puck") {
  radius_ = PUCK_RADIUS;
  mass_ = PUCK_MASS;
  friction_ = PUCK_FRICTION;
  alpha_ = 1;
  fade_ticks_left_ = 0;
  hit_puck_last_time_ = false;
  hit_puck_this_time_ = false;
  hit_paddle_last_time_ = false;
  hit_paddle_this_time_ = false;
  hit_rink_last_time_ = false;
  hit_rink_this_time_ = false;
}

void Puck::PlaceForPlayer(int player_id, const vector<sp<RoundThing> > &round_things, bool center) {
  double startX = SCREEN_WIDTH / 2;
  if (!center) {
    startX += PUCK_X_SEPARATION / 2;
  }
  x_ = startX;
  y_ = player_id == PLAYER_1 ? PLAYER_1_PUCK_Y : PLAYER_2_PUCK_Y;
  
  vx_ = 0;
  vy_ = 0;
  
  bool goLeft = true;
  int offset = 1;
  bool overlapping;
  do {
    overlapping = false;
    for (int i = 0; i < round_things.size(); i++) {
      RoundThing *thing = round_things[i].get();
      if (thing != this && Overlaps(thing)) {
        overlapping = true;
        if (goLeft) {
          x_ = startX - offset * PUCK_X_SEPARATION;
          goLeft = false;
        } else {
          x_ = startX + offset * PUCK_X_SEPARATION;
          offset++;
          goLeft = true;
        }
        break;
      }
    }
  } while (overlapping);
}

void Puck::FadeIn() {
  fade_ticks_left_ = PUCK_FADE_TICKS;
  alpha_ = 0;
}


// ViewEntity

void Puck::Update() {
  if (!hit_puck_this_time_) {
    hit_puck_last_time_ = false;
  }
  hit_puck_this_time_ = false;

  if (!hit_paddle_this_time_) {
    hit_paddle_last_time_ = false;
  }
  hit_paddle_this_time_ = false;

  if (!hit_rink_this_time_) {
    hit_rink_last_time_ = false;
  }
  hit_rink_this_time_ = false;

  RoundThing::Update();

  // Stop the puck from getting stuck in the goal.
  if (y() < RINK_TOP_Y && fabs(vy()) < PUCK_GOAL_MIN_DROP_SPEED) {
    set_vy(-PUCK_GOAL_MIN_DROP_SPEED);
  } else if (y_ > RINK_BOTTOM_Y && fabs(vy_) < PUCK_GOAL_MIN_DROP_SPEED) {
    vy_ = PUCK_GOAL_MIN_DROP_SPEED;
  }
  
  if (fade_ticks_left_ > 0) {
    fade_ticks_left_--;
    alpha_ = ((double)PUCK_FADE_TICKS - fade_ticks_left_) / PUCK_FADE_TICKS;
  }
}

void Puck::Render() {
  if (is_active()) {
    sprite_.Draw(game_point_make(x_ - sprite_.content_size().width / 2,
                                 y_ - sprite_.content_size().height / 2),
                 0, alpha_, 1);
  }
}


// RoundThing

void Puck::DidBounceOff(ViewEntity *other, double total_velocity) {
  float volume = (float)(total_velocity / 50.0);
  if (volume > 1.8) volume = 1.8;
  else if (volume < 0.4) volume = 0.4;
  float position = (x_ / SCREEN_WIDTH - 0.5) * 2;
  if (typeid(*other) == typeid(Puck)) {
    if (!hit_puck_this_time_) {
      SoundPlayer::instance()->setVolume(kSoundTwoPuckHit, volume);
      SoundPlayer::instance()->setPosition(kSoundTwoPuckHit, position);
      SoundPlayer::instance()->playSound(kSoundTwoPuckHit);
      hit_puck_last_time_ = true;
    }
    hit_puck_this_time_ = true;
  } else if (typeid(*other) == typeid(Paddle)) {
    if (!hit_paddle_last_time_) {
      SoundPlayer::instance()->setVolume(kSoundPaddleHit, volume);
      SoundPlayer::instance()->setPosition(kSoundPaddleHit, position);
      SoundPlayer::instance()->playSound(kSoundPaddleHit);
      hit_paddle_last_time_ = true;
    }
    hit_paddle_this_time_ = true;
  } else if (typeid(*other) == typeid(Post) || typeid(*other) == typeid(Rink)) {
    if (!hit_rink_last_time_) {
      SoundPlayer::instance()->setVolume(kSoundPuckRinkBounce, volume);
      SoundPlayer::instance()->setPosition(kSoundPuckRinkBounce, position);
      SoundPlayer::instance()->playSound(kSoundPuckRinkBounce);
      hit_rink_last_time_ = true;
    }
    hit_rink_this_time_ = true;
  }
}
