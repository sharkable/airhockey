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
  Texture2D* texture_;
  double   _x;
  double   _y;
  double   oldX_;
  double   oldY_;
  double   vx_;
  double   vy_;
  double   radius_;
  double   mass_;
  double   friction_;
  BOOL     grabbed_;
  UITouch* grabbedTouch_;
  BOOL     active_;
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
