//
//  ResourceLoader.m
//  AirHockey
//
//  Created by Jon Sharkey on 4/29/12.
//  Copyright (c) 2012 Sharkable. All rights reserved.
//

#include "gameengine/ResourceLoader.h"

#import <UIKit/UIKit.h>

#include "ios/TypeUtil.h"
#include "opengl/texture2D.h"

Texture2D ResourceLoader::Texture(string name) {
  UIImage *uiImage;
  NSUInteger        width,
  height,
  i;
  CGContextRef      context = nil;
  void*          data = nil;;
  CGColorSpaceRef      colorSpace;
  void*          tempData;
  unsigned int*      inPixel32;
  unsigned short*      outPixel16;
  BOOL          hasAlpha;
  CGImageAlphaInfo    info;
  CGAffineTransform    transform;
  SGSize          imageSize;
  Texture2DPixelFormat    pixelFormat;
  CGImageRef        image;
  BOOL          sizeToFit = NO;
  
  

  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
    uiImage = [UIImage imageNamed:[NSString stringWithFormat:@"%@.png", TypeUtil::string2NSString(name)]];
  } else {
    uiImage = [UIImage imageNamed:[NSString stringWithFormat:@"%@_iphone.png", TypeUtil::string2NSString(name)]];
  }
  
  image = [uiImage CGImage];
  
  if(image == NULL) {
    //[self release];
    image = [[UIImage imageNamed:@"yellowright.png"] CGImage];
    NSLog(@"Image is Null");
    //return nil;
  }
  
  
  info = CGImageGetAlphaInfo(image);
  hasAlpha = ((info == kCGImageAlphaPremultipliedLast) || (info == kCGImageAlphaPremultipliedFirst) || (info == kCGImageAlphaLast) || (info == kCGImageAlphaFirst) ? YES : NO);
  if(CGImageGetColorSpace(image)) {
    //if(hasAlpha)
    pixelFormat = kTexture2DPixelFormat_RGBA8888;
    //else
    //  pixelFormat = kTexture2DPixelFormat_RGB565;
  } else  //NOTE: No colorspace means a mask image
    pixelFormat = kTexture2DPixelFormat_A8;
  
  
  imageSize = SGSizeMake(CGImageGetWidth(image), CGImageGetHeight(image));
  transform = CGAffineTransformIdentity;
  
  width = imageSize.width;
  
  if((width != 1) && (width & (width - 1))) {
    i = 1;
    while((sizeToFit ? 2 * i : i) < width)
      i *= 2;
    width = i;
  }
  height = imageSize.height;
  if((height != 1) && (height & (height - 1))) {
    i = 1;
    while((sizeToFit ? 2 * i : i) < height)
      i *= 2;
    height = i;
  }
  while((width > kMaxTextureSize) || (height > kMaxTextureSize)) {
    width /= 2;
    height /= 2;
    transform = CGAffineTransformScale(transform, 0.5, 0.5);
    imageSize.width *= 0.5;
    imageSize.height *= 0.5;
  }
  
  switch(pixelFormat) {    
    case kTexture2DPixelFormat_RGBA8888:
      colorSpace = CGColorSpaceCreateDeviceRGB();
      data = malloc(height * width * 4);
      context = CGBitmapContextCreate(data, width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
      CGColorSpaceRelease(colorSpace);
      break;
    case kTexture2DPixelFormat_RGB565:
      colorSpace = CGColorSpaceCreateDeviceRGB();
      data = malloc(height * width * 4);
      context = CGBitmapContextCreate(data, width, height, 8, 4 * width, colorSpace, kCGImageAlphaNoneSkipLast | kCGBitmapByteOrder32Big);
      CGColorSpaceRelease(colorSpace);
      break;
      
    case kTexture2DPixelFormat_A8:
      data = malloc(height * width);
      context = CGBitmapContextCreate(data, width, height, 8, width, NULL, kCGImageAlphaOnly);
      break;        
    default:
      [NSException raise:NSInternalInconsistencyException format:@"Invalid pixel format"];
  }
  
  
  CGContextClearRect(context, CGRectMake(0, 0, width, height));
  CGContextTranslateCTM(context, 0, height - imageSize.height);
  
  if(!CGAffineTransformIsIdentity(transform))
    CGContextConcatCTM(context, transform);
  CGContextDrawImage(context, CGRectMake(0, 0, CGImageGetWidth(image), CGImageGetHeight(image)), image);
  //Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGGBBBBB"
  if(pixelFormat == kTexture2DPixelFormat_RGB565) {
    tempData = malloc(height * width * 2);
    inPixel32 = (unsigned int*)data;
    outPixel16 = (unsigned short*)tempData;
    for(i = 0; i < width * height; ++i, ++inPixel32)
      *outPixel16++ = ((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | ((((*inPixel32 >> 8) & 0xFF) >> 2) << 5) | ((((*inPixel32 >> 16) & 0xFF) >> 3) << 0);
    free(data);
    data = tempData;
    
  }
  Texture2D texture(data, pixelFormat, width, height, imageSize);
  
  CGContextRelease(context);
  free(data);
  return texture;
}
