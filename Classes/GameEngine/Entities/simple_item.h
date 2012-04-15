//
//  simple_item.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAMEENGINE_ENTITIES_SIMPLEITEM_H_
#define AIRHOCKEY_GAMEENGINE_ENTITIES_SIMPLEITEM_H_

#include <vector>
using namespace std;

#import "gameengine/StateEntity.h"
#import "opengl/Texture2D.h"

class SimpleItem : public StateEntity {
 public:
  SimpleItem(Texture2D texture, SGPoint position);
  SimpleItem(vector<Texture2D> textures, SGPoint position);
  ~SimpleItem();

  // StateEntity
  void Update() {}
  void Render();

  // Accessors
  int texture() { return texture_; }
  void set_texture(int texture) { texture_ = texture; }
  SGPoint position() { return position_; }
  void set_position(SGPoint position) { position_ = position; }
  double angle() { return angle_; }
  void set_angle(double angle) { angle_ = angle; }
  SGSize size() { return textures_[texture_].contentSize(); }

 private:
  vector<Texture2D> textures_;
  int texture_;
  SGPoint position_;
  double angle_;
};

#endif
