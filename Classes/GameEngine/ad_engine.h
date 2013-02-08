//
//  AdEngine.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 2/25/12.
//  Copyright (c) 2012 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "Texture2D.h"

class GameEngine;

@interface AdEngine : NSObject

@property(nonatomic, assign) GameEngine *gameEngine;

- (void)addAdAtPoint:(ScreenPoint)point;
- (void)removeAd;

@end
