//
//  sound_slider.cc
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-06.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "sound_slider.h"
#import "resource_loader.h"
#import "gameengine/local_store.h"
#include "Touch.h"
#import "const.h"

#include "soundengine/sound_player.h"

SoundSlider::SoundSlider(sp<GameEngine> game_engine, GamePoint position)
    : position_(position),
      empty_sprite_(game_engine, "sound_empty"),
      full_sprite_(game_engine, "sound_full"),
      thumb_sprite_(game_engine, "sound_thumb") {
  if (LocalStore::HasEntryForKey(LS_VOLUME)) {
    value_ = LocalStore::DoubleForKey(LS_VOLUME);
  } else {
    value_ = 0.75;
  }
  SoundPlayer::instance()->setGlobalVolume(value_);
}

GamePoint SoundSlider::ThumbPoint() {
  return game_point_make(position_.x + 19 + (269.0 - thumb_sprite_.content_size().width)*value_, position_.y);
}


// ViewEntity

void SoundSlider::Update() {
}

void SoundSlider::Render() {
  double drawRatio = (269.0 - thumb_sprite_.content_size().width)/320.0 * value_ + (19.0 + thumb_sprite_.content_size().width/2)/320.0;
  full_sprite_.DrawAtPointLeftRatio(position_, drawRatio);
  empty_sprite_.DrawAtPointRightRatio(position_, 1.0 - drawRatio);
  thumb_sprite_.DrawAtPoint(ThumbPoint());
}

void SoundSlider::TouchesBegan(vector<Touch> touches) {
  for (int i = 0; i < touches.size(); i++) {
    GamePoint touchP = touches[i].location();
    GamePoint thumbP = ThumbPoint();
    double thumbWidth = thumb_sprite_.content_size().width;
    double thumbHeight = thumb_sprite_.content_size().height;
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
      GamePoint touchP = touches[i].location();
      value_ += (touchP.x - last_touch_position_.x) / (269 - thumb_sprite_.content_size().width);
      last_touch_position_ = touchP;
      if (last_touch_position_.x < position_.x + 19) {
        last_touch_position_.x = position_.x + 10;
      } else if (last_touch_position_.x >= position_.x + 19 + (269.0 - thumb_sprite_.content_size().width)) {
        last_touch_position_.x = position_.x + 19 + (269.0 - thumb_sprite_.content_size().width) - 1;
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
      SoundPlayer::instance()->setGlobalVolume(value_);
      LocalStore::SetDouble(value_, LS_VOLUME);
      grabbed_touch_ = NULL;
      return;
    }
  }
}