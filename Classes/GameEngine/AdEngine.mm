//
//  AdEngine.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 2/25/12.
//  Copyright (c) 2012 Sharkable. All rights reserved.
//

#import "AdEngine.h"

#import "GADBannerView.h"
#import "game_engine.h"

@implementation AdEngine {
 @private
  GameEngine *gameEngine_;
  GADBannerView *ad_;
  long tickCount_;
  long lastAdRefresh_;
}

@synthesize gameEngine = gameEngine_;

- (void)addAdAtPoint:(ScreenPoint)point {
  CGRect adFrame = CGRectZero;
  adFrame.origin = CGPointMake(point.x, point.y);
  adFrame.size = GAD_SIZE_320x50;
  if (!ad_) {
    ad_ = [[GADBannerView alloc] initWithFrame:adFrame];
    ad_.adUnitID = @"a14bdda6dfc895a";
// TODO    ad_.rootViewController = gameEngine_;
    [ad_ loadRequest:nil];
//    self.gameEngine->addUIView(ad_);
    lastAdRefresh_ = tickCount_;
  } else {
    ad_.frame = adFrame;
//    self.gameEngine->addUIView(ad_);
    // TODO: what should happen here?
    //    if (tickCount - lastAdRefresh >= 60*60) {
    //      [ad loadRequest:nil];
    //      lastAdRefresh = tickCount;
    //    }
  }
}

- (void)removeAd {
  [ad_ removeFromSuperview];
}

@end
