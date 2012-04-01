/*
 
 ===== IMPORTANT =====
 
 This is sample code demonstrating API, technology or techniques in development.
 Although this sample code has been reviewed for technical accuracy, it is not
 final. Apple is supplying this information to help you plan for the adoption of
 the technologies and programming interfaces described herein. This information
 is subject to change, and software implemented based on this sample code should
 be tested with final operating system software and final documentation. Newer
 versions of this sample code may be provid
 ed with future seeds of the API or
 technology. For information about updates to this and other developer
 documentation, view the New & Updated sidebars in subsequent documentation
 seeds.
 
 =====================
 
 File: Texture2D.m
 Abstract: Creates OpenGL 2D textures from images or text.
 
 Version: 1.6
 
 Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
 ("Apple") in consideration of your agreement to the following terms, and your
 use, installation, modification or redistribution of this Apple software
 constitutes acceptance of these terms.  If you do not agree with these terms,
 please do not use, install, modify or redistribute this Apple software.
 
 In consideration of your agreement to abide by the following terms, and subject
 to these terms, Apple grants you a personal, non-exclusive license, under
 Apple's copyrights in this original Apple software (the "Apple Software"), to
 use, reproduce, modify and redistribute the Apple Software, with or without
 modifications, in source and/or binary forms; provided that if you redistribute
 the Apple Software in its entirety and without modifications, you must retain
 this notice and the following text and disclaimers in all such redistributions
 of the Apple Software.
 Neither the name, trademarks, service marks or logos of Apple Inc. may be used
 to endorse or promote products derived from the Apple Software without specific
 prior written permission from Apple.  Except as expressly stated in this notice,
 no other rights or licenses, express or implied, are granted by Apple herein,
 including but not limited to any patent rights that may be infringed by your
 derivative works or by other works in which the Apple Software may be
 incorporated.
 
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
 WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
 WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
 COMBINATION WITH YOUR PRODUCTS.
 
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
 DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
 CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
 APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 Copyright (C) 2008 Apple Inc. All Rights Reserved.
 
 */

#import <OpenGLES/ES1/glext.h>

#import "Texture2D.h"
#import "const.h"

#import "TypeUtil.h"

//CONSTANTS:

#define kMaxTextureSize   1024

//CLASS IMPLEMENTATIONS:

int nameCounter__ = 0;
GLfloat globalAlpha__ = 1.f;

void Texture2D::setGlobalAlpha(GLfloat alpha) {
  globalAlpha__ = alpha;
}

void Texture2D::init(const void *data, Texture2DPixelFormat pixelFormat, NSUInteger width,
                     NSUInteger height, CGSize size) {
  NSLog(@"HERE B: %p", this);

  GLint saveName;

  //glGenTextures(1, &name_);
  name_ = ++nameCounter__;
  
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &saveName);
  glBindTexture(GL_TEXTURE_2D, name_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  switch(pixelFormat) {
      
    case kTexture2DPixelFormat_RGBA8888:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      break;
    case kTexture2DPixelFormat_RGB565:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
      break;
    case kTexture2DPixelFormat_A8:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
      break;
    default:
      [NSException raise:NSInternalInconsistencyException format:@""];
      
  }
  glBindTexture(GL_TEXTURE_2D, saveName);
  
  size_ = size;
  width_ = width;
  height_ = height;
  format_ = pixelFormat;
  maxS_ = size.width / (float)width;
  maxT_ = size.height / (float)height;
  
  coordinates_[0] = 0;
  coordinates_[1] = maxT_;
  
  coordinates_[2] = maxS_;
  coordinates_[3] = maxT_;
  
  coordinates_[4] = 0;
  coordinates_[5] = 0;
  
  coordinates_[6] = maxS_;
  coordinates_[7] = 0;
  
  
  vertices_[0] = -size.width/2.0;
  vertices_[1] = -size.height/2.0;
  vertices_[2] = 0;
  
  vertices_[3] = size.width/2.0;
  vertices_[4] = -size.height/2.0;
  vertices_[5] = 0;
  
  vertices_[6] = -size.width/2.0;
  vertices_[7] = size.height/2.0;
  vertices_[8] = 0.0;
  
  vertices_[9] = size.width/2.0;
  vertices_[10] = size.height/2.0;
  vertices_[11] = 0.0;
}

