//
//  ResourceLoader.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 09-09-28.
//  Copyright 2009 Sharkable. All rights reserved.
//

#import "ResourceLoader.h"
#import "const.h"

@implementation ResourceLoader

static ResourceLoader *instance_ = nil;

+ (ResourceLoader *)instance {
  if (instance_ == nil) {
    instance_ = [[ResourceLoader alloc] init];
  }
  return instance_;
}

- (ResourceLoader *)init {
  self = [super init];
  if (self) {
    resources_ = [[NSMutableDictionary alloc] init];
    resourceCounter_ = [[NSMutableDictionary alloc] init];
  }
  return self;
}

- (void)dealloc {
  [resources_ release];
  [resourceCounter_ release];
  
  [super dealloc];
}

- (Texture2D *)getTextureWithName:(NSString *)name {
  NSNumber *count = [resourceCounter_ objectForKey:name];
  if ([count intValue] == 0) {
    UIImage *image;
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
      image = [UIImage imageNamed:[NSString stringWithFormat:@"%@.png", name]];
    } else {
      image = [UIImage imageNamed:[NSString stringWithFormat:@"%@_iphone.png", name]];
    }
    
    Texture2D *texture = [[[Texture2D alloc] initWithImage:image] autorelease];
    [resourceCounter_ setObject:[NSNumber numberWithInt:[count intValue] + 1] forKey:name];
    [resources_ setObject:texture forKey:name];
    return texture;
  }
  
  Texture2D *texture = [resources_ objectForKey:name];
  [resourceCounter_ setObject:[NSNumber numberWithInt:[count intValue]+1] forKey:name];
  
  return texture;  
}

- (void)releaseResourceWithName:(NSString *)name {
  if (!name) {
    return;
  }
  NSNumber *count = [resourceCounter_ objectForKey:name];
  if (count == nil || [count intValue] == 0) {
    return;
  } else if ([count intValue] == 1) {
    [resources_ removeObjectForKey:name];
    [resourceCounter_ removeObjectForKey:name];
  } else {
    [resourceCounter_ setObject:[NSNumber numberWithInt:[count intValue] - 1] forKey:name];
  }
}

- (void)releaseResource:(id)resource {
  if (!resource) {
    return;
  }
  NSArray *keys = [resources_ allKeysForObject:resource];
  if (keys.count == 1) {
    [self releaseResourceWithName:[keys objectAtIndex:0]];    
  } else {
    // TODO assert
  }
}

@end
