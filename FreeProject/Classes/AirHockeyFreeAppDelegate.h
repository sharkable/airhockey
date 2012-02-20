//
//  AirHockeyFreeAppDelegate.h
//  AirHockeyFree
//
//  Created by Jonathan Sharkey on 10-05-07.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class EAGLView;

@interface AirHockeyFreeAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    EAGLView *glView;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet EAGLView *glView;

@end

