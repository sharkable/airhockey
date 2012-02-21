//
//  AirHockeyAppDelegate.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright Sharkable 2010. All rights reserved.
//

#import "AirHockeyAppDelegate.h"
#import "EAGLView.h"
#import "SplashState.h"
#import "SoundPlayer.h"
#import "GameEngine.h"
#import "IsFree.h"
//#import "FlurryAPI.h"

@implementation AirHockeyAppDelegate

@synthesize window;
@synthesize glView;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
  if (IS_FREE) {
//    [FlurryAPI startSession:@"BGGPH5B2THWFSJHXEKRH"];
  } else {
//    [FlurryAPI startSession:@"4HECR4PRJJP4ZSLZ2EJB"];
  }
  SplashState* rootState = [[[SplashState alloc] init] autorelease];
    [glView startAnimation];
  [[GameEngine instance] pushState:rootState];
  [SoundPlayer syncAudioSessionForITunes];
  [SoundPlayer initializeWithDelegate:rootState];
  [SoundPlayer setSoundEffectsOn:YES];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    [glView stopAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    [glView startAnimation];
  [[GameEngine instance] clearTouches];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [glView stopAnimation];
}

- (void)dealloc
{
    [window release];
    [glView release];

    [super dealloc];
}

@end
