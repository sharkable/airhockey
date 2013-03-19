//
//  AirHockeyAppDelegate.m
//  AirHockey
//
//  Created by Jon Sharkey on 2010-05-07.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "airhockey/ios/AirHockeyAppDelegate.h"

#import "gameengine/ios/thirdparty/Flurry iPhone SDK v4.1.0/Flurry/Flurry.h"
#import "gameengine/ios/TypeUtil.h"
#import "gameengine/ios/ViewController.h"
#import "gameengine/game_engine.h"
#import "soundengine/sound_player.h"

#import "airhockey/views/rink_view.h"
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

  SoundPlayer::instance()->initializeWithDelegate(delegate);
  SoundPlayer::instance()->setSoundEffectsOn(true);

  [pool release];
}

#pragma mark - UIApplicationDelegate

- (BOOL)application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
  NSString *appVersion = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"];
  [Flurry setAppVersion:appVersion];
  [Flurry startSession:@"BGGPH5B2THWFSJHXEKRH"];

  viewController_ = [[ViewController alloc] init];

  double screenWidth = [UIScreen mainScreen].bounds.size.width * [UIScreen mainScreen].scale;
  double screenHeight = screenWidth * 1024.0 / 768.0;
  viewController_.gameEngine->SetScreenSize(screen_size_make(screenWidth, screenHeight),
                                            game_size_make(768.0, 1024.0));

  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
    // TODO this is kinda hacky. I want it to be right on point 53 of an iPhone.
    viewController_.gameEngine->SetGameOffset(game_point_make(0, 53 * 768.0 / 320.0));
  }

  NSString *mainMenuPositionsFilename = nil;
  NSString *rinkPositionsFilename = nil;
  NSString *playPositionsFilename = nil;
  NSString *gameMenuPositionsFilename = nil;

  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
    mainMenuPositionsFilename =
        [[NSBundle mainBundle] pathForResource:@"main_menu_iphone" ofType:@"xml"];
    rinkPositionsFilename = [[NSBundle mainBundle] pathForResource:@"rink_iphone" ofType:@"xml"];
    playPositionsFilename = [[NSBundle mainBundle] pathForResource:@"play_iphone" ofType:@"xml"];
    gameMenuPositionsFilename = [[NSBundle mainBundle] pathForResource:@"game_menu" ofType:@"xml"];
  } else {
    mainMenuPositionsFilename = [[NSBundle mainBundle] pathForResource:@"main_menu" ofType:@"xml"];
    rinkPositionsFilename = [[NSBundle mainBundle] pathForResource:@"rink" ofType:@"xml"];
    playPositionsFilename = [[NSBundle mainBundle] pathForResource:@"play" ofType:@"xml"];
    gameMenuPositionsFilename = [[NSBundle mainBundle] pathForResource:@"game_menu" ofType:@"xml"];
  }
  viewController_.gameEngine->load_positions(TypeUtil::NSString2string(mainMenuPositionsFilename));
  viewController_.gameEngine->load_positions(TypeUtil::NSString2string(rinkPositionsFilename));
  viewController_.gameEngine->load_positions(TypeUtil::NSString2string(playPositionsFilename));
  viewController_.gameEngine->load_positions(TypeUtil::NSString2string(gameMenuPositionsFilename));

  sp<EngineView> rootView =
      sp<EngineView>(new RinkView(sp<GameEngine>(viewController_.gameEngine)));
  viewController_.gameEngine->SetRootView(rootView);
  sp<EngineView> splashView =
      sp<EngineView>(new SplashView(sp<GameEngine>(viewController_.gameEngine)));
  viewController_.gameEngine->PushView(splashView);

  return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
  [viewController_ stop];
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
  SoundPlayer::instance()->syncAudioSessionForITunes();
  SoundPlayer::instance()->duckAudioFromITunes(true);

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
