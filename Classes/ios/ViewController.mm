//
//  ViewController.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 4/1/12.
//  Copyright (c) 2012 Sharkable. All rights reserved.
//

#import "ViewController.h"

#import "EAGLView.h"
#import "game_engine.h"
#import "GameTimer.h"
#import "GameTouchWindow.h"
#import "splash_view.h"

@implementation ViewController {
 @private
  EAGLView *view_;
  GameTimer *gameTimer_;
  GameTouchWindow *gameTouchWindow_;

  GameEngine *game_engine_;
  sp<EngineView> root_view_;
};

@synthesize window = gameTouchWindow_, game_engine = game_engine_;

- (id)init {
  self = [super init];
  if (self) {
    CGRect screenSize = [[UIScreen mainScreen] bounds];
    gameTouchWindow_ = [[GameTouchWindow alloc] initWithFrame:screenSize];
    [gameTouchWindow_ addSubview:self.view];
    [gameTouchWindow_ makeKeyAndVisible];

    game_engine_ = new GameEngine();
    root_view_.reset(new SplashView(*game_engine_));
    game_engine_->PushView(root_view_);
    gameTimer_ = [[GameTimer alloc] initWithTarget:self selector:@selector(update)];
    gameTouchWindow_.gameEngine = game_engine_;    
}
  return self;
}

- (void)dealloc {
  delete game_engine_;

  [view_ release];
  [gameTimer_ release];
  [gameTouchWindow_ release];

  [super dealloc];
}

- (void)start {
  [gameTimer_ start];
}

- (void)stop {  
  [gameTimer_ stop];
}

#pragma mark - UIViewController

- (void)loadView {
  // TODO: Consider handling viewDidUnload: to free memory.
  if (!view_) {
    view_ = [[EAGLView alloc] initWithFrame:gameTouchWindow_.frame];
  }
  self.view = view_;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
  return (interfaceOrientation == UIInterfaceOrientationPortrait ||
          interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown);
}

#pragma mark - Private

- (void)update {
  game_engine_->Update();
  [view_ setUpRender];
  game_engine_->Render();
  [view_ finishRender];
}

@end
