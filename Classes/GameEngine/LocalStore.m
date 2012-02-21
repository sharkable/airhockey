//
//  LocalStore.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-02.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "LocalStore.h"


@implementation LocalStore

+ (BOOL) hasEntryForKey:(NSString*)key {
  NSUserDefaults* userDefaults = [NSUserDefaults standardUserDefaults];
  return [[[userDefaults dictionaryRepresentation] allKeys] containsObject:key];
}

+ (BOOL) boolForKey:(NSString*)key
{
  NSUserDefaults* userDefaults = [NSUserDefaults standardUserDefaults];
  return [userDefaults boolForKey:key];
}

+ (void) setBool:(BOOL)value forKey:(NSString*)key
{
  NSUserDefaults* userDefaults = [NSUserDefaults standardUserDefaults];
  [userDefaults setBool:value forKey:key];
  [userDefaults synchronize];
}

+ (int) integerForKey:(NSString*)key
{
  NSUserDefaults* userDefaults = [NSUserDefaults standardUserDefaults];
  return [userDefaults integerForKey:key];
}

+ (void) setInteger:(int)value forKey:(NSString*)key
{
  NSUserDefaults* userDefaults = [NSUserDefaults standardUserDefaults];
  [userDefaults setInteger:value forKey:key];
  [userDefaults synchronize];
}

+ (double) doubleForKey:(NSString*)key
{
  NSUserDefaults* userDefaults = [NSUserDefaults standardUserDefaults];
  return [userDefaults doubleForKey:key];
}

+ (void) setDouble:(double)value forKey:(NSString*)key;
{
  NSUserDefaults* userDefaults = [NSUserDefaults standardUserDefaults];
  [userDefaults setDouble:value forKey:key];
  [userDefaults synchronize];
}  

+ (NSString*) stringForKey:(NSString*)key
{
  NSUserDefaults* userDefaults = [NSUserDefaults standardUserDefaults];
  return [userDefaults stringForKey:key];
}

+ (void) setString:(NSString*)value forKey:(NSString*)key
{
  NSUserDefaults* userDefaults = [NSUserDefaults standardUserDefaults];
  [userDefaults setObject:value forKey:key];
  [userDefaults synchronize];
}

@end
