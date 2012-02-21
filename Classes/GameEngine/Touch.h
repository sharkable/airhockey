//
//  Touch.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface Touch : NSObject {
  CGPoint  _location;
  UITouch* _identifier;
}

@property (assign) CGPoint  location;
@property (assign) UITouch* identifier;

@end
