//
//  SoundSlider.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-06.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "SoundSlider.h"
#import "SoundPlayer.h"
#import "ResourceLoader.h"
#import "LocalStore.h"
#import "const.h"

@implementation SoundSlider

- (id) initWithPosition:(CGPoint)position {
	[super init];
	
	_position = position;
	
	_emptyTexture = [[ResourceLoader instance] getTextureWithName:@"sound_empty"];
	_fullTexture  = [[ResourceLoader instance] getTextureWithName:@"sound_full"];
	_thumbTexture = [[ResourceLoader instance] getTextureWithName:@"sound_thumb"];
	
	if ([LocalStore hasEntryForKey:LS_VOLUME]) {
		_value = [LocalStore doubleForKey:LS_VOLUME];
	} else {
		_value = 0.75;
	}
	[SoundPlayer setGlobalVolume:_value];
	
	return self;
}

- (void) update {
}

- (void) render {
	double drawRatio = (269.0 - _thumbTexture.contentSize.width)/320.0 * _value + (19.0 + _thumbTexture.contentSize.width/2)/320.0;
	[_fullTexture drawAtPoint:_position leftRatio:drawRatio];
	[_emptyTexture drawAtPoint:_position rightRatio:1.0-drawRatio];
	[_thumbTexture drawAtPoint:self.thumbPoint];
}

- (CGPoint) thumbPoint {
	return CGPointMake(_position.x + 19 + (269.0 - _thumbTexture.contentSize.width)*_value, _position.y);
}

- (void) touchesBegan:(Touch*[])touches numTouches:(int)numTouches {
	for (int i = 0; i < numTouches; i++) {
		CGPoint touchP = touches[i].location;
		CGPoint thumbP = [self thumbPoint];
		double thumbWidth = _thumbTexture.contentSize.width;
		double thumbHeight = _thumbTexture.contentSize.height;
		if (touchP.x >= thumbP.x - thumbWidth && touchP.y >= thumbP.y - thumbHeight &&
					touchP.x < thumbP.x + 2 * thumbWidth &&
					touchP.y < thumbP.y + 2 * thumbHeight) {
			_grabbedTouch = touches[i].identifier;
			_lastTouchPosition = touchP;
			return;
		}
	}
}

- (void) touchesMoved:(Touch*[])touches numTouches:(int)numTouches {
	for (int i = 0; i < numTouches; i++) {
		if (touches[i].identifier == _grabbedTouch) {
			CGPoint touchP = touches[i].location;
			_value += (touchP.x - _lastTouchPosition.x) / (269 - _thumbTexture.contentSize.width);
			_lastTouchPosition = touchP;
			if (_lastTouchPosition.x < _position.x + 19) {
				_lastTouchPosition.x = _position.x + 10;
			} else if (_lastTouchPosition.x >= _position.x + 19 + (269.0 - _thumbTexture.contentSize.width)) {
				_lastTouchPosition.x = _position.x + 19 + (269.0 - _thumbTexture.contentSize.width) - 1;
			}
			if (_value < 0.0) {
				_value = 0.0;
			} else if (_value > 1.0) {
				_value = 1.0;
			}
			return;
		}
	}
}

- (void) touchesEnded:(Touch*[])touches numTouches:(int)numTouches {
	for (int i = 0; i < numTouches; i++) {
		if (touches[i].identifier == _grabbedTouch) {
			[SoundPlayer setGlobalVolume:_value];
			[LocalStore setDouble:_value forKey:LS_VOLUME];
			_grabbedTouch = nil;
			return;
		}
	}
}

@end
