//
//  SoundSlider.cc
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-06.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "SoundSlider.h"
#import "ResourceLoader.h"
#import "gameengine/local_store.h"
#include "Touch.h"
#import "const.h"

SoundSlider::SoundSlider(SGPoint position) {
  position_ = position;
  
  empty_texture_ = ResourceLoader::Instance().TextureWithName("sound_empty");
  full_texture_ = ResourceLoader::Instance().TextureWithName("sound_full");
  thumb_texture_ = ResourceLoader::Instance().TextureWithName("sound_thumb");
  
  if (LocalStore::HasEntryForKey(LS_VOLUME)) {
    value_ = LocalStore::DoubleForKey(LS_VOLUME);
  } else {
    value_ = 0.75;
  }
  // TODO [SoundPlayer setGlobalVolume:value_];
}

SGPoint SoundSlider::ThumbPoint() {
  return SGPointMake(position_.x + 19 + (269.0 - thumb_texture_.contentSize().width)*value_, position_.y);
}


// StateEntity

void SoundSlider::Update() {
}

void SoundSlider::Render() {
  double drawRatio = (269.0 - thumb_texture_.contentSize().width)/320.0 * value_ + (19.0 + thumb_texture_.contentSize().width/2)/320.0;
  full_texture_.drawAtPointLeftRatio(position_, drawRatio);
  empty_texture_.drawAtPointRightRatio(position_, 1.0 - drawRatio);
  thumb_texture_.drawAtPoint(ThumbPoint());
}

void SoundSlider::TouchesBegan(vector<Touch> touches) {
  for (int i = 0; i < touches.size(); i++) {
    SGPoint touchP = touches[i].location();
    SGPoint thumbP = ThumbPoint();
    double thumbWidth = thumb_texture_.contentSize().width;
    double thumbHeight = thumb_texture_.contentSize().height;
    if (touchP.x >= thumbP.x - thumbWidth && touchP.y >= thumbP.y - thumbHeight &&
          touchP.x < thumbP.x + 2 * thumbWidth &&
          touchP.y < thumbP.y + 2 * thumbHeight) {
      grabbed_touch_ = touches[i].identifier();
      last_touch_position_ = touchP;
      return;
    }
  }
}

void SoundSlider::TouchesMoved(vector<Touch> touches) {
  for (int i = 0; i < touches.size(); i++) {
    if (touches[i].identifier() == grabbed_touch_) {
      SGPoint touchP = touches[i].location();
      value_ += (touchP.x - last_touch_position_.x) / (269 - thumb_texture_.contentSize().width);
      last_touch_position_ = touchP;
      if (last_touch_position_.x < position_.x + 19) {
        last_touch_position_.x = position_.x + 10;
      } else if (last_touch_position_.x >= position_.x + 19 + (269.0 - thumb_texture_.contentSize().width)) {
        last_touch_position_.x = position_.x + 19 + (269.0 - thumb_texture_.contentSize().width) - 1;
      }
      if (value_ < 0.0) {
        value_ = 0.0;
      } else if (value_ > 1.0) {
        value_ = 1.0;
      }
      return;
    }
  }
}

void SoundSlider::TouchesEnded(vector<Touch> touches) {
  for (int i = 0; i < touches.size(); i++) {
    if (touches[i].identifier() == grabbed_touch_) {
      // TODO [SoundPlayer setGlobalVolume:value_];
      LocalStore::SetDouble(value_, LS_VOLUME);
      grabbed_touch_ = NULL;
      return;
    }
  }
}
