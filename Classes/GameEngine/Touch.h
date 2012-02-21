//
//  Touch.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Touch : NSObject {
 @private
  CGPoint location_;
  UITouch *identifier_;
}

@property(nonatomic, assign) CGPoint location;
@property(nonatomic, retain) UITouch *identifier;

@end
