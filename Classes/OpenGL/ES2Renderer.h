//
//  ES2Renderer.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright Sharkable 2010. All rights reserved.
//

#import "ESRenderer.h"

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

@class GameEngine;

@interface ES2Renderer : NSObject<ESRenderer> {
 @private
  EAGLContext *context;

  // The pixel dimensions of the CAEAGLLayer
  GLint backingWidth;
  GLint backingHeight;

  // The OpenGL ES names for the framebuffer and renderbuffer used to render to this view
  GLuint defaultFramebuffer, colorRenderbuffer;

  GLuint program;
  
  GameEngine *gameEngine_;
}

@property(nonatomic, assign) GameEngine *gameEngine;

- (void)render;
- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer;

@end
