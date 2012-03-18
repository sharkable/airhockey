//
//  SoundSlider.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-06.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AirHockey_SoundSlider_h
#define AirHockey_SoundSlider_h

#import <Foundation/Foundation.h>
#import "StateEntity.h"
#import "Texture2D.h"

class SoundSlider : public StateEntity {
 private:
  CGPoint    position_;
  Texture2D* emptyTexture_;
  Texture2D* fullTexture_;
  Texture2D* thumbTexture_;
  double     value_;
  void *grabbedTouch_;
  CGPoint    lastTouchPosition_;

 public:
  SoundSlider(CGPoint position);
  void update();
  void render();
  void touchesBegan(Touch *touches[], int numTouches);
  void touchesMoved(Touch *touches[], int numTouches);
  void touchesEnded(Touch *touches[], int numTouches);
  CGPoint getThumbPoint();
};

#endif
