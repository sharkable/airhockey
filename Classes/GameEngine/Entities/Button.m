//
//  Button.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "Button.h"
#import "SoundPlayer.h"
#import "ResourceLoader.h"

@implementation Button

@synthesize delegate=delegate_, selector=selector_;

- (id)initWithNormalTexture:(Texture2D *)normalTexture
             pressedTexture:(Texture2D *)pressedTexture
                   position:(CGPoint)position {
  [super init];
  
  if (self) {
    normalTexture_ = normalTexture;  
    pressedTexture_ = pressedTexture;
    
    position_ = position;
    state_ = BUTTON_STATE_NORMAL;
  }
  
  return self;
}

- (void)dealloc {
  [[ResourceLoader instance] releaseResource:normalTexture_];
  [[ResourceLoader instance] releaseResource:pressedTexture_];
  
  [super dealloc];
}

- (void) update {
}

- (void)render {
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

- (void)touchesBegan:(Touch *[])touches numTouches:(int)numTouches {
  if (state_ == BUTTON_STATE_NORMAL) {
    for (int i = 0; i < numTouches; i++) {
      if ([self containsPoint:touches[i].location]) {
        state_ = BUTTON_STATE_PRESSED;
        [SoundPlayer playSound:kSoundButton];
      }
    }
  }
}

- (void)touchesEnded:(Touch *[])touches numTouches:(int)numTouches {
  if (state_ == BUTTON_STATE_PRESSED) {
    state_ = BUTTON_STATE_NORMAL;
    for (int i = 0; i < numTouches; i++) {
      if ([self containsPoint:touches[i].location]) {
        [delegate_ performSelector:selector_];
      }
    }
  }
}

- (BOOL)containsPoint:(CGPoint)p {
  return p.x >= position_.x &&
       p.y >= position_.y &&
       p.x < position_.x + pressedTexture_.contentSize.width &&
       p.y < position_.y + pressedTexture_.contentSize.height;
}

- (CGSize) size {
  return normalTexture_.contentSize;
}

@end
