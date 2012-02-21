//
//  Stack.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface Stack : NSObject {
  NSMutableArray* array_;
}

- (void) push:(id)object;
- (id) pop;
- (id) top;
- (id) objectAtIndex:(int)index;

@property (readonly) int count;

@end
