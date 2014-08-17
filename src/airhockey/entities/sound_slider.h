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

#include "sharkengine/sprite.h"
#include "sharkengine/coordinates/coordinate_types.h"
#include "sharkengine/input/input_handler.h"
#include "sharkengine/render/renderer.h"

class GameEngine;
class Touch;

class SoundSlider : public Renderer, public InputHandler {
 public:
  SoundSlider(GameEngine &game_engine, GamePoint position);

  GamePoint ThumbPoint();

  // Renderer
  void Render(CoordinateSystem const &coordinate_system);

  // InputHandler
  virtual bool HandleInputEvent(InputEvent const &event, CoordinateSystem const &coordinate_system);

 private:
  GameEngine &game_engine_;
  GamePoint position_;
  Sprite empty_sprite_;
  Sprite full_sprite_;
  Sprite thumb_sprite_;
  int left_marin_;
  int slider_width_;
  double value_;
  int grabbed_touch_;
  GamePoint start_touch_position_;
  double start_value_;
};

#endif
