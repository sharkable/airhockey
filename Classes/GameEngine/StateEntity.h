//
//  StateEntity.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "Touch.h"

@protocol StateEntity

- (void)update;
- (void)render;

@optional

- (void)touchesBegan:(Touch *[])touches numTouches:(int)numTouches;
- (void)touchesMoved:(Touch *[])touches numTouches:(int)numTouches;
- (void)touchesEnded:(Touch *[])touches numTouches:(int)numTouches;
- (void)clearTouches;

@end
