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

static ResourceLoader* _instance = nil;

+ (ResourceLoader*) instance {
	if (_instance == nil) {
		_instance = [[ResourceLoader alloc] init];
	}
	return _instance;
}

- (ResourceLoader*) init {
	_resources = [[NSMutableDictionary alloc] init];
	_resourceCounter = [[NSMutableDictionary alloc] init];
	return self;
}

- (void) dealloc {
	[_resources release];
	[_resourceCounter release];
	[super dealloc];
}

- (Texture2D*) getTextureWithName:(NSString*)name {
	NSNumber* count = [_resourceCounter objectForKey:name];
	if ([count intValue] == 0) {
		UIImage* image;
		if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
			image = [UIImage imageNamed:[NSString stringWithFormat:@"%@.png", name]];
		} else {
			image = [UIImage imageNamed:[NSString stringWithFormat:@"%@_iphone.png", name]];
		}
		
		Texture2D* texture = [[[Texture2D alloc] initWithImage:image] autorelease];
		[_resourceCounter setObject:[NSNumber numberWithInt:[count intValue] + 1] forKey:name];
		[_resources setObject:texture forKey:name];
		//NSLog(@"GOT: %@ fors the first time!", name);
		return texture;
	}
	
	Texture2D* texture = [_resources objectForKey:name];
	[_resourceCounter setObject:[NSNumber numberWithInt:[count intValue]+1] forKey:name];
	//NSLog(@"GOT: %@ again", name);
	
	return texture;	
}

- (void) releaseResourceWithName:(NSString*)name {
	if (!name) {
		return;
	}
	NSNumber* count = [_resourceCounter objectForKey:name];
	if (count == nil || [count intValue] == 0) {
		//NSLog(@"ERROR: %@ RELEASED BUT NOT THERE", name);
		return;
	} else if ([count intValue] == 1) {
		[_resources removeObjectForKey:name];
		[_resourceCounter removeObjectForKey:name];
		//NSLog(@"REMOVED: %@", name);
	} else {
		[_resourceCounter setObject:[NSNumber numberWithInt:[count intValue]-1] forKey:name];
	}
}

- (void) releaseResource:(id)resource {
	if (!resource) {
		return;
	}
	NSArray* keys = [_resources allKeysForObject:resource];
	if (keys.count == 1) {
		[self releaseResourceWithName:[keys objectAtIndex:0]];		
	} else {
		//NSLog(@"Resource has unexpected number of keys!");
	}
}

@end
