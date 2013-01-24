//
//  sound_slider.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-06.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAME_ENTITIES_SOUNDSLIDER_H_
#define AIRHOCKEY_GAME_ENTITIES_SOUNDSLIDER_H_

#include "gameengine/view_entity.h"
#include "opengl/Texture2D.h"

class SoundSlider : public ViewEntity {
 public:
  SoundSlider(SGPoint position);

  SGPoint ThumbPoint();

  // ViewEntity
  void Update();
  void Render();
  void TouchesBegan(vector<Touch> touches);
  void TouchesMoved(vector<Touch> touches);
  void TouchesEnded(vector<Touch> touches);

 private:
  SGPoint position_;
  Texture2D empty_texture_;
  Texture2D full_texture_;
  Texture2D thumb_texture_;
  double value_;
  void *grabbed_touch_;
  SGPoint last_touch_position_;
};

#endif
