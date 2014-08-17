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

#include "sharkengine/shark_engine.h"
#include "sharksound/sound_controller.h"
#include "sharksound/sound.h"

#include "airhockey/entities/post.h"
#include "airhockey/entities/puck.h"
#include "airhockey/entities/rink.h"

using std::vector;

static const int kPuckFadeTicks = 10;
static const int kPuckXSeparation = 150;
static const double kPuckGoalMinDropSpeed = 5;
static const double kPuckMass = 10;
static const double kPuckFriction = 0.995;
static const double kPuckRadius = 43.5;

Puck::Puck(GameEngine &game_engine, Rink &rink) : RoundThing(game_engine, "puck"), rink_(rink) {
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

  two_puck_hit_sound_ = game_engine.sound()->GetSound("sounds/puck_puck_hit.wav");
  paddle_hit_sound_ = game_engine.sound()->GetSound("sounds/paddle_hit.wav");
  puck_rink_bounce_sound_ = game_engine.sound()->GetSound("sounds/puck_rink_bounce.wav");
}

void Puck::PlaceForPlayer(PlayerId player_id, const vector<RoundThing *> &round_things,
                          bool center) {
  double startX = rink_.CenterX();
  if (!center) {
    startX += kPuckXSeparation / 2;
  }
  x_ = startX;
  y_ = rink_.TotalHeight() / 3;
  if (player_id == kPlayerId1) {
    y_ = rink_.TotalHeight() - y_ - 1;
  }

  vx_ = 0;
  vy_ = 0;

  bool goLeft = true;
  int offset = 1;
  bool overlapping;
  do {
    overlapping = false;
    for (int i = 0; i < round_things.size(); i++) {
      RoundThing *thing = round_things[i];
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


#pragma mark - Simulator

void Puck::SimulateStep() {
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

  RoundThing::SimulateStep();

  // Stop the puck from getting stuck in the goal.
  if (y() < rink_.TopY() && fabs(vy()) < kPuckGoalMinDropSpeed) {
    set_vy(-kPuckGoalMinDropSpeed);
  } else if (y_ > rink_.BottomY() && fabs(vy_) < kPuckGoalMinDropSpeed) {
    vy_ = kPuckGoalMinDropSpeed;
  }

  if (fade_ticks_left_ > 0) {
    fade_ticks_left_--;
    alpha_ = ((double)kPuckFadeTicks - fade_ticks_left_) / kPuckFadeTicks;
  }
}


#pragma mark - Renderer

void Puck::Render(CoordinateSystem const &coordinate_system) {
  if (is_active()) {
    sprite_.set_alpha(alpha_);
    sprite_.Draw(coordinate_system.Translate(GamePoint(x_ - sprite_.content_size().width / 2,
                                                       y_ - sprite_.content_size().height / 2)));
  }
}


#pragma mark - RoundThing

void Puck::DidBounceOff(void *other, double total_velocity) {
  float volume = (float)(total_velocity / 50.0);
  if (volume > 1.8) volume = 1.8;
  else if (volume < 0.4) volume = 0.4;
  float position = (x_ / rink_.TotalWidth() - 0.5) * 2;
  if (typeid(*other) == typeid(Puck)) {
    if (!hit_puck_this_time_) {
      two_puck_hit_sound_->Play(volume, position);
      hit_puck_last_time_ = true;
    }
    hit_puck_this_time_ = true;
  } else if (typeid(*other) == typeid(Paddle)) {
    if (!hit_paddle_last_time_) {
      paddle_hit_sound_->Play(volume, position);
      hit_paddle_last_time_ = true;
    }
    hit_paddle_this_time_ = true;
  } else if (typeid(*other) == typeid(Post) || typeid(*other) == typeid(Rink)) {
    if (!hit_rink_last_time_) {
      puck_rink_bounce_sound_->Play(volume, position);
      hit_rink_last_time_ = true;
    }
    hit_rink_this_time_ = true;
  }
}
