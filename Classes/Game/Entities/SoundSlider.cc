//
//  SoundSlider.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-06.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "SoundSlider.h"
#import "ResourceLoader.h"
#import "LocalStore.h"
#include "Touch.h"
#import "const.h"

SoundSlider::SoundSlider(SGPoint position) {
  position_ = position;
  
  emptyTexture_ = ResourceLoader::instance()->getTextureWithName("sound_empty");
  fullTexture_ = ResourceLoader::instance()->getTextureWithName("sound_full");
  thumbTexture_ = ResourceLoader::instance()->getTextureWithName("sound_thumb");
  
  if (LocalStore::HasEntryForKey(LS_VOLUME)) {
    value_ = LocalStore::DoubleForKey(LS_VOLUME);
  } else {
    value_ = 0.75;
  }
  // TODO [SoundPlayer setGlobalVolume:value_];
}

void SoundSlider::Update() {
}

void SoundSlider::Render() {
  double drawRatio = (269.0 - thumbTexture_.contentSize().width)/320.0 * value_ + (19.0 + thumbTexture_.contentSize().width/2)/320.0;
  fullTexture_.drawAtPointLeftRatio(position_, drawRatio);
  emptyTexture_.drawAtPointRightRatio(position_, 1.0 - drawRatio);
  thumbTexture_.drawAtPoint(getThumbPoint());
}

SGPoint SoundSlider::getThumbPoint() {
  return SGPointMake(position_.x + 19 + (269.0 - thumbTexture_.contentSize().width)*value_, position_.y);
}

void SoundSlider::TouchesBegan(vector<Touch> touches) {
  for (int i = 0; i < touches.size(); i++) {
    SGPoint touchP = touches[i].location();
    SGPoint thumbP = getThumbPoint();
    double thumbWidth = thumbTexture_.contentSize().width;
    double thumbHeight = thumbTexture_.contentSize().height;
    if (touchP.x >= thumbP.x - thumbWidth && touchP.y >= thumbP.y - thumbHeight &&
          touchP.x < thumbP.x + 2 * thumbWidth &&
          touchP.y < thumbP.y + 2 * thumbHeight) {
      grabbedTouch_ = touches[i].identifier();
      lastTouchPosition_ = touchP;
      return;
    }
  }
}

void SoundSlider::TouchesMoved(vector<Touch> touches) {
  for (int i = 0; i < touches.size(); i++) {
    if (touches[i].identifier() == grabbedTouch_) {
      SGPoint touchP = touches[i].location();
      value_ += (touchP.x - lastTouchPosition_.x) / (269 - thumbTexture_.contentSize().width);
      lastTouchPosition_ = touchP;
      if (lastTouchPosition_.x < position_.x + 19) {
        lastTouchPosition_.x = position_.x + 10;
      } else if (lastTouchPosition_.x >= position_.x + 19 + (269.0 - thumbTexture_.contentSize().width)) {
        lastTouchPosition_.x = position_.x + 19 + (269.0 - thumbTexture_.contentSize().width) - 1;
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
    if (touches[i].identifier() == grabbedTouch_) {
      // TODO [SoundPlayer setGlobalVolume:value_];
      LocalStore::SetDouble(value_, LS_VOLUME);
      grabbedTouch_ = NULL;
      return;
    }
  }
}