Texture2D::Texture2D(GLuint name, CGSize size, NSUInteger width, NSUInteger height,
                     Texture2DPixelFormat format, GLfloat maxS, GLfloat maxT) {
  name_ = name;
  size_ = size;
  width_ = width;
  height_ = height;
  format_ = format;
  maxS_ = maxS;
  maxT_ = maxT;
}

Texture2D::Texture2D(string filename) {
  init(filename, false, false);
}

Texture2D::Texture2D(string filename, bool silhouette, bool lighten) {
  init(filename, silhouette, lighten);
}

void Texture2D::init(string filename, bool silhouette, bool lighten) {
  resourceName_ = filename;

  NSString* filePath = [[NSBundle mainBundle] pathForResource:TypeUtil::string2NSString(filename)
                                                       ofType:nil];
  NSData* data = [NSData dataWithContentsOfFile:filePath];

  unsigned char* byteData = (unsigned char*)[data bytes];
  int originalWidth = byteData[0] + (byteData[1] << 8) + (byteData[2] << 16) + (byteData[3] << 24);
  int originalHeight = byteData[4] + (byteData[5] << 8) + (byteData[6] << 16) + (byteData[7] << 24);
  int textureWidth = byteData[8] + (byteData[9] << 8) + (byteData[10] << 16) + (byteData[11] << 24);
  int textureHeight = byteData[12] + (byteData[13] << 8) + (byteData[14] << 16) +
                      (byteData[15] << 24);
  if (silhouette) {
    for (int i = 16; i < [data length]; i += 4) {
      byteData[i] = 0;
      byteData[i+1] = 0;
      byteData[i+2] = 0;
    }
  }
  if (lighten) {
    for (int i = 16; i < [data length]; i += 4) {
      int lightenAmount = 300 - i * 300 / [data length];
      for (int j = i; j < i + 3; j++) {
        if (byteData[j] < 255 - lightenAmount) {
          byteData[j] += lightenAmount;
        } else {
          byteData[j] = 255;
        }
      }  
    }
  }
  init(byteData+16, kTexture2DPixelFormat_RGBA8888, textureWidth, textureHeight,
       CGSizeMake(originalWidth, originalHeight));
}

Texture2D::Texture2D(UIImage *uiImage) {
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
  CGSize          imageSize;
  Texture2DPixelFormat    pixelFormat;
  CGImageRef        image;
  BOOL          sizeToFit = NO;
  
  
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
  
  
  imageSize = CGSizeMake(CGImageGetWidth(image), CGImageGetHeight(image));
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
  NSLog(@"HERE A: %p", this);
  init(data, pixelFormat, width, height, imageSize);

  CGContextRelease(context);
  free(data);
}

Texture2D::Texture2D(string str, CGSize dimensions, UITextAlignment alignment, UIFont *font) {
  NSUInteger width, height, i;
  CGContextRef context;
  void *data;
  CGColorSpaceRef colorSpace;
  
  width = dimensions.width;
  if((width != 1) && (width & (width - 1))) {
    i = 1;
    while(i < width)
      i *= 2;
    width = i;
  }
  height = dimensions.height;
  if((height != 1) && (height & (height - 1))) {
    i = 1;
    while(i < height)
      i *= 2;
    height = i;
  }
  
  colorSpace = CGColorSpaceCreateDeviceGray();
  data = calloc(height, width);
  context = CGBitmapContextCreate(data, width, height, 8, width, colorSpace, kCGImageAlphaNone);
  CGColorSpaceRelease(colorSpace);
  
  
  CGContextSetGrayFillColor(context, 1.0, 1.0);
  CGContextTranslateCTM(context, 0.0, height);
  CGContextScaleCTM(context, 1.0, -1.0); //NOTE: NSString draws in UIKit referential i.e. renders upside-down compared to CGBitmapContext referential
  UIGraphicsPushContext(context);
  [TypeUtil::string2NSString(str) drawInRect:CGRectMake(0, 0, dimensions.width, dimensions.height) withFont:font lineBreakMode:UILineBreakModeWordWrap alignment:alignment];
  UIGraphicsPopContext();
  
  init(data, kTexture2DPixelFormat_A8, width, height, dimensions);
  
  CGContextRelease(context);
  free(data);
}

Texture2D::~Texture2D() {
  if (name_) {
    // TODO glDeleteTextures(1, &name_);
  }
}
  
void Texture2D::drawAtPoint(CGPoint point) {
  drawAtPoint(point, 1.f, 1.f, 0.f, 0.f);
}

