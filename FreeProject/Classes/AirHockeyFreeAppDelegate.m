//
//  AirHockeyFreeAppDelegate.m
//  AirHockeyFree
//
//  Created by Jonathan Sharkey on 10-05-07.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "AirHockeyFreeAppDelegate.h"
#import "EAGLView.h"
#import "SoundPlayer.h"
#import "SplashState.h"
#import "GameEngine.h"
#import "IsFree.h"
#import "FlurryAPI.h"

@implementation AirHockeyFreeAppDelegate

@synthesize window;
@synthesize glView;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	if (IS_FREE) {
		[FlurryAPI startSession:@"BGGPH5B2THWFSJHXEKRH"];
	} else {
		[FlurryAPI startSession:@"4HECR4PRJJP4ZSLZ2EJB"];
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
