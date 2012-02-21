//
//  MultiSelect.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "StateEntity.h"
#import "Texture2D.h"

@interface MultiSelect : NSObject <StateEntity> {
  NSMutableArray* _normalTextures;
  NSMutableArray* _selectedTextures;
  NSMutableArray* _positionsX;
  NSMutableArray* _positionsY;
  int _selectedValue;
}

- (void) addValueWithNormalTexture:(Texture2D*)normalTexture selectedTexture:(Texture2D*)selectedTexture position:(CGPoint)position;

@property (assign) int selectedValue;

@end
