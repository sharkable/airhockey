//
//  RoundThing.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-14.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "RoundThing.h"
#import "SoundPlayer.h"
#import "Paddle.h"
#import "Puck.h"

@implementation RoundThing

@synthesize x=_x, y=_y, vx=_vx, vy=_vy, radius=_radius, mass=_mass, friction=_friction, active=_active;

- (id) init {
	[super init];
	
	_active = YES;
	
	return self;
}

- (void) dealloc {
	[[ResourceLoader instance] releaseResource:_texture];
	
	[super dealloc];
}

- (void) update {
	if (!self.movable || !self.active) {
		return;
	}
	
	if (!_grabbed) {
		_x += _vx;
		_y += _vy;
	} else {		
		_vx = _vx * 0.75 + (_x - _oldX) * 0.25;
		_vy = _vy * 0.75 + (_y - _oldY) * 0.25;
		_oldX = _x;
		_oldY = _y;
	}
}

- (void) applyFriction {
	if (!self.grabbed) {
		_vx *= _friction;
		_vy *= _friction;
	}
}

- (void) render {
	if (_active) {
		[_texture drawAtPoint:CGPointMake(_x - _texture.contentSize.width/2, _y - _texture.contentSize.height/2)];
	}
}

- (void) bounceOff:(RoundThing*)other {
	// TODO optimize this function.
	// For now I'm just getting it to work.
	
	double dxFull = self.x - other.x;
	double dyFull = self.y - other.y;
	double radiusTotal = other.radius + self.radius;
//	double v = sqrt(self.vx * self.vx + self.vy * self.vy);
//	double otherV = sqrt(other.vx * other.vx + other.vy * other.vy);
	
	if (dxFull*dxFull + dyFull*dyFull <= radiusTotal*radiusTotal) {		
		// Normalize N
		double nL = sqrt(dxFull*dxFull + dyFull*dyFull);
		double dx = dxFull / nL;
		double dy = dyFull / nL;
		
		// *** Move the round things outside of each other. ***
		double vFraction = self.mass / (other.mass + self.mass);
		
		if ((self.grabbed && !other.grabbed && other.movable) || !self.movable) {
			vFraction = 1;
		} else if ((!self.grabbed && other.grabbed) || !other.movable) {
			vFraction = 0;
		}
		
		double diff = sqrt(radiusTotal*radiusTotal) - nL;
		other.x -= dx * diff * vFraction;
		other.y -= dy * diff * vFraction;
		vFraction = 1 - vFraction;
		self.x += dx * diff * vFraction;
		self.y += dy * diff * vFraction;
		
		// *** Now change the direction based on the bounce. ***
		
		// Based on this: http://stackoverflow.com/questions/573084/how-to-calculate-bounce-angle
		// But it had some problems.
		// Looked at Wikipedia dot product article to help.
		double vDn = self.vx*dx + self.vy*dy;
		double ux = dx * vDn;
		double uy = dy * vDn;
		double wx = self.vx - ux;
		double wy = self.vy - uy;
		
		double other_vDn = other.vx*dx + other.vy*dy;
		double other_ux = dx * other_vDn;
		double other_uy = dy * other_vDn;
		double other_wx = other.vx - other_ux;
		double other_wy = other.vy - other_uy;
		
		double new_ux = (ux * (self.mass - other.mass) + 2.0 * other.mass * other_ux) / (self.mass + other.mass);
		double new_uy = (uy * (self.mass - other.mass) + 2.0 * other.mass * other_uy) / (self.mass + other.mass);	
		
		double new_other_ux = (other_ux * (other.mass - self.mass) + 2.0 * self.mass * ux) / (self.mass + other.mass);
		double new_other_uy = (other_uy * (other.mass - self.mass) + 2.0 * self.mass * uy) / (self.mass + other.mass);		
		
//		if (!self.grabbed && !other.grabbed) {
//			NSLog(@"\n");
//			NSLog(@"Before total: %f", v * self.mass + otherV * other.mass);
//		}
		
		if (!self.grabbed && self.movable) {
			self.vx = new_ux + wx;
			self.vy = new_uy + wy;
		}
		
		if (!other.grabbed && other.movable) {
			other.vx = new_other_ux + other_wx;
			other.vy = new_other_uy + other_wy;
		}

		BOOL onePaddle = [self isKindOfClass:[Paddle class]] || [other isKindOfClass:[Paddle class]];
		BOOL twoPucks  = [self isKindOfClass:[Puck class]] && [other isKindOfClass:[Puck class]];
		BOOL onePuck   = !twoPucks && ([self isKindOfClass:[Puck class]] || [other isKindOfClass:[Puck class]]);
		
		if (onePaddle && onePuck) {
			[SoundPlayer playSound:kSoundPaddleHit];
		} else if (onePuck) {
			// The puck hit a post.
			[SoundPlayer playSound:kSoundPuckRinkBounce];
		} else if (twoPucks) {
			[SoundPlayer playSound:kSoundTwoPuckHit];
		}
		
		double newVSquared = self.vx * self.vx + self.vy * self.vy;
		double newOtherVSquared = other.vx * other.vx + other.vy * other.vy;
		
		if (newVSquared > (MAX_SPEED*MAX_SPEED)) {
			double newV = sqrt(newVSquared);
			double newRatio = MAX_SPEED / newV;
			self.vx *= newRatio;
			self.vy *= newRatio;
		}
		if (newOtherVSquared > (MAX_SPEED*MAX_SPEED)) {
			double newOtherV = sqrt(newOtherVSquared);
			double newRatio = MAX_SPEED / newOtherV;
			other.vx *= newRatio;
			other.vy *= newRatio;
		}		
	}	
}


