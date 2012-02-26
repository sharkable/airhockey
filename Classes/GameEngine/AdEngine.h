//
//  AdEngine.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 2/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@class GameEngine;

@interface AdEngine : NSObject

@property(nonatomic, assign) GameEngine *gameEngine;

- (void)addAdAtPoint:(CGPoint)point;
- (void)removeAd;

@end
