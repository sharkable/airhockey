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

#include "gameengine/view_entity.h"
#include "opengl/Texture2D.h"

class SimpleItem : public ViewEntity {
 public:
  SimpleItem();
  SimpleItem(Texture2D texture, ScreenPoint position);
  SimpleItem(vector<Texture2D> textures, ScreenPoint position);
  ~SimpleItem();

  // ViewEntity
  void Update() {}
  void Render();

  // Accessors
  void set_textures(vector<Texture2D> textures) { textures_ = textures; }
  int texture() { return texture_; }
  void set_texture(int texture) { texture_ = texture; }
  void add_texture(Texture2D texture) { textures_.push_back(texture); }
  ScreenPoint position() { return position_; }
  void set_position(ScreenPoint position) { position_ = position; }
  double angle() { return angle_; }
  void set_angle(double angle) { angle_ = angle; }
  ScreenSize size() { return textures_[texture_].content_size(); }

 private:
  vector<Texture2D> textures_;
  int texture_;
  ScreenPoint position_;
  double angle_;
};

#endif
