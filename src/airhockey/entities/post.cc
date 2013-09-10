//
//  post.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2010-04-25.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "airhockey/entities/post.h"

#include "gameengine/game_engine.h"
#include "gameengine/view_entity.h"

#include "airhockey/entities/puck.h"

Post::Post(GameEngine *game_engine, double x, double y) : RoundThing(game_engine) {
  set_x(x);
  set_y(y);
  set_radius(1);
  set_active(true);
  set_mass(99999999999.99);
}


#pragma mark - RoundThing

void Post::DidBounceOff(ViewEntity *other, double total_velocity) {
  if (typeid(*other) == typeid(Puck)) {
    ((Puck *)other)->DidBounceOff(this, total_velocity);
  }
}

bool Post::IsMovable() {
  return false;
}


#pragma mark - ViewEntity

void Post::Render(GamePoint offset) {
}
