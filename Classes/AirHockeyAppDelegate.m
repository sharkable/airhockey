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

@implementation AirHockeyAppDelegate

@synthesize glViewController = glViewController_;

- (void)dealloc {
  [window_ release];
  [glViewController_ release];
  
  [super dealloc];
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

#pragma mark - UIApplicationDelegate

@synthesize window = window_;

- (BOOL)application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
  NSString *appVersion = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"];
  [FlurryAnalytics setAppVersion:appVersion];
  if (IS_FREE) {
    [FlurryAnalytics startSession:@"BGGPH5B2THWFSJHXEKRH"];
  } else {
    [FlurryAnalytics startSession:@"4HECR4PRJJP4ZSLZ2EJB"];
  }
  
  glViewController_ = [[EAGLViewController alloc] init];
  
  CGRect screenSize = [[UIScreen mainScreen] bounds];
  window_ = [[UIWindow alloc] initWithFrame:screenSize];
  [window_ addSubview:glViewController_.view];
  [window_ makeKeyAndVisible];

  [self startGame];

  return YES;
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

@end