void Texture2D::drawAtPoint(CGPoint point, GLfloat alpha, GLfloat zoom, GLfloat angle, GLfloat z) {
  // Swap vertical coordinate system.
  point.y = 1024.f - point.y;

  GLfloat width = (GLfloat)width_ * maxS_,
  height = (GLfloat)height_ * maxT_;

  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
    point.x *= 320.0/768.0;
    point.y *= 320.0/768.0;
    if (IS_FREE) {
    } else {
      point.y += 27;
    }  
  }  

  glLoadIdentity();
  glBindTexture(GL_TEXTURE_2D, name_);
  glVertexPointer(3, GL_FLOAT, 0, vertices_);
  glTexCoordPointer(2, GL_FLOAT, 0, coordinates_);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
  glTranslatef(point.x + width/2.0, point.y - height/2.0, 0.0);
  glRotatef(angle, 0.f, 0.f, 1.f);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(1.f, 1.f, 1.f, alpha*globalAlpha__);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Texture2D::drawAtPointLeftRatio(CGPoint point, CGFloat leftRatio) {
  // Swap vertical coordinate system.
  point.y = 1024.f - point.y;
  
  GLfloat    width = (GLfloat)width_ * maxS_ * leftRatio,
  height = (GLfloat)height_ * maxT_;
  
  coordinates_[2] = coordinates_[6] = leftRatio * maxS_;

  vertices_[0] = -width/2.0;
  vertices_[1] = -height/2.0;
  
  vertices_[3] = width/2.0;
  vertices_[4] = -height/2.0;
  
  vertices_[6] = -width/2.0;
  vertices_[7] = height/2.0;
  
  vertices_[9] = width/2.0;
  vertices_[10] = height/2.0;
  
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
    point.x *= 320.0/768.0;
    point.y *= 320.0/768.0;
    if (IS_FREE) {
    } else {
      point.y += 27;
    }  
    
  }  
  
  glLoadIdentity();
  glBindTexture(GL_TEXTURE_2D, name_);
  glVertexPointer(3, GL_FLOAT, 0, vertices_);
  glTexCoordPointer(2, GL_FLOAT, 0, coordinates_);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTranslatef(point.x + width/2.0, point.y - height/2.0, 0.0);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Texture2D::drawAtPointRightRatio(CGPoint point, CGFloat rightRatio) {
  // Swap vertical coordinate system.
  point.y = 1024.f - point.y;
  
  GLfloat    width = (GLfloat)width_ * maxS_ * rightRatio,
  height = (GLfloat)height_ * maxT_;
  
  coordinates_[0] = coordinates_[4] = (1.0 - rightRatio) * maxS_;

  vertices_[0] = -width/2.0;
  vertices_[1] = -height/2.0;
  
  vertices_[3] = width/2.0;
  vertices_[4] = -height/2.0;
  
  vertices_[6] = -width/2.0;
  vertices_[7] = height/2.0;
  
  vertices_[9] = width/2.0;
  vertices_[10] = height/2.0;
  
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
    point.x *= 320.0/768.0;
    point.y *= 320.0/768.0;
    if (IS_FREE) {
    } else {
      point.y += 27;
    }  
    
  }  
  
  glLoadIdentity();
  glBindTexture(GL_TEXTURE_2D, name_);
  glVertexPointer(3, GL_FLOAT, 0, vertices_);
  glTexCoordPointer(2, GL_FLOAT, 0, coordinates_);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTranslatef(point.x + width/2.0 + (1.0 - rightRatio) * (GLfloat)width_ * maxS_, point.y - height/2.0, 0.0);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


void Texture2D::drawAtPointAngle(CGPoint point, GLfloat angle) {
  // Swap vertical coordinate system.
  point.y = 1024.f - point.y;
  
  GLfloat    width = (GLfloat)width_ * maxS_,
  height = (GLfloat)height_ * maxT_;

  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
    point.x *= 320.0/768.0;
    point.y *= 320.0/768.0;
    if (IS_FREE) {
    } else {
      point.y += 27;
    }  
    
  }
  
  glLoadIdentity();
  glBindTexture(GL_TEXTURE_2D, name_);
  glVertexPointer(3, GL_FLOAT, 0, vertices_);
  glTexCoordPointer(2, GL_FLOAT, 0, coordinates_);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTranslatef(point.x + width/2.0, point.y - height/2.0, 0.0);
  glRotatef(angle, 0.f, 0.f, 1.f);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
