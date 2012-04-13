//
//  SimpleItem.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AirHockey_SimpleItem_h
#define AirHockey_SimpleItem_h

#import "StateEntity.h"
#import "Texture2D.h"

#include <vector>
using namespace std;

class SimpleItem : public StateEntity {
 private:
  vector<Texture2D> textures_;
  int texture_;
  SGPoint position_;
  double angle_;

 public:
  SimpleItem(Texture2D texture, SGPoint position);
  SimpleItem(vector<Texture2D> textures, SGPoint position);
  ~SimpleItem();
  void update();
  void render();

  int getTexture() { return texture_; }
  void setTexture(int texture) { texture_ = texture; }
  SGPoint getPosition() { return position_; }
  void setPosition(SGPoint position) { position_ = position; }
  double getAngle() { return angle_; }
  void setAngle(double angle) { angle_ = angle; }
  SGSize getSize();
};

#endif
