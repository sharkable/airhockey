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
  
  _array = [[NSMutableArray alloc] init];
  
  return self;
}

- (void) push:(id)object {
  [_array addObject:object];
}

- (id) pop {
  id object = [_array lastObject];
  [_array removeLastObject];
  return object;
}

- (id) top {
  return [_array lastObject];
}

- (id) objectAtIndex:(int)index {
  return [_array objectAtIndex:index];
}

- (int) count {
  return _array.count;
}

@end
