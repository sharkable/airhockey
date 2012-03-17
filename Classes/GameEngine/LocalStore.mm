//
//  LocalStore.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-02.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "LocalStore.h"

#include "TypeUtil.h"

bool LocalStore::hasEntryForKey(string key) {
  NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
  return [[[userDefaults dictionaryRepresentation] allKeys]
          containsObject:TypeUtil::string2NSString(key)];
}

static bool boolForKey(string key) {
  NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
  return [userDefaults boolForKey:TypeUtil::string2NSString(key)];
}

void LocalStore::setBool(bool value, string key) {
  NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
  [userDefaults setBool:value forKey:TypeUtil::string2NSString(key)];
  [userDefaults synchronize];
}

int LocalStore::integerForKey(string key) {
  NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
  return [userDefaults integerForKey:TypeUtil::string2NSString(key)];
}

void LocalStore::setInteger(int value, string key) {
  NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
  [userDefaults setInteger:value forKey:TypeUtil::string2NSString(key)];
  [userDefaults synchronize];
}

double LocalStore::doubleForKey(string key) {
  NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
  return [userDefaults doubleForKey:TypeUtil::string2NSString(key)];
}

void LocalStore::setDouble(double value, string key) {
  NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
  [userDefaults setDouble:value forKey:TypeUtil::string2NSString(key)];
  [userDefaults synchronize];
}

string LocalStore::stringForKey(string key) {
  NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
  NSString *value = [userDefaults stringForKey:TypeUtil::string2NSString(key)];
  return TypeUtil::NSString2string(value);
}

void LocalStore::setString(string value, string key) {
  NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
  NSString *valueObjc =
      [userDefaults stringForKey:TypeUtil::string2NSString(value)];
  [userDefaults setObject:valueObjc forKey:TypeUtil::string2NSString(key)];
  [userDefaults synchronize];
}
