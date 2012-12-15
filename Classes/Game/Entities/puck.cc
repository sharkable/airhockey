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

Puck::Puck() {
  texture_ = ResourceLoader::Instance().TextureWithName("puck");
  radius_ = PUCK_RADIUS;
  mass_ = PUCK_MASS;
  friction_ = PUCK_FRICTION;
  alpha_ = 1;
}

void Puck::PlaceForPlayer(int player_id, const vector<RoundThing *> &round_things, bool center) {
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
      RoundThing *thing = round_things[i];
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


// StateEntity

void Puck::Update() {
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
  cout << "Is active? " << is_active() << endl;
  if (is_active()) {
    texture_.DrawAtPoint(SGPointMake(x_ - texture_.content_size().width / 2,
                                     y_ - texture_.content_size().height / 2),
                         alpha_, 1, 0, 0);
  }
}
