//
//  multi_select.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "gameengine/entities/multi_select.h"

#include "gameengine/resource_loader.h"
#include "gameengine/touch.h"

MultiSelect::~MultiSelect() {
  for (int i = 0; i < normal_textures_.size(); i++) {
    ResourceLoader::Instance().ReleaseResource(normal_textures_[i]);
  }
  for (int i = 0; i < selected_textures_.size(); i++) {
    ResourceLoader::Instance().ReleaseResource(selected_textures_[i]);
  }
}

void MultiSelect::Add(Texture2D normal_texture, Texture2D selected_texture, SGPoint position) {
  normal_textures_.push_back(normal_texture);
  selected_textures_.push_back(selected_texture);
  positions_x_.push_back(position.x);
  positions_y_.push_back(position.y);
}


// StateEntity

void MultiSelect::Render() {
  for (int i = 0; i < normal_textures_.size(); i++) {
    SGPoint p = SGPointMake(positions_x_[i], positions_y_[i]);
    if (i == selected_value_) {
      selected_textures_[i].DrawAtPoint(p);
    } else {
      normal_textures_[i].DrawAtPoint(p);
    }
  }
}

void MultiSelect::TouchesBegan(vector<Touch> touches) {
  for (int i = 0; i < normal_textures_.size(); i++) {
    double x = positions_x_[i];
    double y = positions_y_[i];
    SGSize size = normal_textures_[i].content_size();
    for (int j = 0; j < touches.size(); j++) {
      SGPoint touchPoint = touches[j].location();
      if (touchPoint.x >= x && touchPoint.y >= y && touchPoint.x < x + size.width &&
          touchPoint.y < y + size.height) {
        if (selected_value_ != i) {
          selected_value_ = i;
          // TODO [SoundPlayer playSound:kSoundMultiSelect];
        };
        return;
      }
    }
  }
}
