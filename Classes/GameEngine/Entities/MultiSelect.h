//
//  MultiSelect.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AirHockey_MutliSelect_h
#define AirHockey_MultiSelect_h

#import "StateEntity.h"
#import "Texture2D.h"

#include <vector>
using namespace std;

class MultiSelect : public StateEntity {
 private:
  vector<Texture2D> normalTextures_;
  vector<Texture2D> selectedTextures_;
  vector<double> positionsX_;
  vector<double> positionsY_;
  int selectedValue_;

 public:
  MultiSelect() {}
  MultiSelect(Texture2D normalTexture, Texture2D selectedTexture,
              SGPoint position);
  ~MultiSelect();
  void update();
  void render();
  void touchesBegan(Touch *touches[], int numTouches);
  void add(Texture2D normalTexture, Texture2D selectedTexture,
           SGPoint position);
  int getSelectedValue() { return selectedValue_; };
  void setSelectedValue(int selectedValue) { selectedValue_ = selectedValue; }
};

#endif
