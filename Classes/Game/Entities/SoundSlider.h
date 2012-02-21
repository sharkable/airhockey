//
//  SoundSlider.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-06.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "StateEntity.h"
#import "Texture2D.h"

@interface SoundSlider : NSObject <StateEntity> {
  CGPoint    position_;
  Texture2D* emptyTexture_;
  Texture2D* fullTexture_;
  Texture2D* thumbTexture_;
  double     value_;
  UITouch*   grabbedTouch_;
  CGPoint    lastTouchPosition_;
}

- (id) initWithPosition:(CGPoint)position;

@property (readonly) CGPoint thumbPoint;

@end
