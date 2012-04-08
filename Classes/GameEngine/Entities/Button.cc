//
//  Button.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "Button.h"
//#import "SoundPlayer.h"
#include "ResourceLoader.h"
#include "Touch.h"

Button::Button(Texture2D normalTexture, Texture2D pressedTexture, SGPoint position) {
  normalTexture_ = normalTexture;
  pressedTexture_ = pressedTexture;
  position_ = position;
  state_ = BUTTON_STATE_NORMAL;
  delegate_ = NULL;
}

Button::~Button() {
  ResourceLoader::instance()->releaseResource(normalTexture_);
  ResourceLoader::instance()->releaseResource(pressedTexture_);
}

void Button::update() {
}

void Button::render() {
  switch (state_) {
    case BUTTON_STATE_NORMAL: {
      normalTexture_.drawAtPoint(position_);
      break;
    }
    case BUTTON_STATE_PRESSED: {
      pressedTexture_.drawAtPoint(position_);
      break;
    }
  }
}

void Button::touchesBegan(vector<Touch> touches) {
  if (state_ == BUTTON_STATE_NORMAL) {
    for (int i = 0; i < touches.size(); i++) {
      if (containsPoint(touches[i].getLocation())) {
        state_ = BUTTON_STATE_PRESSED;
//        [SoundPlayer playSound:kSoundButton];
      }
    }
  }
}

void Button::touchesEnded(vector<Touch> touches) {
  if (state_ == BUTTON_STATE_PRESSED) {
    state_ = BUTTON_STATE_NORMAL;
    for (int i = 0; i < touches.size(); i++) {
      if (containsPoint(touches[i].getLocation())) {
        delegate_->buttonPressed(this);
      }
    }
  }
}

bool Button::containsPoint(SGPoint p) {
  return p.x >= position_.x &&
       p.y >= position_.y &&
       p.x < position_.x + pressedTexture_.contentSize().width &&
       p.y < position_.y + pressedTexture_.contentSize().height;
}

SGSize Button::getSize() {
  return normalTexture_.contentSize();
}
