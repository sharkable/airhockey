//
//  EngineState.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "EngineState.h"
#import "GameEngine.h"

@implementation EngineState

- (id) init {
	[super init];
	
	_entities = [[NSMutableArray alloc] init];
	
	return self;
}

- (void) dealloc {
	[_entities release];
	
	[super dealloc];
}

- (void) stateIsShown {
}

- (void) update {
	for (id <StateEntity> entity in _entities) {
		[entity update];
	}
}

- (void) render {
	for (id <StateEntity> entity in _entities) {
		[entity render];
	}
}

- (void) addEntity:(id <StateEntity>)entity {
	[_entities addObject:entity];
}

- (void) removeEntity:(id <StateEntity>)entity {
	[_entities removeObject:entity];
}

- (void) touchesBegan:(Touch*[])touches numTouches:(int)numTouches {
	for (NSObject <StateEntity>* entity in _entities) {
		if ([entity respondsToSelector:@selector(touchesBegan:numTouches:)]) {
			[entity touchesBegan:touches numTouches:numTouches];
		}
	}
}

- (void) touchesMoved:(Touch*[])touches numTouches:(int)numTouches {
	for (NSObject <StateEntity>* entity in _entities) {
		if ([entity respondsToSelector:@selector(touchesMoved:numTouches:)]) {
			[entity touchesMoved:touches numTouches:numTouches];
		}
	}
}

- (void) touchesEnded:(Touch*[])touches numTouches:(int)numTouches {
	for (NSObject <StateEntity>* entity in _entities) {
		if ([entity respondsToSelector:@selector(touchesEnded:numTouches:)]) {
			[entity touchesEnded:touches numTouches:numTouches];
		}
	}
}

- (void) clearTouches {
	for (NSObject <StateEntity>* entity in _entities) {
		if ([entity respondsToSelector:@selector(clearTouches)]) {
			[entity clearTouches];
		}
	}
}

@end
