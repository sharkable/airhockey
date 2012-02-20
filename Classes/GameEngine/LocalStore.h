//
//  LocalStore.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-02.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface LocalStore : NSObject {

}

+ (BOOL) hasEntryForKey:(NSString*)key;
+ (BOOL) boolForKey:(NSString*)key;
+ (void) setBool:(BOOL)value forKey:(NSString*)key;
+ (int) integerForKey:(NSString*)key;
+ (void) setInteger:(int)value forKey:(NSString*)key;
+ (double) doubleForKey:(NSString*)key;
+ (void) setDouble:(double)value forKey:(NSString*)key;
+ (NSString*) stringForKey:(NSString*)key;
+ (void) setString:(NSString*)value forKey:(NSString*)key;

@end
