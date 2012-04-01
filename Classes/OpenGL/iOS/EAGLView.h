//
//  EAGLView.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright Sharkable 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

// This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
// The view content is basically an EAGL surface you render your OpenGL scene into.
// Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
@interface EAGLView : UIView

// Render a frame. The target is called with the selector to render all of the objects within the
// scene.
// Before |selector| is called, the scene is cleared and the coordinate system is set up. After it
// is called, the buffer is presented.
- (void)renderWithTarget:(id)target renderSelector:(SEL)selector;

@end
