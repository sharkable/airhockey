//
//  ResourceLoader.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 09-09-28.
//  Copyright 2009 Sharkable. All rights reserved.
//

#import "ResourceLoader.h"
#import "const.h"

#import "TypeUtil.h"

ResourceLoader instance__;

ResourceLoader *ResourceLoader::instance() {
  return &instance__;          
}

Texture2D ResourceLoader::getTextureWithName(string name) {
  int count = resourceCounter_[name];
  if (count == 0) {
    UIImage *image;
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
      image = [UIImage imageNamed:[NSString stringWithFormat:@"%@.png", TypeUtil::string2NSString(name)]];
    } else {
      image = [UIImage imageNamed:[NSString stringWithFormat:@"%@_iphone.png", TypeUtil::string2NSString(name)]];
    }
    
    Texture2D texture(image);
    resourceCounter_[name] = count + 1;
    resources_[name] = texture;
    return texture;
  }
  
  Texture2D texture = resources_[name];
  resourceCounter_[name] = count + 1;
  
  return texture;  
}

void ResourceLoader::releaseResourceWithName(string name) {
  int count = resourceCounter_[name];
  if (count == 0) {
    return;
  } else if (count == 1) {
    resources_.erase(name);
    resourceCounter_.erase(name);
  } else {
    resourceCounter_[name] = count - 1;
  }
}

void ResourceLoader::releaseResource(Texture2D resource) {
  for (map<string, Texture2D>::iterator i = resources_.begin(); i != resources_.end(); i++) {
    if (i->second.name() == resource.name()) {
      releaseResourceWithName(i->first);
    }
  }
}
