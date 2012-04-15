//
//  Touch.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AirHockey_Touch_h
#define AirHockey_Touch_h

#include "Texture2D.h"

class Touch {
 public:
  SGPoint location() const { return location_; }
  void set_location(SGPoint location) { location_ = location; }
  void *identifier() const { return identifier_; }
  void set_identifier(void *identifier) { identifier_ = identifier; }

 private:
  SGPoint location_;
  void *identifier_;
};

#endif
