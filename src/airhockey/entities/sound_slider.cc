//
//  sound_slider.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2010-05-06.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "airhockey/entities/sound_slider.h"

#include "gameengine/modules/persistence_module.h"
#include "gameengine/game_engine.h"
#include "gameengine/platform.h"
#include "gameengine/touch.h"

#include "sharksound/sound_controller.h"

using std::string;
using std::vector;

const int kLeftMarginTablet = 14;
const int kSliderWidthTablet = 231;
const int kLeftMarginPhone = 32;
const int kSliderWidthPhone = 506;

// Locale Store key
static const string kLocalStoreVolume = "ls_volume";

SoundSlider::SoundSlider(GameEngine &game_engine, GamePoint position)
    : game_engine_(game_engine),
      position_(position),
      empty_sprite_(game_engine, "sound_empty"),
      full_sprite_(game_engine, "sound_full"),
      thumb_sprite_(game_engine, "sound_thumb") {
  switch (game_engine.platform().texture_group()) {
    case Platform::kTextureGroupIPhone35cmLowRes:
    case Platform::kTextureGroupAndroidLowRes:
    case Platform::kTextureGroupIPhone35cmHighRes:
    case Platform::kTextureGroupIPhone40cmHighRes:
    case Platform::kTextureGroupAndroidHighRes:
      left_marin_ = kLeftMarginPhone;
      slider_width_ = kSliderWidthPhone;
      break;
    case Platform::kTextureGroupIPadLowRes:
    case Platform::kTextureGroupIPadHighRes:
    case Platform::kTextureGroupPCHighRes:
    case Platform::kTextureGroupPCUltraHighRes:
      left_marin_ = kLeftMarginTablet;
      slider_width_ = kSliderWidthTablet;
      break;
  }
  if (game_engine.persistence_module()->HasEntryForKey(kLocalStoreVolume)) {
    value_ = game_engine.persistence_module()->DoubleForKey(kLocalStoreVolume);
  } else {
    value_ = 0.75;
  }
  game_engine.sound()->SetGlobalVolume(value_);
}

GamePoint SoundSlider::ThumbPoint() {
  return GamePoint(position_.x + left_marin_ +
                       (slider_width_ - thumb_sprite_.content_size().width) * value_,
                   position_.y);
}


#pragma mark - ViewEntity

void SoundSlider::Update() {
}

void SoundSlider::Render(GamePoint offset) {
  double sprite_width = full_sprite_.content_size().width;
  double draw_ratio = (slider_width_ - thumb_sprite_.content_size().width) / sprite_width * value_ +
      (left_marin_ + thumb_sprite_.content_size().width / 2) / sprite_width;
  GamePoint draw_position = position_ + offset;
  full_sprite_.DrawAtPointLeftRatio(draw_position, draw_ratio);
  empty_sprite_.DrawAtPointRightRatio(draw_position, 1 - draw_ratio);
  thumb_sprite_.DrawAtPoint(ThumbPoint() + offset);
}


#pragma mark - InputHandler

bool SoundSlider::HandleEvent(InputEvent const &event) {
  if (!event.HasLocation()) {
    return false;
  }
  if (event.Action() == kInputActionDown) {
    GamePoint touchP = event.Location();
    GamePoint thumbP = ThumbPoint();
    double thumbWidth = thumb_sprite_.content_size().width;
    double thumbHeight = thumb_sprite_.content_size().height;
    if (touchP.x >= thumbP.x - thumbWidth && touchP.y >= thumbP.y - thumbHeight &&
        touchP.x < thumbP.x + 2 * thumbWidth &&
        touchP.y < thumbP.y + 2 * thumbHeight) {
      grabbed_touch_ = event.Id();
      start_touch_position_ = touchP;
      start_value_ = value_;
      return true;
    }
  } else if (event.Action() == kInputActionMove) {
    if (event.Id() == grabbed_touch_) {
      GamePoint touchP = event.Location();
      value_ = start_value_ + (touchP.x - start_touch_position_.x) /
      (slider_width_ - thumb_sprite_.content_size().width);
      // Stop moving when we reach the ends. Lock into this value until the user retouches.
      if (value_ < 0) {
        value_ = 0;
      } else if (value_ > 1) {
        value_ = 1;
      }
      return true;
    }
  } else if (event.Action() == kInputActionUp) {
    if (event.Id() == grabbed_touch_) {
      game_engine_.sound()->SetGlobalVolume(value_);
      game_engine_.persistence_module()->SetDouble(value_, kLocalStoreVolume);
      grabbed_touch_ = NULL;
      return true;
    }
  }
  return false;
}
