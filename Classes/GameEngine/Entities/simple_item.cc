//
//  SimpleItem.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "simple_item.h"
#import "ResourceLoader.h"

SimpleItem::SimpleItem(Texture2D texture, SGPoint position) {
  textures_.push_back(texture);
  texture_ = 0;
  angle_ = 0;
  
  position_ = position;
}

SimpleItem::SimpleItem(vector<Texture2D> textures, SGPoint position) {
  textures_ = textures;
  texture_ = 0;
  angle_ = 0;
  
  position_ = position;
}

SimpleItem::~SimpleItem() {
  for (int i = 0; i < textures_.size(); i++) {
    ResourceLoader::instance()->releaseResource(textures_[i]);
  }
}

void SimpleItem::update() {
}

void SimpleItem::render() {
  textures_[texture_].drawAtPointAngle(SGPointMake(position_.x, position_.y), angle_);
}

SGSize SimpleItem::getSize() {
  return textures_[texture_].contentSize();
}
