//
//  StateEntity.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AirHockey_StateEntity_h
#define AirHockey_StateEntity_h

#include <vector>
using namespace std;

class Touch;

class StateEntity {
 public:
  virtual void update() = 0;
  virtual void render() = 0;
  virtual void touchesBegan(vector<Touch> touches) {}
  virtual void touchesMoved(vector<Touch> touches) {}
  virtual void touchesEnded(vector<Touch> touches) {}
  virtual void clearTouches() {}
};

#endif
