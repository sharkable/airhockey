//
//  AirHockeyFreeAppDelegate.m
//  AirHockeyFree
//
//  Created by Jonathan Sharkey on 10-05-07.
//  Copyright Sharkable 2010. All rights reserved.
//

#import "AirHockeyFreeAppDelegate.h"
#import "EAGLView.h"
#import "FlurryAnalytics.h"
#import "GameEngine.h"
#import "IsFree.h"
#import "SoundPlayer.h"
#import "SplashState.h"

@implementation AirHockeyFreeAppDelegate

@synthesize window = window_;
@synthesize glViewController = glViewController_;

- (BOOL)application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
  // TODO: Set app version.
  if (IS_FREE) {
    [FlurryAnalytics startSession:@"BGGPH5B2THWFSJHXEKRH"];
  } else {
    [FlurryAnalytics startSession:@"4HECR4PRJJP4ZSLZ2EJB"];
  }
  
  CGRect screenSize = [[UIScreen mainScreen] bounds];
  self.window = [[UIWindow alloc] initWithFrame:screenSize];
  
  self.glViewController = [[EAGLViewController alloc] init];
  [self.window addSubview:glViewController_.view];
  
  [self startGame];
  
  [window_ makeKeyAndVisible];
  
  return YES;
}

- (void)startGame {
  SplashState *rootState = [[[SplashState alloc] init] autorelease];
  [[GameEngine instance] pushState:rootState];
  [NSThread detachNewThreadSelector:@selector(initAudio:) toTarget:self withObject:rootState];
  [self.glViewController startAnimation];
}  

- (void)initAudio:(id)delegate {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  
  [SoundPlayer syncAudioSessionForITunes];
  [SoundPlayer initializeWithDelegate:delegate];
  [SoundPlayer setSoundEffectsOn:YES];
  
  [pool release];
}  

- (void)applicationWillResignActive:(UIApplication *)application {
  [glViewController_ stopAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
  [glViewController_ startAnimation];
  [[GameEngine instance] clearTouches];
}

- (void)applicationWillTerminate:(UIApplication *)application {
  [glViewController_ stopAnimation];
}

- (void)dealloc {
  [window_ release];
  [glViewController_ release];
  
  [super dealloc];
}

@end
