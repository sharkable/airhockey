//
//  GameTouchWindow.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 2/26/12.
//  Copyright (c) 2012 Sharkable. All rights reserved.
//

#import "GameTouchWindow.h"

#import "GameEngine.h"

@implementation GameTouchWindow {
 @private
  GameEngine *gameEngine_;  // weak
}

@synthesize gameEngine = gameEngine_;

#pragma mark - UIWindow

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
  [gameEngine_ setTouchesBegan:touches];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  [gameEngine_ setTouchesMoved:touches];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
  [gameEngine_ setTouchesEnded:touches];
}

@end
