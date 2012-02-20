//
//  SimpleItem.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "SimpleItem.h"
#import "ResourceLoader.h"

@implementation SimpleItem

@synthesize texture=_texture, position=_position, angle=_angle;

- (id) initWithTexture:(Texture2D*)texture position:(CGPoint)position {
	[super init];
	
	_textures = [[NSMutableArray alloc] init];
	[_textures addObject:texture];
	_texture = 0;
	_angle = 0;
	
	_position = position;
	
	return self;
}

- (id) initWithTextures:(NSArray*)textures position:(CGPoint)position {
	[super init];
	
	_textures = [[NSMutableArray alloc] initWithArray:textures];
	_texture = 0;
	_angle = 0;
	
	_position = position;
	
	return self;
}

- (void) dealloc {
	for (Texture2D* texture in _textures) {
		[[ResourceLoader instance] releaseResource:texture];
	}
	[_textures release];
	
	[super dealloc];
}

- (void) update {
}

- (void) render {
	Texture2D* t = [_textures objectAtIndex:_texture];
	[t drawAtPoint:CGPointMake(_position.x, _position.y)
			 angle:_angle];
}

- (CGSize) size {
	Texture2D* t = [_textures objectAtIndex:_texture];
	return t.contentSize;
}

@end
