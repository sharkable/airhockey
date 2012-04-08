//
//  Post.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-25.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AirHockey_Post_h
#define AirHockey_Post_h

#import "RoundThing.h"

class Post : public RoundThing {
 public:
  Post(double x, double y);
  virtual bool isMovable();
};

#endif
