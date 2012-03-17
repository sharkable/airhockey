//
//  Stack.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "Stack.h"

@implementation Stack

- (id) init {
  [super init];
  
  if (self) {
    array_ = [[NSMutableArray alloc] init];
  }
  
  return self;
}

- (void)push:(id)object {
  [array_ addObject:object];
}

- (id)pop {
  id object = [array_ lastObject];
  [array_ removeLastObject];
  return object;
}

- (id)top {
  return [array_ lastObject];
}

- (id)objectAtIndex:(int)index {
  return [array_ objectAtIndex:index];
}

- (int)count {
  return [array_ count];
}

@end
