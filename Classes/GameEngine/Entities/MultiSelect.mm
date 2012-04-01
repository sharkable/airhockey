//
//  MultiSelect.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "MultiSelect.h"
#import "SoundPlayer.h"
#import "ResourceLoader.h"
#include "Touch.h"

MultiSelect::MultiSelect(Texture2D normalTexture, Texture2D selectedTexture,
              CGPoint position) {
}

MultiSelect::~MultiSelect() {
  for (int i = 0; i < normalTextures_.size(); i++) {
    ResourceLoader::instance()->releaseResource(normalTextures_[i]);
  }
  for (int i = 0; i < selectedTextures_.size(); i++) {
    ResourceLoader::instance()->releaseResource(selectedTextures_[i]);
  }
}

void MultiSelect::update() {
}

void MultiSelect::render() {
  for (int i = 0; i < normalTextures_.size(); i++) {
    CGPoint p = CGPointMake(positionsX_[i], positionsY_[i]);
    if (i == selectedValue_) {
      selectedTextures_[i].drawAtPoint(p);
    } else {
      normalTextures_[i].drawAtPoint(p);
    }
  }
}

void MultiSelect::add(Texture2D normalTexture, Texture2D selectedTexture,
                      CGPoint position) {
  normalTextures_.push_back(normalTexture);
  selectedTextures_.push_back(selectedTexture);
  positionsX_.push_back(position.x);
  positionsY_.push_back(position.y);
}

void MultiSelect::touchesBegan(Touch *touches[], int numTouches) {
  for (int i = 0; i < normalTextures_.size(); i++) {
    double x = positionsX_[i];
    double y = positionsY_[i];
    CGSize size = normalTextures_[i].contentSize();
    for (int j = 0; j < numTouches; j++) {
      CGPoint touchPoint = touches[j]->getLocation();
      if (touchPoint.x >= x && touchPoint.y >= y && touchPoint.x < x + size.width &&
          touchPoint.y < y + size.height) {
        if (selectedValue_ != i) {
          selectedValue_ = i;
          [SoundPlayer playSound:kSoundMultiSelect];
        };
        return;
      }
    }
  }
}
