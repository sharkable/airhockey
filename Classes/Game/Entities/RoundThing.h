//
//  RoundThing.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-14.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "StateEntity.h"
#import "Texture2D.h"
#import "ResourceLoader.h"

@interface RoundThing : NSObject <StateEntity> {
	Texture2D* _texture;
	double   _x;
	double   _y;
	double   _oldX;
	double   _oldY;
	double   _vx;
	double   _vy;
	double   _radius;
	double   _mass;
	double   _friction;
	BOOL     _grabbed;
	UITouch* _grabbedTouch;
	BOOL     _active;
}

- (void) applyFriction;
- (void) bounceOff:(RoundThing*)other;
- (BOOL) containsTouch:(Touch*)touch;
- (BOOL) overlaps:(RoundThing*)thing;

@property (assign)   double x;
@property (assign)   double y;
@property (assign)   double vx;
@property (assign)   double vy;
@property (readonly) double radius;
@property (readonly) double mass;
@property (readonly) double friction;
@property (readonly) BOOL   grabbable;
@property (readonly) BOOL   grabbed;
@property (readonly) BOOL   movable;
@property (assign)   BOOL   active;

@end
