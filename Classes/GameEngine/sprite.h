//
//  sprite.h
//  AirHockey
//
//  Created by Jon Sharkey on 1/27/13.
//
//

#ifndef AIRHOCKEY_GAMEENGINE_SPRITE_H_
#define AIRHOCKEY_GAMEENGINE_SPRITE_H_

#include "opengl/texture2D.h"

#include "gameengine/game_engine.c"

class Sprite {
 public:
  Sprite(GameEngine &game_engine, Texture2D texture)
    : game_engine_(game_engine),
      texture_(texture) {
  }

 private:
  GameEngine &game_engine_;
  Texture2D texture_;
}

#endif
