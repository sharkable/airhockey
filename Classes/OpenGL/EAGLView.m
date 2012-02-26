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
  }

  return self;
}

- (void)dealloc {
  [renderer_ release];
  
  [super dealloc];
}

- (void)render {
  [renderer_ render];
}

#pragma mark - UIView

- (void)layoutSubviews {
  [renderer_ resizeFromLayer:(CAEAGLLayer *)self.layer];
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
