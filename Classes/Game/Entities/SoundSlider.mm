//
//  SoundSlider.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-06.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "SoundSlider.h"
#import "SoundPlayer.h"
#import "ResourceLoader.h"
#import "LocalStore.h"
#include "Touch.h"
#import "const.h"

SoundSlider::SoundSlider(CGPoint position) {
  position_ = position;
  
  emptyTexture_ = [[ResourceLoader instance] getTextureWithName:@"sound_empty"];
  fullTexture_  = [[ResourceLoader instance] getTextureWithName:@"sound_full"];
  thumbTexture_ = [[ResourceLoader instance] getTextureWithName:@"sound_thumb"];
  
  if (LocalStore::hasEntryForKey(LS_VOLUME)) {
    value_ = LocalStore::doubleForKey(LS_VOLUME);
  } else {
    value_ = 0.75;
  }
  [SoundPlayer setGlobalVolume:value_];
}

void SoundSlider::update() {
}

void SoundSlider::render() {
  double drawRatio = (269.0 - thumbTexture_.contentSize.width)/320.0 * value_ + (19.0 + thumbTexture_.contentSize.width/2)/320.0;
  [fullTexture_ drawAtPoint:position_ leftRatio:drawRatio];
  [emptyTexture_ drawAtPoint:position_ rightRatio:1.0-drawRatio];
  [thumbTexture_ drawAtPoint:getThumbPoint()];
}

CGPoint SoundSlider::getThumbPoint() {
  return CGPointMake(position_.x + 19 + (269.0 - thumbTexture_.contentSize.width)*value_, position_.y);
}

void SoundSlider::touchesBegan(Touch *touches[], int numTouches) {
  for (int i = 0; i < numTouches; i++) {
    CGPoint touchP = touches[i]->getLocation();
    CGPoint thumbP = getThumbPoint();
    double thumbWidth = thumbTexture_.contentSize.width;
    double thumbHeight = thumbTexture_.contentSize.height;
    if (touchP.x >= thumbP.x - thumbWidth && touchP.y >= thumbP.y - thumbHeight &&
          touchP.x < thumbP.x + 2 * thumbWidth &&
          touchP.y < thumbP.y + 2 * thumbHeight) {
      grabbedTouch_ = touches[i]->getIdentifier();
      lastTouchPosition_ = touchP;
      return;
    }
  }
}

void SoundSlider::touchesMoved(Touch *touches[], int numTouches) {
  for (int i = 0; i < numTouches; i++) {
    if (touches[i]->getIdentifier() == grabbedTouch_) {
      CGPoint touchP = touches[i]->getLocation();
      value_ += (touchP.x - lastTouchPosition_.x) / (269 - thumbTexture_.contentSize.width);
      lastTouchPosition_ = touchP;
      if (lastTouchPosition_.x < position_.x + 19) {
        lastTouchPosition_.x = position_.x + 10;
      } else if (lastTouchPosition_.x >= position_.x + 19 + (269.0 - thumbTexture_.contentSize.width)) {
        lastTouchPosition_.x = position_.x + 19 + (269.0 - thumbTexture_.contentSize.width) - 1;
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

void SoundSlider::touchesEnded(Touch *touches[], int numTouches) {
  for (int i = 0; i < numTouches; i++) {
    if (touches[i]->getIdentifier() == grabbedTouch_) {
      [SoundPlayer setGlobalVolume:value_];
      LocalStore::setDouble(value_, LS_VOLUME);
      grabbedTouch_ = nil;
      return;
    }
  }
}