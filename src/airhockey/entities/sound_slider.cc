//
//  sound_slider.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2010-05-06.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "airhockey/entities/sound_slider.h"

#include "sharkengine/modules/persistence_module.h"
#include "sharkengine/platform.h"
#include "sharkengine/shark_engine.h"

#include "sharksound/sound_controller.h"

using std::string;
using std::vector;

const int kLeftMarginTablet = 14;
const int kSliderWidthTablet = 231;
const int kLeftMarginPhone = 32;
const int kSliderWidthPhone = 506;

// Locale Store key
static const string kLocalStoreVolume = "ls_volume";

SoundSlider::SoundSlider(SharkEngine &shark_engine, GamePoint position)
    : shark_engine_(shark_engine),
      position_(position),
      empty_sprite_(shark_engine, "sound_empty"),
      full_sprite_(shark_engine, "sound_full"),
      thumb_sprite_(shark_engine, "sound_thumb") {
  switch (shark_engine.platform().texture_group()) {
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
  if (shark_engine.persistence_module()->HasEntryForKey(kLocalStoreVolume)) {
    value_ = shark_engine.persistence_module()->DoubleForKey(kLocalStoreVolume);
  } else {
    value_ = 0.75;
  }
  shark_engine.sound()->SetGlobalVolume(value_);
}

GamePoint SoundSlider::ThumbPoint() {
  return GamePoint(position_.x + left_marin_ +
                       (slider_width_ - thumb_sprite_.content_size().width) * value_,
                   position_.y);
}


#pragma mark - Renderer

void SoundSlider::Render(CoordinateSystem const &coordinate_system) {
//  double sprite_width = full_sprite_.content_size().width;
//  double draw_ratio = (slider_width_ - thumb_sprite_.content_size().width) / sprite_width * value_ +
//      (left_marin_ + thumb_sprite_.content_size().width / 2) / sprite_width;
  GamePoint draw_position = position_ + coordinate_system.origin();
  GameRect subtexture(kGamePointZero, full_sprite_.content_size());
  shark_log("texture: %s", to_string(subtexture).c_str());
  full_sprite_.Draw(coordinate_system.Translate(draw_position), subtexture);
//  empty_sprite_.Draw(coordinate_system.Translate(draw_position), 1, 1 - draw_ratio, 0, 1);
  thumb_sprite_.Draw(coordinate_system.Translate(ThumbPoint()));
}


#pragma mark - InputHandler

bool SoundSlider::HandleInputEvent(InputEvent const &event,
                                   CoordinateSystem const &coordinate_system) {
  if (!event.HasLocation()) {
    return false;
  }
  if (event.action() == InputEvent::kActionDown) {
    GamePoint touchP = event.location();
    GamePoint thumbP = ThumbPoint();
    double thumbWidth = thumb_sprite_.content_size().width;
    double thumbHeight = thumb_sprite_.content_size().height;
    if (touchP.x >= thumbP.x - thumbWidth && touchP.y >= thumbP.y - thumbHeight &&
        touchP.x < thumbP.x + 2 * thumbWidth &&
        touchP.y < thumbP.y + 2 * thumbHeight) {
      grabbed_touch_ = event.id();
      start_touch_position_ = touchP;
      start_value_ = value_;
      return true;
    }
  } else if (event.action() == InputEvent::kActionMove) {
    if (event.id() == grabbed_touch_) {
      GamePoint touchP = event.location();
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
  } else if (event.action() == InputEvent::kActionUp) {
    if (event.id() == grabbed_touch_) {
      shark_engine_.sound()->SetGlobalVolume(value_);
      shark_engine_.persistence_module()->SetDouble(value_, kLocalStoreVolume);
      grabbed_touch_ = NULL;
      return true;
    }
  }
  return false;
}
