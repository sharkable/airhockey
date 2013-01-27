//
//  touch.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAMEENGINE_TOUCH_H_
#define AIRHOCKEY_GAMEENGINE_TOUCH_H_

#include "Texture2D.h"

class Touch {
 public:
  ScreenPoint location() const { return location_; }
  void set_location(ScreenPoint location) { location_ = location; }
  void *identifier() const { return identifier_; }
  void set_identifier(void *identifier) { identifier_ = identifier; }

 private:
  ScreenPoint location_;
  void *identifier_;
};

#endif
