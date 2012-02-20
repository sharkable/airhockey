//
//  Rink.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-13.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "Rink.h"
#import "Puck.h"
#import "SoundPlayer.h"
#import "const.h"

@implementation Rink

- (id) init {
	[super init];
	
	_texture = [[ResourceLoader instance] getTextureWithName:@"rink_bg"];
	_renderPoint = CGPointMake(0, 0);
	return self;
}

- (void) dealloc {
	[[ResourceLoader instance] releaseResource:_texture];
	
	[super dealloc];
}

- (void) update {
}

- (void) render {
	[_texture drawAtPoint:_renderPoint];
}

- (void) bounceOff:(RoundThing*)thing {
	if (thing.grabbed || !thing.movable) {
		return;
	}
	
	BOOL dampen = NO;
	if (thing.x + thing.radius > RINK_RIGHT_X) {
		thing.vx = -thing.vx;
		thing.x = 2*(RINK_RIGHT_X - thing.radius) - thing.x;
		dampen = YES;
	}
	if (thing.x - thing.radius < RINK_LEFT_X) {
		thing.vx = -thing.vx;
		thing.x = 2*(RINK_LEFT_X + thing.radius) - thing.x;
		dampen = YES;
	}
	if (thing.y + thing.radius > RINK_BOTTOM_Y) {
		// Interpolate circle x (hx) when the circle reached the edge of the goal.
		double oldX = thing.x - thing.vx;
		double oldY = thing.y - thing.vy;
		if (oldY + thing.radius <= RINK_BOTTOM_Y) {
			double topFraction = (thing.y - (RINK_BOTTOM_Y - thing.radius)) / (thing.y - oldY);
			double hx = thing.x - (thing.x - oldX) * topFraction;
			if (hx < GOAL_LEFT_X || hx >= GOAL_RIGHT_X) {
				thing.vy = -thing.vy;
				thing.y = 2*(RINK_BOTTOM_Y - thing.radius) - thing.y;
				dampen = YES;				
			}
		} else if (thing.vx < 0 && thing.x - thing.radius < GOAL_LEFT_X) {
			double outsideFraction = ((GOAL_LEFT_X + thing.radius) - thing.x) / (oldX - thing.x);
			double hy = thing.y - (thing.y - oldY) * outsideFraction;
			if (hy >= RINK_BOTTOM_Y) {
				thing.vx = -thing.vx;
				thing.x = 2*(GOAL_LEFT_X + thing.radius) - thing.x;
				dampen = YES;					
			}
		} else if (thing.vx > 0 && thing.x + thing.radius > GOAL_RIGHT_X) {
			double outsideFraction = (thing.x - (GOAL_RIGHT_X - thing.radius)) / (thing.x - oldX);
			double hy = thing.y - (thing.y - oldY) * outsideFraction;
			if (hy >= RINK_BOTTOM_Y) {
				thing.vx = -thing.vx;
				thing.x = 2*(GOAL_RIGHT_X - thing.radius) - thing.x;
				dampen = YES;					
			}
		}
	}
	if (thing.y - thing.radius < RINK_TOP_Y) {
		// Interpolate circle x (hx) when the circle reached the edge of the goal.
		double oldX = thing.x - thing.vx;
		double oldY = thing.y - thing.vy;
		if (oldY - thing.radius >= RINK_TOP_Y) {
			double topFraction = ((RINK_TOP_Y + thing.radius) - thing.y) / (oldY - thing.y);
			double hx = thing.x - (thing.x - oldX) * topFraction;
			if (hx < GOAL_LEFT_X || hx >= GOAL_RIGHT_X) {
				thing.vy = -thing.vy;
				thing.y = 2*(RINK_TOP_Y + thing.radius) - thing.y;
				dampen = YES;				
			}
		} else if (thing.vx < 0 && thing.x - thing.radius < GOAL_LEFT_X) {
			double outsideFraction = ((GOAL_LEFT_X + thing.radius) - thing.x) / (oldX - thing.x);
			double hy = thing.y - (thing.y - oldY) * outsideFraction;
			if (hy <= RINK_TOP_Y) {
				thing.vx = -thing.vx;
				thing.x = 2*(GOAL_LEFT_X + thing.radius) - thing.x;
				dampen = YES;					
			}
		} else if (thing.vx > 0 && thing.x + thing.radius > GOAL_RIGHT_X) {
			double outsideFraction = (thing.x - (GOAL_RIGHT_X - thing.radius)) / (thing.x - oldX);
			double hy = thing.y - (thing.y - oldY) * outsideFraction;
			if (hy <= RINK_TOP_Y) {
				thing.vx = -thing.vx;
				thing.x = 2*(GOAL_RIGHT_X - thing.radius) - thing.x;
				dampen = YES;					
			}
		}
	}
	
	if (dampen) {
		thing.vx *= 0.7;
		thing.vy *= 0.7;
		if ([thing isKindOfClass:[Puck class]]) {
			[SoundPlayer playSound:kSoundPuckRinkBounce];
		}
	}	
}

- (void) moveInFromEdge:(RoundThing*)thing {
	if (!thing.movable) {
		return;
	}
	if (thing.x - thing.radius < RINK_LEFT_X) {
		thing.x = RINK_LEFT_X + thing.radius;
	} else if (thing.x + thing.radius > RINK_RIGHT_X) {
		thing.x = RINK_RIGHT_X - thing.radius;
	}
	if (thing.y - thing.radius < RINK_TOP_Y && (thing.x < GOAL_LEFT_X || thing.x >= GOAL_RIGHT_X)) {
		thing.y = RINK_TOP_Y + thing.radius;
	} else if (thing.y + thing.radius > RINK_BOTTOM_Y && (thing.x < GOAL_LEFT_X || thing.x >= GOAL_RIGHT_X)) {
		thing.y = RINK_BOTTOM_Y - thing.radius;
	}
}

@end
