//
//  SplashState.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-07.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "EngineState.h"
#import "SimpleItem.h"
#import "SoundInitializationDelegate.h"

@interface SplashState : EngineState <SoundInitializationDelegate> {
  UIActivityIndicatorView* _spinner;
}

@end
