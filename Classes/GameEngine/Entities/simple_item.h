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

#include "gameengine/sprite.h"
#include "gameengine/view_entity.h"

class SimpleItem : public ViewEntity {
 public:
  SimpleItem();
  SimpleItem(Sprite sprite, ScreenPoint position);
  SimpleItem(vector<Sprite> sprites, ScreenPoint position);
  ~SimpleItem();

  // ViewEntity
  void Update() {}
  void Render();

  // Accessors
  void set_sprites(vector<Sprite> textures) { textures_ = textures; }
  int texture() { return texture_; }
  void set_sprite(int sprite) { sprite_ = sprite; }
  void add_sprite(Sprite sprite) { sprites_.push_back(sprite); }
  ScreenPoint position() { return position_; }
  void set_position(ScreenPoint position) { position_ = position; }
  double angle() { return angle_; }
  void set_angle(double angle) { angle_ = angle; }
  ScreenSize size() { return textures_[texture_].content_size(); }

 private:
  vector<Sprite> sprites_;
  int sprite_;
  ScreenPoint position_;
  double angle_;
};

#endif
