//
//  sound_slider.h
//  AirHockey
//
//  Created by Jon Sharkey on 2010-05-06.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_ENTITIES_SOUNDSLIDER_H_
#define AIRHOCKEY_ENTITIES_SOUNDSLIDER_H_

#include <vector>

#include "gameengine/coordinate_types.h"
#include "gameengine/sprite.h"
#include "gameengine/view_entity.h"

class GameEngine;
class Touch;

class SoundSlider : public ViewEntity {
 public:
  SoundSlider(sp<GameEngine> game_engine, GamePoint position);

  GamePoint ThumbPoint();

  // ViewEntity
  void Update();
  void Render();
  bool TouchesBegan(std::vector<Touch> touches);
  void TouchesMoved(std::vector<Touch> touches);
  void TouchesEnded(std::vector<Touch> touches);

 private:
  GamePoint position_;
  Sprite empty_sprite_;
  Sprite full_sprite_;
  Sprite thumb_sprite_;
  double value_;
  void *grabbed_touch_;
  GamePoint start_touch_position_;
  double start_value_;
};

#endif
