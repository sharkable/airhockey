//
//  Touch.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AirHockey_Touch_h
#define AirHockey_Touch_h

class Touch {
 private:
  CGPoint location_;
  void *identifier_;

 public:
  CGPoint getLocation() const { return location_; }
  void setLocation(CGPoint location) { location_ = location; }
  void *getIdentifier() const { return identifier_; }
  void setIdentifier(void *identifier) { identifier_ = identifier; }
};

#endif
