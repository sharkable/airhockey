//
//  EAGLViewController.h
//  AirHockeyFree
//
//  Created by Jonathan Sharkey on 10-05-12.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <UIKit/UIKit.h>

@class GameEngine;

@interface EAGLViewController : UIViewController

@property(nonatomic, assign) GameEngine *gameEngine;

- (void)startAnimation;
- (void)stopAnimation;

@end
