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
  NSMutableArray* normalTextures_;
  NSMutableArray* selectedTextures_;
  NSMutableArray* positionsX_;
  NSMutableArray* positionsY_;
  int selectedValue_;
}

- (void) addValueWithNormalTexture:(Texture2D*)normalTexture selectedTexture:(Texture2D*)selectedTexture position:(CGPoint)position;

@property (assign) int selectedValue;

@end
