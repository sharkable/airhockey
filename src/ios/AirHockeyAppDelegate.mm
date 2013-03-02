//
//  AirHockeyAppDelegate.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-07.
//  Copyright Sharkable 2010. All rights reserved.
//

#import "ios/AirHockeyAppDelegate.h"

#import "gameengine/game_engine.h"
#import "ios/thirdparty/Flurry iPhone SDK v4.1.0/Flurry/Flurry.h"
#import "ios/ViewController.h"
#import "ios/EAGLView.h"
#import "ios/TypeUtil.h"
#import "soundengine/sound_player.h"

#import "airhockey/views/splash_view.h"

@interface AirHockeyAppDelegate ()
- (void)initAudio:(SoundInitializationDelegate *)delegate;
@end

@implementation AirHockeyAppDelegate {
 @private
  ViewController *viewController_;
}

- (void)dealloc {
  [viewController_ release];

  [super dealloc];
}

- (void)initAudio:(SoundInitializationDelegate *)delegate {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  
// TODO  SoundPlayer::instance()->syncAudioSessionForITunes();
  SoundPlayer::instance()->initializeWithDelegate(delegate);
  SoundPlayer::instance()->setSoundEffectsOn(true);
  
  [pool release];
}  

#pragma mark - UIApplicationDelegate

- (BOOL)application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
  NSString *appVersion = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"];
  [Flurry setAppVersion:appVersion];
  if (IS_FREE) {
    [Flurry startSession:@"BGGPH5B2THWFSJHXEKRH"];
  } else {
    [Flurry startSession:@"4HECR4PRJJP4ZSLZ2EJB"];
  }

  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
    viewController_ = [[ViewController alloc] initWithGameSize:game_size_make(768, 1152)];
    viewController_.gameEngine->SetScreenOffset(screen_point_make(0, 50));
  } else {
    viewController_ = [[ViewController alloc] initWithGameSize:game_size_make(768, 1024)];
  }

  NSString *mainMenuPositionsFilename =
      [[NSBundle mainBundle] pathForResource:@"main_menu" ofType:@"xml"];
  NSString *rinkPositionsFilename =
      [[NSBundle mainBundle] pathForResource:@"rink" ofType:@"xml"];
  viewController_.gameEngine->load_positions(TypeUtil::NSString2string(mainMenuPositionsFilename));
  viewController_.gameEngine->load_positions(TypeUtil::NSString2string(rinkPositionsFilename));
  sp<EngineView> rootView =
      sp<EngineView>(new SplashView(sp<GameEngine>(viewController_.gameEngine)));
  viewController_.gameEngine->PushView(rootView);

  return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
  [viewController_ stop];
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
  [viewController_ start];
  viewController_.gameEngine->ClearTouches();
}

- (void)applicationWillTerminate:(UIApplication *)application {
  [viewController_ stop];
}

- (UIWindow *)window {
  return viewController_.window;
}

@end
