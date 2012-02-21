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

@interface SimpleItem : NSObject<StateEntity> {
 @private
  NSMutableArray *textures_;
  int texture_;
  CGPoint position_;
  double angle_;
}

- (id)initWithTexture:(Texture2D *)texture position:(CGPoint)position;
- (id)initWithTextures:(NSArray*)textures position:(CGPoint)position;

@property(nonatomic, assign) int texture;
@property(nonatomic, assign) CGPoint position;
@property(nonatomic, assign) double angle;
@property(nonatomic, readonly) CGSize size;

@end

