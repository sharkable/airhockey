//
//  AirHockeyAppDelegate.m
//  AirHockey
//
//  Created by Jon Sharkey on 2010-05-07.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "airhockey/ios/AirHockeyAppDelegate.h"

#import "gameengine/ios/thirdparty/Flurry iPhone SDK v4.1.0/Flurry/Flurry.h"
#import "gameengine/ios/GameEngineFactoryIOS.h"
#import "gameengine/ios/TypeUtil.h"
#import "gameengine/ios/ViewController.h"
#import "gameengine/game_engine.h"
#import "soundengine/sound_player.h"

#import "airhockey/views/rink_view.h"
#import "airhockey/views/splash_view.h"

@interface AirHockeyAppDelegate ()
- (void)loadPositions:(NSString *)filename;
@end

@implementation AirHockeyAppDelegate {
 @private
  ViewController *viewController_;
}

- (void)dealloc {
  [viewController_ release];

  [super dealloc];
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
    // Make room for the banner ad.
    ScreenPoint offset = screen_point_make(0, 53 * [UIScreen mainScreen].scale);
    viewController_.gameEngine->SetScreenOffset(offset);
  }

  viewController_.gameEngine->set_factory(sp<GameEngineFactory>(new GameEngineFactoryIOS()));

  NSString *mainMenuPositionsFilename = nil;
  NSString *rinkPositionsFilename = nil;
  NSString *playPositionsFilename = nil;
  NSString *gameMenuPositionsFilename = nil;

  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
    mainMenuPositionsFilename = @"main_menu_iphone.xml";
    rinkPositionsFilename = @"rink_iphone.xml";
    playPositionsFilename = @"play_iphone.xml";
    gameMenuPositionsFilename = @"game_menu_iphone.xml";
  } else {
    mainMenuPositionsFilename = @"main_menu.xml";
    rinkPositionsFilename = @"rink.xml";
    playPositionsFilename = @"play.xml";
    gameMenuPositionsFilename = @"game_menu.xml";
  }
  [self loadPositions:mainMenuPositionsFilename];
  [self loadPositions:rinkPositionsFilename];
  [self loadPositions:playPositionsFilename];
  [self loadPositions:gameMenuPositionsFilename];

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


// private

- (void)loadPositions:(NSString *)filename {
  std::string filenameString = TypeUtil::NSString2string(filename);
  sp<AssetReader> assetReader =
      viewController_.gameEngine->factory()->createAssetReader(filenameString);
  viewController_.gameEngine->load_positions(*assetReader);
}

@end
