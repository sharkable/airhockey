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

MultiSelect::MultiSelect() {
  normalTextures_ = [[NSMutableArray alloc] init];
  selectedTextures_ = [[NSMutableArray alloc] init];
  positionsX_ = [[NSMutableArray alloc] init];
  positionsY_ = [[NSMutableArray alloc] init];
}

MultiSelect::MultiSelect(Texture2D *normalTexture, Texture2D *selectedTexture,
              CGPoint position) {
  MultiSelect();
}

MultiSelect::~MultiSelect() {
  for (Texture2D *texture in normalTextures_) {
    [[ResourceLoader instance] releaseResource:texture];
  }
  for (Texture2D *texture in selectedTextures_) {
    [[ResourceLoader instance] releaseResource:texture];
  }
  
  [normalTextures_ release];
  [selectedTextures_ release];
  [positionsX_ release];
  [positionsY_ release];
}

void MultiSelect::update() {
}

void MultiSelect::render() {
  for (int i = 0; i < normalTextures_.count; i++) {
    CGPoint p = CGPointMake([[positionsX_ objectAtIndex:i] doubleValue], 
                            [[positionsY_ objectAtIndex:i] doubleValue]);
    if (i == selectedValue_) {
      [[selectedTextures_ objectAtIndex:i] drawAtPoint:p];
    } else {
      [[normalTextures_ objectAtIndex:i] drawAtPoint:p];  
    }
  }
}

void MultiSelect::add(Texture2D *normalTexture, Texture2D *selectedTexture,
                      CGPoint position) {
  [normalTextures_ addObject:normalTexture];
  [selectedTextures_ addObject:selectedTexture];
  [positionsX_ addObject:[NSNumber numberWithDouble:position.x]];
  [positionsY_ addObject:[NSNumber numberWithDouble:position.y]];
}

void MultiSelect::touchesBegan(Touch *touches[], int numTouches) {
  for (int i = 0; i < normalTextures_.count; i++) {
    double x = [[positionsX_ objectAtIndex:i] doubleValue];
    double y = [[positionsY_ objectAtIndex:i] doubleValue];
    CGSize size = ((Texture2D *)[normalTextures_ objectAtIndex:i]).contentSize;
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