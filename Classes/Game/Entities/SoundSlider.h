//
//  SoundSlider.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-06.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AirHockey_SoundSlider_h
#define AirHockey_SoundSlider_h

#include "gameengine/state_entity.h"
#import "Texture2D.h"

class SoundSlider : public StateEntity {
 private:
  SGPoint    position_;
  Texture2D emptyTexture_;
  Texture2D fullTexture_;
  Texture2D thumbTexture_;
  double     value_;
  void *grabbedTouch_;
  SGPoint    lastTouchPosition_;

 public:
  SoundSlider(SGPoint position);
  void Update();
  void Render();
  void TouchesBegan(vector<Touch> touches);
  void TouchesMoved(vector<Touch> touches);
  void TouchesEnded(vector<Touch> touches);
  SGPoint getThumbPoint();
};

#endif
