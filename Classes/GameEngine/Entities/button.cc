//
//  button.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "gameengine/entities/button.h"

#include "gameengine/resource_loader.h"
#include "gameengine/touch.h"
#include "soundengine/sound_player.h"

Button::Button(Texture2D normal_texture, Texture2D pressed_texture, GamePoint position)
    : normal_texture_(normal_texture),
      pressed_texture_(pressed_texture),
      position_(position),
      state_(kButtonStateNormal),
      delegate_(NULL) {
}

Button::~Button() {
  if (normal_texture_.loaded()) {
    ResourceLoader::Instance().ReleaseResource(normal_texture_);
  }
  if (pressed_texture_.loaded()) {
    ResourceLoader::Instance().ReleaseResource(pressed_texture_);
  }
}

bool Button::ContainsPoint(GamePoint p) {
  return p.x >= position_.x &&
  p.y >= position_.y &&
  p.x < position_.x + pressed_texture_.content_size().width &&
  p.y < position_.y + pressed_texture_.content_size().height;
}


// ViewEntity

void Button::Update() {
}

void Button::Render() {
  switch (state_) {
    case kButtonStateNormal: {
      // TODONOW normal_texture_.DrawAtPoint(position_);
      break;
    }
    case kButtonStatePressed: {
      // TODONOW pressed_texture_.DrawAtPoint(position_);
      break;
    }
  }
}

void Button::TouchesBegan(vector<Touch> touches) {
  if (state_ == kButtonStateNormal) {
    for (int i = 0; i < touches.size(); i++) {
// TODONOW
//      if (ContainsPoint(touches[i].location())) {
//        state_ = kButtonStatePressed;
//        SoundPlayer::instance()->playSound(kSoundButton);
//      }
    }
  }
}

void Button::TouchesEnded(vector<Touch> touches) {
  if (state_ == kButtonStatePressed) {
    state_ = kButtonStateNormal;
    for (int i = 0; i < touches.size(); i++) {
// TODONOW
//      if (ContainsPoint(touches[i].location())) {
//        if (delegate_) {
//          delegate_->ButtonPressed(this);
//        }
//      }
    }
  }
}
