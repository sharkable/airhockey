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

@synthesize delegate=_delegate, selector=_selector;

- (id) initWithNormalTexture:(Texture2D*)normalTexture pressedTexture:(Texture2D*)pressedTexture position:(CGPoint)position {
  [super init];
  
  _normalTexture = normalTexture;  
  _pressedTexture = pressedTexture;
  
  _position = position;
  _state = BUTTON_STATE_NORMAL;
  
  return self;
}

- (void) dealloc {
  [[ResourceLoader instance] releaseResource:_normalTexture];
  [[ResourceLoader instance] releaseResource:_pressedTexture];
  
  [super dealloc];
}

- (void) update {
}

- (void) render {
  switch (_state) {
    case BUTTON_STATE_NORMAL: {
      [_normalTexture drawAtPoint:_position];
      break;
    }
    case BUTTON_STATE_PRESSED: {
      [_pressedTexture drawAtPoint:_position];
      break;
    }
  }
}

- (void) touchesBegan:(Touch*[])touches numTouches:(int)numTouches {
  if (_state == BUTTON_STATE_NORMAL) {
    for (int i = 0; i < numTouches; i++) {
      if ([self containsPoint:touches[i].location]) {
        _state = BUTTON_STATE_PRESSED;
        [SoundPlayer playSound:kSoundButton];
      }
    }
  }
}

- (void) touchesEnded:(Touch*[])touches numTouches:(int)numTouches {
  if (_state == BUTTON_STATE_PRESSED) {
    _state = BUTTON_STATE_NORMAL;
    for (int i = 0; i < numTouches; i++) {
      if ([self containsPoint:touches[i].location]) {
        [_delegate performSelector:_selector];
      }
    }
  }
}

- (BOOL) containsPoint:(CGPoint)p {
  return p.x >= _position.x &&
       p.y >= _position.y &&
       p.x < _position.x + _pressedTexture.contentSize.width &&
         p.y < _position.y + _pressedTexture.contentSize.height;
}

- (CGSize) size {
  return _normalTexture.contentSize;
}

@end
