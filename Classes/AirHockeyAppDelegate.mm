//
//  AirHockeyAppDelegate.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-07.
//  Copyright Sharkable 2010. All rights reserved.
//

#import "AirHockeyAppDelegate.h"
#import "EAGLView.h"
#import "FlurryAnalytics.h"
#import "GameEngine.h"
#import "SoundPlayer.h"
#import "SplashState.h"
#import "ViewController.h"

@interface AirHockeyAppDelegate ()
- (void)initAudio:(id)delegate;
@end

@implementation AirHockeyAppDelegate {
 @private
  ViewController *viewController_;
}

- (void)dealloc {
  [viewController_ release];

  [super dealloc];
}

- (void)initAudio:(id)delegate {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  
  [SoundPlayer syncAudioSessionForITunes];
  [SoundPlayer initializeWithDelegate:delegate];
  [SoundPlayer setSoundEffectsOn:YES];
  
  [pool release];
}  

#pragma mark - UIApplicationDelegate

- (BOOL)application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
  NSString *appVersion = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"];
  [FlurryAnalytics setAppVersion:appVersion];
  if (IS_FREE) {
    [FlurryAnalytics startSession:@"BGGPH5B2THWFSJHXEKRH"];
  } else {
    [FlurryAnalytics startSession:@"4HECR4PRJJP4ZSLZ2EJB"];
  }
  
  viewController_ = [[ViewController alloc] init];
  
  return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
  [viewController_ stop];
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
  [viewController_ start];
  viewController_.game_engine->clearTouches();
}

- (void)applicationWillTerminate:(UIApplication *)application {
  [viewController_ stop];
}

- (UIWindow *)window {
  return viewController_.window;
}

@end
