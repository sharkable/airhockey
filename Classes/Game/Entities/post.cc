//
//  post.cc
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-25.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "game/entities/post.h"

#include "soundengine/sound_player.h"

#include "game/entities/rink.h"

Post::Post(double x, double y) {
  set_x(x);
  set_y(y);
  set_radius(1);
  set_mass(99999999999.99);  
}


// RoundThing

void Post::DidBounceOff(ViewEntity *other) {
  if (other->Name() == "Puck") {
    SoundPlayer::instance()->playSound(kSoundPuckRinkBounce);
  }
}

bool Post::IsMovable() {
  return false;
}
