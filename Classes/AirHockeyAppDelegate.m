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

@interface AirHockeyAppDelegate ()
- (void)startGame;
- (void)initAudio:(id)delegate;
@end

@implementation AirHockeyAppDelegate {
 @private
  UIWindow *window_;
  GameEngine *gameEngine_;
}

- (void)dealloc {
  [window_ release];  
  [gameEngine_ release];
  
  [super dealloc];
}

- (void)startGame {
  SplashState *rootState = [[[SplashState alloc] init] autorelease];
  [gameEngine_ pushState:rootState];
  [NSThread detachNewThreadSelector:@selector(initAudio:) toTarget:self withObject:rootState];
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
  
  gameEngine_ = [[GameEngine alloc] init];
  
  CGRect screenSize = [[UIScreen mainScreen] bounds];
  window_ = [[UIWindow alloc] initWithFrame:screenSize];
  [window_ addSubview:gameEngine_.view];
  [window_ makeKeyAndVisible];

  [self startGame];

  return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
  [gameEngine_ stopAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
  [gameEngine_ startAnimation];
  [gameEngine_ clearTouches];
}

- (void)applicationWillTerminate:(UIApplication *)application {
  [gameEngine_ stopAnimation];
}

@end
