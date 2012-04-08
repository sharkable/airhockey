//
//  Puck.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "Puck.h"
#import "const.h"

#include <cmath>

#include <vector>
using namespace std;

Puck::Puck() {
  texture_ = ResourceLoader::instance()->getTextureWithName("puck");
  radius_ = PUCK_RADIUS;
  mass_ = PUCK_MASS;
  friction_ = PUCK_FRICTION;
  alpha_ = 1;
}

void Puck::update() {
  RoundThing::update();
  
  // Stop the puck from getting stuck in the goal.
  if (getY() < RINK_TOP_Y && fabs(getVY()) < PUCK_GOAL_MIN_DROP_SPEED) {
    setVY(-PUCK_GOAL_MIN_DROP_SPEED);
  } else if (y_ > RINK_BOTTOM_Y && fabs(vy_) < PUCK_GOAL_MIN_DROP_SPEED) {
    vy_ = PUCK_GOAL_MIN_DROP_SPEED;
  }
  
  if (fadeTicksLeft_ > 0) {
    fadeTicksLeft_--;
    alpha_ = ((double)PUCK_FADE_TICKS - fadeTicksLeft_) / PUCK_FADE_TICKS;
  }
}

void Puck::render() {
  if (isActive()) {
    texture_.drawAtPoint(SGPointMake(x_ - texture_.contentSize().width/2, y_ - texture_.contentSize().height/2),
                         alpha_, 1, 0, 0);
  }
}

void Puck::placeForPlayer(int playerId, const vector<RoundThing *> &roundThings, bool center) {
  double startX = SCREEN_WIDTH / 2;
  if (!center) {
    startX += PUCK_X_SEPARATION / 2;
  }
  x_ = startX;
  y_ = playerId == PLAYER_1 ? PLAYER_1_PUCK_Y : PLAYER_2_PUCK_Y;

  vx_ = 0;
  vy_ = 0;
  
  bool goLeft = true;
  int offset = 1;
  bool overlapping;
  do {
    overlapping = false;
    for (int i = 0; i < roundThings.size(); i++) {
      RoundThing *thing = roundThings[i];
      if (thing != this && overlaps(thing)) {
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

void Puck::fadeIn() {
  fadeTicksLeft_ = PUCK_FADE_TICKS;
  alpha_ = 0;
}
