//
//  ResourceLoader.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 09-09-28.
//  Copyright 2009 Sharkable. All rights reserved.
//

#import <map>
using namespace std;

#include "Texture2D.h"

class ResourceLoader {
 private:
  map<string, Texture2D> resources_;
  map<string, int> resourceCounter_;

 public:
  static ResourceLoader *instance();
  Texture2D getTextureWithName(string name);
  void releaseResourceWithName(string name);
  void releaseResource(Texture2D resource);
};
