//
//  sound_slider.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-06.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAME_ENTITIES_SOUNDSLIDER_H_
#define AIRHOCKEY_GAME_ENTITIES_SOUNDSLIDER_H_

#include "gameengine/sprite.h"
#include "gameengine/view_entity.h"

class SoundSlider : public ViewEntity {
 public:
  SoundSlider(sp<GameEngine> game_engine, GamePoint position);

  GamePoint ThumbPoint();

  // ViewEntity
  void Update();
  void Render();
  void TouchesBegan(vector<Touch> touches);
  void TouchesMoved(vector<Touch> touches);
  void TouchesEnded(vector<Touch> touches);

 private:
  GamePoint position_;
  Sprite empty_sprite_;
  Sprite full_sprite_;
  Sprite thumb_sprite_;
  double value_;
  void *grabbed_touch_;
  GamePoint last_touch_position_;
};

#endif