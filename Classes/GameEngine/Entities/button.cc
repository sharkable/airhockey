//
//  Button.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "button.h"

//#import "SoundPlayer.h"
#include "ResourceLoader.h"
#include "Touch.h"

Button::Button(Texture2D normal_texture, Texture2D pressed_texture, SGPoint position) {
  normal_texture_ = normal_texture;
  pressed_texture_ = pressed_texture;
  position_ = position;
  state_ = BUTTON_STATE_NORMAL;
  delegate_ = NULL;
}

Button::~Button() {
  ResourceLoader::instance()->releaseResource(normal_texture_);
  ResourceLoader::instance()->releaseResource(pressed_texture_);
}


bool Button::ContainsPoint(SGPoint p) {
  return p.x >= position_.x &&
  p.y >= position_.y &&
  p.x < position_.x + pressed_texture_.contentSize().width &&
  p.y < position_.y + pressed_texture_.contentSize().height;
}


// State Entity

void Button::Update() {
}

void Button::Render() {
  switch (state_) {
    case BUTTON_STATE_NORMAL: {
      normal_texture_.drawAtPoint(position_);
      break;
    }
    case BUTTON_STATE_PRESSED: {
      pressed_texture_.drawAtPoint(position_);
      break;
    }
  }
}

void Button::TouchesBegan(vector<Touch> touches) {
  if (state_ == BUTTON_STATE_NORMAL) {
    for (int i = 0; i < touches.size(); i++) {
      if (ContainsPoint(touches[i].location())) {
        state_ = BUTTON_STATE_PRESSED;
//        [SoundPlayer playSound:kSoundButton];
      }
    }
  }
}

void Button::TouchesEnded(vector<Touch> touches) {
  if (state_ == BUTTON_STATE_PRESSED) {
    state_ = BUTTON_STATE_NORMAL;
    for (int i = 0; i < touches.size(); i++) {
      if (ContainsPoint(touches[i].location())) {
        delegate_->ButtonPressed(this);
      }
    }
  }
}
