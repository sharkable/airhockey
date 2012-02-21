//
//  AirHockeyFreeAppDelegate.h
//  AirHockeyFree
//
//  Created by Jonathan Sharkey on 10-05-07.
//  Copyright Sharkable 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "EAGLViewController.h"

@interface AirHockeyFreeAppDelegate : NSObject<UIApplicationDelegate> {
 @private
  UIWindow *window_;
  EAGLViewController *glViewController_;
}

- (void)startGame;
- (void)initAudio:(id)delegate;

@property(nonatomic, retain) UIWindow* window;
@property(nonatomic, retain) EAGLViewController *glViewController;

@end
