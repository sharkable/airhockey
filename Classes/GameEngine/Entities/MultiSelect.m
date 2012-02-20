//
//  MultiSelect.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "MultiSelect.h"
#import "SoundPlayer.h"
#import "ResourceLoader.h"

@implementation MultiSelect

@synthesize selectedValue=_selectedValue;

- (id) init {
	[super init];
	
	_normalTextures = [[NSMutableArray alloc] init];
	_selectedTextures = [[NSMutableArray alloc] init];
	_positionsX = [[NSMutableArray alloc] init];
	_positionsY = [[NSMutableArray alloc] init];
	
	return self;
}

- (void) dealloc {
	for (Texture2D* texture in _normalTextures) {
		[[ResourceLoader instance] releaseResource:texture];
	}
	for (Texture2D* texture in _selectedTextures) {
		[[ResourceLoader instance] releaseResource:texture];
	}
	
	[_normalTextures release];
	[_selectedTextures release];
	[_positionsX release];
	[_positionsY release];
	
	[super dealloc];
}

- (void) update {
}

- (void) render {
	for (int i = 0; i < _normalTextures.count; i++) {
		CGPoint p = CGPointMake([[_positionsX objectAtIndex:i] doubleValue], 
								[[_positionsY objectAtIndex:i] doubleValue]);
		if (i == _selectedValue) {
			[[_selectedTextures objectAtIndex:i] drawAtPoint:p];
		} else {
			[[_normalTextures objectAtIndex:i] drawAtPoint:p];	
		}
	}
}

- (void) addValueWithNormalTexture:(Texture2D*)normalTexture selectedTexture:(Texture2D*)selectedTexture position:(CGPoint)position {
	[_normalTextures addObject:normalTexture];
	[_selectedTextures addObject:selectedTexture];
	[_positionsX addObject:[NSNumber numberWithDouble:position.x]];
	[_positionsY addObject:[NSNumber numberWithDouble:position.y]];
}

- (void) touchesBegan:(Touch*[])touches numTouches:(int)numTouches {
	for (int i = 0; i < _normalTextures.count; i++) {
		double x = [[_positionsX objectAtIndex:i] doubleValue];
		double y = [[_positionsY objectAtIndex:i] doubleValue];
		CGSize size = ((Texture2D*)[_normalTextures objectAtIndex:i]).contentSize;
		for (int j = 0; j < numTouches; j++) {
			CGPoint touchPoint = touches[j].location;
			if (touchPoint.x >= x && touchPoint.y >= y && touchPoint.x < x + size.width && touchPoint.y < y + size.height) {
				if (_selectedValue != i) {
					_selectedValue = i;
					[SoundPlayer playSound:kSoundMultiSelect];
				};
				return;
			}
		}
	}
}

@end
