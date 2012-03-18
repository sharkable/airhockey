//
//  SimpleItem.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AirHockey_SimpleItem_h
#define AirHockey_SimpleItem_h

#import <Foundation/Foundation.h>
#import "StateEntity.h"
#import "Texture2D.h"

class SimpleItem : public StateEntity {
 private:
  NSMutableArray *textures_;
  int texture_;
  CGPoint position_;
  double angle_;

 public:
  SimpleItem(Texture2D *texture, CGPoint position);
  SimpleItem(NSArray *textures, CGPoint position);
  ~SimpleItem();
  void update();
  void render();

  int getTexture() { return texture_; }
  void setTexture(int texture) { texture_ = texture; }
  CGPoint getPosition() { return position_; }
  void setPosition(CGPoint position) { position_ = position; }
  double getAngle() { return angle_; }
  void setAngle(double angle) { angle_ = angle; }
  CGSize getSize();
};

#endif
