//
//  LocalStore.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-02.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AirHockey_LocalStore_h
#define AirHockey_LocalStore_h

#import <string>
using namespace std;

class LocalStore {
 public:
  static bool hasEntryForKey(string key);
  static bool boolForKey(string key);
  static void setBool(bool value, string key);
  static int integerForKey(string key);
  static void setInteger(int value, string key);
  static double doubleForKey(string key);
  static void setDouble(double value, string key);
  static string stringForKey(string key);
  static void setString(string value, string key);
};

//@interface LocalStore : NSObject
//
//+ (BOOL)hasEntryForKey:(NSString *)key;
//+ (BOOL)boolForKey:(NSString *)key;
//+ (void)setBool:(BOOL)value forKey:(NSString*)key;
//+ (int)integerForKey:(NSString *)key;
//+ (void)setInteger:(int)value forKey:(NSString *)key;
//+ (double)doubleForKey:(NSString *)key;
//+ (void)setDouble:(double)value forKey:(NSString *)key;
//+ (NSString *)stringForKey:(NSString *)key;
//+ (void)setString:(NSString *)value forKey:(NSString *)key;
//
//@end

#endif