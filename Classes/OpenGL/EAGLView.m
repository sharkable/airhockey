//
//  EAGLView.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright Sharkable 2010. All rights reserved.
//

#import "EAGLView.h"

#import "ES1Renderer.h"
#import "ES2Renderer.h"
#import "GameEngine.h"
#import "const.h"

@implementation EAGLView {    
 @private
  id<ESRenderer> renderer_;
  
  BOOL animating_;
  BOOL displayLinkSupported_;
  // Use of the CADisplayLink class is the preferred method for controlling your animation timing.
  // CADisplayLink will link to the main display and fire every vsync when added to a given
  // run-loop. The NSTimer class is used only as fallback when running on a pre 3.1 device where
  // CADisplayLink isn't available.
  id displayLink_;
  NSTimer *animationTimer_;
  
  GameEngine *gameEngine_;  // weak
}

// You must implement this method
+ (Class)layerClass {
  return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)frame {  
  self = [super initWithFrame:frame];
  if (self) {
    self.multipleTouchEnabled = YES;
    
    // Get the layer
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;

    eaglLayer.opaque = YES;
    eaglLayer.drawableProperties =
        [NSDictionary dictionaryWithObjectsAndKeys:
         [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking,
         kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
         nil];

    renderer_ = [[ES2Renderer alloc] init];

    if (!renderer_) {
      renderer_ = [[ES1Renderer alloc] init];

      if (!renderer_) {
        [self release];
        return nil;
      }
    }

    animating_ = NO;
    displayLinkSupported_ = NO;
    displayLink_ = nil;
    animationTimer_ = nil;

    // A system version of 3.1 or greater is required to use CADisplayLink. The NSTimer
    // class is used as fallback when it isn't available.
    NSString *reqSysVer = @"3.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending) {
      displayLinkSupported_ = YES;
    }
  }

  return self;
}

- (void)dealloc {
  [renderer_ release];
  [displayLink_ release];
  [animationTimer_ release];
  
  [super dealloc];
}

- (void)layoutSubviews {
  [renderer_ resizeFromLayer:(CAEAGLLayer *)self.layer];
}

- (void)startAnimation {
  if (!animating_) {
    if (displayLinkSupported_) {
      // CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will result
      // in a warning, but can be dismissed if the system version runtime check for CADisplayLink
      // exists in -initWithCoder:. The runtime check ensures this code will not be called in system
      // versions earlier than 3.1.

      displayLink_ =
          [[NSClassFromString(@"CADisplayLink") displayLinkWithTarget:renderer_
                                                             selector:@selector(render)] retain];
      [displayLink_ setFrameInterval:1];  // 60 fps
      [displayLink_ addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    } else {
      NSTimeInterval interval = (NSTimeInterval)(1.0 / 60.0);
      animationTimer_ = [[NSTimer scheduledTimerWithTimeInterval:interval
                                                          target:renderer_
                                                        selector:@selector(render)
                                                        userInfo:nil
                                                         repeats:YES] retain];
    }

    animating_ = YES;
  }
}

- (void)stopAnimation {
  if (animating_) {
    if (displayLinkSupported_) {
      [displayLink_ invalidate];
      [displayLink_ release];
      displayLink_ = nil;
    } else {
      [animationTimer_ invalidate];
      [animationTimer_ release];
      animationTimer_ = nil;
    }

    animating_ = NO;
  }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
  [gameEngine_ setTouchesBegan:touches];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  [gameEngine_ setTouchesMoved:touches];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
  [gameEngine_ setTouchesEnded:touches];
}

#pragma mark - Accessors

- (GameEngine *)gameEngine {
  return gameEngine_;
}

- (void)setGameEngine:(GameEngine *)gameEngine {
  gameEngine_ = gameEngine;
  renderer_.gameEngine = gameEngine_;
}

@end
