//
//  GameEngine.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "GameEngine.h"
#import "IsFree.h"
#import "const.h"

@implementation GameEngine

+ (GameEngine*) instance {
	static GameEngine* __instance = nil;
	if (__instance == nil) {
		__instance = [[GameEngine alloc] init];
	}
	return __instance;
}

- (id) init {
	[super init];
	
	init_genrand(time(NULL));
	
	_states = [[Stack alloc] init];
	for (int i = 0; i < MAX_TOUCHES; i++) {
		_touchesBegan[i] = [[Touch alloc] init];
		_touchesMoved[i] = [[Touch alloc] init];
		_touchesEnded[i] = [[Touch alloc] init];
	}
	
	return self;
}

- (void) dealloc {
	[_states release];
	for (int i = 0; i < MAX_TOUCHES; i++) {
		[_touchesBegan[i] release];
		[_touchesMoved[i] release];
		[_touchesEnded[i] release];
	}
	
	[super dealloc];
}

- (void) render {
	for (int i = 0; i < _states.count; i++) {
		EngineState* state = [_states objectAtIndex:i];
		[state render];
	}
}

- (void) update {
	if (_popOnNext) {
		[_states pop];
		[[_states top] stateIsShown];
		_popOnNext = NO;
	} else if (_replaceOnNext) {
		[_states pop];
		[_states push:_nextState];
		[_nextState release];
		[_nextState stateIsShown];
		_replaceOnNext = NO;
		_nextState = nil;
	}
	
	// Process input.
	EngineState* topState = [_states top];
	if (_numTouchesBegan > 0) {
		[topState touchesBegan:_touchesBegan numTouches:_numTouchesBegan];
		_numTouchesBegan = 0;
	}
	if (_numTouchesMoved > 0) {
		[topState touchesMoved:_touchesMoved numTouches:_numTouchesMoved];
		_numTouchesMoved = 0;
	}
	if (_numTouchesEnded > 0) {
		[topState touchesEnded:_touchesEnded numTouches:_numTouchesEnded];
		_numTouchesEnded = 0;
	}
	
	// Update states.
	for (int i = 0; i < _states.count; i++) {
		EngineState* state = [_states objectAtIndex:i];
		[state update];
	}
}

- (void) pushState:(EngineState*)state {
	[_states push:state];
	[state stateIsShown];
}

- (void) popState {
	_popOnNext = YES;
}

- (void) replaceTopState:(EngineState*)state {
	_replaceOnNext = YES;
	if (_nextState != state) {
		[_nextState release];
		_nextState = state;
		[_nextState retain];
	}
}

- (void) setTouchesBegan:(NSSet*) touches {
	_numTouchesBegan = touches.count;
	int i = 0;
	for (UITouch* touch in touches) {
		_touchesBegan[i].location = [touch locationInView:touch.view];
		if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
			CGPoint p = _touchesBegan[i].location;
			p.x *= 768.0/320.0;
			p.y = (p.y - (IS_FREE ? 53 : 26)) * (768.0/320.0);
			_touchesBegan[i].location = p;
		}
		_touchesBegan[i].identifier = touch;
		i++;
	}
}

- (void) setTouchesMoved:(NSSet*) touches {
	_numTouchesMoved = touches.count;
	int i = 0;
	for (UITouch* touch in touches) {
		_touchesMoved[i].location = [touch locationInView:touch.view];
		if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
			CGPoint p = _touchesMoved[i].location;
			p.x *= 768.0/320.0;
			p.y = (p.y - (IS_FREE ? 53 : 26)) * (768.0/320.0);
			_touchesMoved[i].location = p;
		}
		_touchesMoved[i].identifier = touch;
		i++;
	}
}

- (void) setTouchesEnded:(NSSet*) touches {
	_numTouchesEnded = touches.count;
	int i = 0;
	for (UITouch* touch in touches) {
		_touchesEnded[i].location = [touch locationInView:touch.view];
		if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
			CGPoint p = _touchesEnded[i].location;
			p.x *= 768.0/320.0;
			p.y = (p.y - (IS_FREE ? 53 : 26)) * (768.0/320.0);
			_touchesEnded[i].location = p;
		}
		_touchesEnded[i].identifier = touch;
		i++;
	}
}

- (void) clearTouches {
	for (int i = 0; i < _states.count; i++) {
		EngineState* state = [_states objectAtIndex:i];
		[state clearTouches];
	}
}

@end
