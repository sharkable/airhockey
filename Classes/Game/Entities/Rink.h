//
//  Rink.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-13.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "StateEntity.h"
#import "RoundThing.h"
#import "Texture2D.h"

@interface Rink : NSObject <StateEntity> {
  Texture2D* _texture;
  CGPoint _renderPoint;
}

- (void) bounceOff:(RoundThing*)thing;
- (void) moveInFromEdge:(RoundThing*)thing;

@end
