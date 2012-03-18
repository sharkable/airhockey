//
//  SimpleItem.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "SimpleItem.h"
#import "ResourceLoader.h"

SimpleItem::SimpleItem(Texture2D *texture, CGPoint position) {
  textures_ = [[NSMutableArray alloc] init];
  [textures_ addObject:texture];
  texture_ = 0;
  angle_ = 0;
  
  position_ = position;
}

SimpleItem::SimpleItem(NSArray *textures, CGPoint position) {
  textures_ = [[NSMutableArray alloc] initWithArray:textures];
  texture_ = 0;
  angle_ = 0;
  
  position_ = position;
}

SimpleItem::~SimpleItem() {
  for (Texture2D *texture in textures_) {
    [[ResourceLoader instance] releaseResource:texture];
  }
  [textures_ release];
}

void SimpleItem::update() {
}

void SimpleItem::render() {
  Texture2D *t = [textures_ objectAtIndex:texture_];
  [t drawAtPoint:CGPointMake(position_.x, position_.y)
           angle:angle_];
}

CGSize SimpleItem::getSize() {
  Texture2D *t = [textures_ objectAtIndex:texture_];
  return t.contentSize;
}
