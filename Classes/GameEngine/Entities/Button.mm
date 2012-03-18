//
//  Button.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "Button.h"
#import "SoundPlayer.h"
#import "ResourceLoader.h"
#include "Touch.h"

Button::Button(Texture2D *normalTexture, Texture2D *pressedTexture,
               CGPoint position) {
  normalTexture_ = normalTexture;  
  pressedTexture_ = pressedTexture;
  position_ = position;
  state_ = BUTTON_STATE_NORMAL;
}

Button::~Button() {
  [[ResourceLoader instance] releaseResource:normalTexture_];
  [[ResourceLoader instance] releaseResource:pressedTexture_];
}

void Button::update() {
}

void Button::render() {
  switch (state_) {
    case BUTTON_STATE_NORMAL: {
      [normalTexture_ drawAtPoint:position_];
      break;
    }
    case BUTTON_STATE_PRESSED: {
      [pressedTexture_ drawAtPoint:position_];
      break;
    }
  }
}

void Button::touchesBegan(Touch *touches[], int numTouches) {
  if (state_ == BUTTON_STATE_NORMAL) {
    for (int i = 0; i < numTouches; i++) {
      if (containsPoint(touches[i]->getLocation())) {
        state_ = BUTTON_STATE_PRESSED;
        [SoundPlayer playSound:kSoundButton];
      }
    }
  }
}

void Button::touchesEnded(Touch *touches[], int numTouches) {
  if (state_ == BUTTON_STATE_PRESSED) {
    state_ = BUTTON_STATE_NORMAL;
    for (int i = 0; i < numTouches; i++) {
      if (containsPoint(touches[i]->getLocation())) {
        [delegate_ performSelector:selector_];
      }
    }
  }
}

bool Button::containsPoint(CGPoint p) {
  return p.x >= position_.x &&
       p.y >= position_.y &&
       p.x < position_.x + pressedTexture_.contentSize.width &&
       p.y < position_.y + pressedTexture_.contentSize.height;
}

CGSize Button::getSize() {
  return normalTexture_.contentSize;
}
