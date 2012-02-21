//
//  SimpleItem.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "StateEntity.h"
#import "Texture2D.h"

@interface SimpleItem : NSObject <StateEntity> {
  NSMutableArray* textures_;
  int             texture_;
  CGPoint         position_;
  double          angle_;
}

- (id) initWithTexture:(Texture2D*)texture position:(CGPoint)position;
- (id) initWithTextures:(NSArray*)textures position:(CGPoint)position;

@property (assign) int      texture;
@property (assign) CGPoint  position;
@property (assign) double   angle;
@property (readonly) CGSize size;

@end

