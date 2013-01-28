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

#include "gameengine/game_engine.h"
#include "gameengine/resource_loader.h"

class Sprite {
 public:
  Sprite() { }

  Sprite(sp<GameEngine> game_engine, Texture2D texture)
    : game_engine_(game_engine),
      texture_(texture) {
  }

  Sprite(sp<GameEngine> game_engine, string texture_name) : game_engine_(game_engine) {
    texture_ = ResourceLoader::Instance().TextureWithName(texture_name);
  }

  // TODO consider removing this.
  Texture2D texture() { return texture_; }

  GameSize content_size() {
    return game_engine_->screen_size_to_game_size(texture_.content_size());
  }

  void DrawAtPoint(GamePoint position) {
    texture_.DrawAtPoint(game_engine_->game_point_to_screen_point(position));
  }

  void DrawAtPointAngle(GamePoint position, GLfloat angle) {
    texture_.DrawAtPointAngle(game_engine_->game_point_to_screen_point(position), angle);
  }

 private:
  sp<GameEngine> game_engine_;
  Texture2D texture_;
};

#endif