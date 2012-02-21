//
//  Post.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-25.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "Post.h"
#import "Rink.h"

@implementation Post

- (id) initWithX:(double)x y:(double)y {
  [super init];

  _x = x;
  _y = y;
  _radius = 1;
  _mass = 99999999999.99;
  
  return self;
}

- (void) render {
}

- (BOOL) movable {
  return NO;
}

@end
