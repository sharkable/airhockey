//
//  SimpleItem.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "SimpleItem.h"
#import "ResourceLoader.h"

@implementation SimpleItem

@synthesize texture = texture_;
@synthesize position = position_;
@synthesize angle = angle_;

- (id)initWithTexture:(Texture2D *)texture position:(CGPoint)position {
  [super init];
  
  if (self) {
    textures_ = [[NSMutableArray alloc] init];
    [textures_ addObject:texture];
    texture_ = 0;
    angle_ = 0;
    
    position_ = position;
  }
  
  return self;
}

- (id)initWithTextures:(NSArray *)textures position:(CGPoint)position {
  [super init];
  
  if (self) {
    textures_ = [[NSMutableArray alloc] initWithArray:textures];
    texture_ = 0;
    angle_ = 0;
    
    position_ = position;
  }
  
  return self;
}

- (void)dealloc {
  for (Texture2D *texture in textures_) {
    [[ResourceLoader instance] releaseResource:texture];
  }
  [textures_ release];
  
  [super dealloc];
}

- (void)update {
}

- (void)render {
  Texture2D *t = [textures_ objectAtIndex:texture_];
  [t drawAtPoint:CGPointMake(position_.x, position_.y)
           angle:angle_];
}

- (CGSize)size {
  Texture2D *t = [textures_ objectAtIndex:texture_];
  return t.contentSize;
}

@end
