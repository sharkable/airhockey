//
//  ResourceLoader.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 09-09-28.
//  Copyright 2009 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Texture2D.h"

@interface ResourceLoader : NSObject {
	NSMutableDictionary* _resources;
	NSMutableDictionary* _resourceCounter;
}

+ (ResourceLoader*) instance;
- (Texture2D*) getTextureWithName:(NSString*) name;
- (void) releaseResourceWithName:(NSString*)name;
- (void) releaseResource:(id)resource;

@end
