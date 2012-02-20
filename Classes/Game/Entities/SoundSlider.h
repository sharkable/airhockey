//
//  SoundSlider.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-06.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "StateEntity.h"
#import "Texture2D.h"

@interface SoundSlider : NSObject <StateEntity> {
	CGPoint    _position;
	Texture2D* _emptyTexture;
	Texture2D* _fullTexture;
	Texture2D* _thumbTexture;
	double     _value;
	UITouch*   _grabbedTouch;
	CGPoint    _lastTouchPosition;
}

- (id) initWithPosition:(CGPoint)position;

@property (readonly) CGPoint thumbPoint;

@end
