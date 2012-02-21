//
//  Touch.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "Touch.h"

@implementation Touch

@synthesize location = location_;
@synthesize identifier = identifier_;

- (void)dealloc {
  [identifier_ release];
  
  [super dealloc];
}

@end
