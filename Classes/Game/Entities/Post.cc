//
//  Post.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-25.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "Post.h"
#import "Rink.h"

Post::Post(double x, double y) {
// TODO  super();

  setX(x);
  setY(y);
  setRadius(1);
  setMass(99999999999.99);  
}

bool Post::isMovable() {
  return false;
}
