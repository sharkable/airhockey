//
//  Button.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "StateEntity.h"
#import "Texture2D.h"

#define BUTTON_STATE_NORMAL  0
#define BUTTON_STATE_PRESSED 1

@interface Button : NSObject<StateEntity> {
 @private
  Texture2D *normalTexture_;
  Texture2D *pressedTexture_;
  CGPoint position_;
  int state_;
  id delegate_;  // weak
  SEL selector_;
}

- (id)initWithNormalTexture:(Texture2D *)normalTexture
             pressedTexture:(Texture2D *)pressedTexture
                   position:(CGPoint)position;
- (BOOL)containsPoint:(CGPoint)p;

@property(nonatomic, assign) id delegate;
@property(nonatomic, assign) SEL selector;
@property(nonatomic, readonly) CGSize size;

@end
