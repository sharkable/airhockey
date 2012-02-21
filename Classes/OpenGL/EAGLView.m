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
#import "IsFree.h"
#import "const.h"

// TODO I don't like this. I'm using this for the static AdMob functions.
EAGLView* __instance;

@implementation EAGLView

@synthesize animating;
@synthesize viewController = viewController_;

@dynamic animationFrameInterval;

// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)frame
{  
    if ((self = [super initWithFrame:frame]))
    {
    __instance = self;
    
    self.multipleTouchEnabled = YES;
    
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;

        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];

        renderer = nil; //[[ES2Renderer alloc] init];

        if (!renderer)
        {
            renderer = [[ES1Renderer alloc] init];

            if (!renderer)
            {
                [self release];
                return nil;
            }
        }
    
        animating = FALSE;
        displayLinkSupported = FALSE;
        animationFrameInterval = 1;
        displayLink = nil;
        animationTimer = nil;

        // A system version of 3.1 or greater is required to use CADisplayLink. The NSTimer
        // class is used as fallback when it isn't available.
        NSString *reqSysVer = @"3.1";
        NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
        if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
            displayLinkSupported = TRUE;
    }

    return self;
}

- (void)drawView:(id)sender
{
  tickCount++;
    [renderer render];
}

- (void)layoutSubviews
{
    [renderer resizeFromLayer:(CAEAGLLayer*)self.layer];
    [self drawView:nil];
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    // Frame interval defines how many display frames must pass between each time the
    // display link fires. The display link will only fire 30 times a second when the
    // frame internal is two on a display that refreshes 60 times a second. The default
    // frame interval setting of one will fire 60 times a second when the display refreshes
    // at 60 times a second. A frame interval setting of less than one results in undefined
    // behavior.
    if (frameInterval >= 1)
    {
        animationFrameInterval = frameInterval;

        if (animating)
        {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void)startAnimation
{
    if (!animating)
    {
        if (displayLinkSupported)
        {
            // CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will result in a warning, but can be dismissed
            // if the system version runtime check for CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
            // not be called in system versions earlier than 3.1.

            displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawView:)];
            [displayLink setFrameInterval:animationFrameInterval];
            [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        }
        else
            animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0 / 60.0) * animationFrameInterval) target:self selector:@selector(drawView:) userInfo:nil repeats:TRUE];

        animating = TRUE;
    }
}

- (void)stopAnimation
{
    if (animating)
    {
        if (displayLinkSupported)
        {
            [displayLink invalidate];
            displayLink = nil;
        }
        else
        {
            [animationTimer invalidate];
            animationTimer = nil;
        }

        animating = FALSE;
    }
}

- (void)dealloc
{
    [renderer release];
  
    [super dealloc];
}

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event {
  [[GameEngine instance] setTouchesBegan:touches];
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event {
  [[GameEngine instance] setTouchesMoved:touches];
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event {
  [[GameEngine instance] setTouchesEnded:touches];
}

- (void)__addAdAtPoint:(CGPoint)point {
  CGRect adFrame = CGRectZero;
  adFrame.origin = CGPointMake(point.x, point.y);
  adFrame.size = GAD_SIZE_320x50;
  if (!ad) {
    ad = [[GADBannerView alloc] initWithFrame:adFrame];
    ad.adUnitID = @"a14bdda6dfc895a";
    ad.rootViewController = viewController_;
    [ad loadRequest:nil];
    [self addSubview:ad];
    lastAdRefresh = tickCount;
  } else {
    ad.frame = adFrame;
    if (![[self subviews] containsObject:ad]) {
      [self addSubview:ad];
    }
    // TODO: what should happen here?
//    if (tickCount - lastAdRefresh >= 60*60) {
//      [ad loadRequest:nil];
//      lastAdRefresh = tickCount;
//    }
  }
}

+ (void)addAdAtPoint:(CGPoint)point {
  if (IS_FREE) {
    [__instance __addAdAtPoint:point];
  }
}

- (void)__removeAd {
  [ad removeFromSuperview];
}

+ (void)removeAd {
  if (IS_FREE && UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
    [__instance __removeAd];
  }
}

+ (void)addUIView:(UIView*)view {
  [__instance addSubview:view];
}

+ (void)removeUIView:(UIView*)view {
  [view removeFromSuperview];
}

@end
