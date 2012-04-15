//
//  simple_item.cc
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "gameengine/entities/simple_item.h"

#import "gameengine/ResourceLoader.h"

SimpleItem::SimpleItem(Texture2D texture, SGPoint position)
    : texture_(0),
      position_(position),
      angle_(0) {
  textures_.push_back(texture);
}

SimpleItem::SimpleItem(vector<Texture2D> textures, SGPoint position)
    : textures_(textures),
      texture_(0),
      position_(position),
      angle_(0) {
}

SimpleItem::~SimpleItem() {
  for (int i = 0; i < textures_.size(); i++) {
    ResourceLoader::Instance().ReleaseResource(textures_[i]);
  }
}


// StateEntity

void SimpleItem::Render() {
  textures_[texture_].drawAtPointAngle(SGPointMake(position_.x, position_.y), angle_);
}
