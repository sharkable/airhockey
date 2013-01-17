//
//  multi_select.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAMEENGINE_ENTITIES_MULTISELECT_H_
#define AIRHOCKEY_GAMEENGINE_ENTITIES_MULTISELECT_H_

#include <vector>
using namespace std;

#include "gameengine/view_entity.h"

class Texture2D;
struct SGPoint;

class MultiSelect : public ViewEntity {
 public:
  MultiSelect() {}
  ~MultiSelect();

  void Add(Texture2D normal_texture, Texture2D selected_texture, SGPoint position);

  // ViewEntity
  void Update() {}
  void Render();
  void TouchesBegan(vector<Touch> touches);
  string Name() { return "MultiSelect"; }

  // Accessors
  int selected_value() { return selected_value_; };
  void set_selected_value(int selected_value) { selected_value_ = selected_value; }

 private:
  vector<Texture2D> normal_textures_;
  vector<Texture2D> selected_textures_;
  vector<double> positions_x_;
  vector<double> positions_y_;
  int selected_value_;
};

#endif