- (void) touchesBegan:(Touch*[])touches numTouches:(int)numTouches {
	if (!self.grabbable || !self.active || self.grabbed) {
		return;
	}
	for (int i = 0; i < numTouches; i++) {
		if ([self containsTouch:touches[i]]) {
			_grabbed = YES;
			_grabbedTouch = touches[i].identifier;
			[self touchesMoved:touches numTouches:numTouches];
			_vx = 0;
			_vy = 0;
			// Set _oldX and _oldY here so that the velocity stays around 0.
			// This is when you touch the outside of the RoundThing and it
			// snaps to center on your touch, it doesn't have a really high
			// initial velocity.
			_oldX = _x;
			_oldY = _y;			
		}
	}
}

- (void) touchesMoved:(Touch*[])touches numTouches:(int)numTouches {
	Touch* correctTouch = nil;
	for (int i = 0; i < numTouches; i++) {
		if (touches[i].identifier == _grabbedTouch) {
			correctTouch = touches[i];
			break;
		}
	}
	if (_grabbed && correctTouch != nil) {
		CGPoint p = correctTouch.location;
		_x = p.x;
		_y = p.y;
	}
}

- (void) touchesEnded:(Touch*[])touches numTouches:(int)numTouches {
	Touch* correctTouch = nil;
	for (int i = 0; i < numTouches; i++) {
		if (touches[i].identifier == _grabbedTouch) {
			correctTouch = touches[i];
			break;
		}
	}
	if (_grabbed && correctTouch != nil) {
		_grabbed = NO;
		_grabbedTouch = nil;
	}
}

- (void) clearTouches {
	_grabbed = NO;
	_grabbedTouch = nil;
}

- (BOOL) grabbable {
	return NO;
}

- (BOOL) grabbed {
	return _grabbed;
}

- (BOOL) movable {
	return YES;
}
	 
- (BOOL) containsTouch:(Touch*)touch {
	double dx = touch.location.x - _x;
	double dy = touch.location.y - _y;
	return (dx*dx + dy*dy <= self.radius*self.radius);
}

- (BOOL) overlaps:(RoundThing*)thing {
	double dx = thing.x - _x;
	double dy = thing.y - _y;
	double totalRadius = thing.radius + _radius;
	return (dx*dx + dy*dy <= totalRadius*totalRadius);
}

@end
