//
//  puck.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2010-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "Puck.h"

#include <cmath>
#include <vector>

#include "gameengine/game_engine.h"
#include "soundengine/sound_player.h"

#include "airhockey/entities/post.h"
#include "airhockey/entities/puck.h"
#include "airhockey/entities/rink.h"
#include "airhockey/const.h"

using std::vector;

static const int kPuckFadeTicks = 10;
static const int kPlayer2PuckY = (SCREEN_HEIGHT / 3);
static const int kPlayer1PuckY = (SCREEN_HEIGHT - kPlayer2PuckY - 1);
static const int kPuckXSeparation = 150;
static const double kPuckGoalMinDropSpeed = 5;
static const double kPuckMass = 10;
static const double kPuckFriction = 0.995;
static const double kPuckRadius = 43.5;

Puck::Puck(sp<GameEngine> game_engine) : RoundThing(game_engine, "puck") {
  radius_ = kPuckRadius;
  mass_ = kPuckMass;
  friction_ = kPuckFriction;
  alpha_ = 1;
  fade_ticks_left_ = 0;
  hit_puck_last_time_ = false;
  hit_puck_this_time_ = false;
  hit_paddle_last_time_ = false;
  hit_paddle_this_time_ = false;
  hit_rink_last_time_ = false;
  hit_rink_this_time_ = false;
}

void Puck::PlaceForPlayer(PlayerId player_id, const vector<sp<RoundThing> > &round_things,
                          bool center) {
  double startX = SCREEN_WIDTH / 2;
  if (!center) {
    startX += kPuckXSeparation / 2;
  }
  x_ = startX;
  y_ = player_id == kPlayerId1 ? kPlayer1PuckY : kPlayer2PuckY;

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
          x_ = startX - offset * kPuckXSeparation;
          goLeft = false;
        } else {
          x_ = startX + offset * kPuckXSeparation;
          offset++;
          goLeft = true;
        }
        break;
      }
    }
  } while (overlapping);
}

void Puck::FadeIn() {
  fade_ticks_left_ = kPuckFadeTicks;
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
  if (y() < Rink::kRinkTopY && fabs(vy()) < kPuckGoalMinDropSpeed) {
    set_vy(-kPuckGoalMinDropSpeed);
  } else if (y_ > Rink::kRinkBottomY && fabs(vy_) < kPuckGoalMinDropSpeed) {
    vy_ = kPuckGoalMinDropSpeed;
  }

  if (fade_ticks_left_ > 0) {
    fade_ticks_left_--;
    alpha_ = ((double)kPuckFadeTicks - fade_ticks_left_) / kPuckFadeTicks;
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
