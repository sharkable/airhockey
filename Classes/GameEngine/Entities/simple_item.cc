//
//  simple_item.cc
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "gameengine/entities/simple_item.h"

#import "gameengine/resource_loader.h"

SimpleItem::SimpleItem()
    : texture_(0),
      position_(SGPointMake(0, 0)),
      angle_(0) {
}

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


// ViewEntity

void SimpleItem::Render() {
  textures_[texture_].DrawAtPointAngle(SGPointMake(position_.x, position_.y), angle_);
}
