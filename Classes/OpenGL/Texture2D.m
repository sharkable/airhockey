/*
 
 ===== IMPORTANT =====
 
 This is sample code demonstrating API, technology or techniques in development.
 Although this sample code has been reviewed for technical accuracy, it is not
 final. Apple is supplying this information to help you plan for the adoption of
 the technologies and programming interfaces described herein. This information
 is subject to change, and software implemented based on this sample code should
 be tested with final operating system software and final documentation. Newer
 versions of this sample code may be provided with future seeds of the API or
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
#import "IsFree.h"
#import "const.h"

//CONSTANTS:

#define kMaxTextureSize   1024

//CLASS IMPLEMENTATIONS:

int nameCounter = 0;
GLfloat globalAlpha = 1.f;

@implementation Texture2D

@synthesize pixelFormat=_format, pixelsWide=_width, pixelsHigh=_height, name=_name, maxS=_maxS, maxT=_maxT, resourceName=_resourceName;

- (CGSize) contentSize {
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
    return _size;
  } else {
    return CGSizeMake(_size.width*(768.0/320.0), _size.height*(768.0/320.0));
  }
}

+ (void) setGlobalAlpha:(GLfloat)alpha {
  globalAlpha = alpha;
}

- (id) initWithData:(const void*)data pixelFormat:(Texture2DPixelFormat)pixelFormat pixelsWide:(NSUInteger)width pixelsHigh:(NSUInteger)height contentSize:(CGSize)size
{
  GLint          saveName;
  if((self = [super init])) {
    //glGenTextures(1, &_name);
    _name = ++nameCounter;
    
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &saveName);
    glBindTexture(GL_TEXTURE_2D, _name);
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
    
    _size = size;
    _width = width;
    _height = height;
    _format = pixelFormat;
    _maxS = size.width / (float)width;
    _maxT = size.height / (float)height;
  }
  
  coordinates[0] = 0;
  coordinates[1] = _maxT;
  
  coordinates[2] = _maxS;
  coordinates[3] = _maxT;
  
  coordinates[4] = 0;
  coordinates[5] = 0;
  
  coordinates[6] = _maxS;
  coordinates[7] = 0;
  
  
  vertices[0] = -size.width/2.0;
  vertices[1] = -size.height/2.0;
  vertices[2] = 0;
  
  vertices[3] = size.width/2.0;
  vertices[4] = -size.height/2.0;
  vertices[5] = 0;
  
  vertices[6] = -size.width/2.0;
  vertices[7] = size.height/2.0;
  vertices[8] = 0.0;
  
  vertices[9] = size.width/2.0;
  vertices[10] = size.height/2.0;
  vertices[11] = 0.0;
  
  return self;
}

- (id) initWithName:(GLuint)name size:(CGSize)size width:(NSUInteger)width height:(NSUInteger)height format:(Texture2DPixelFormat)format maxS:(GLfloat)maxS maxT:(GLfloat)maxT
{
  _name = name;
  _size = size;
  _width = width;
  _height = height;
  _format = format;
  _maxS = maxS;
  _maxT = maxT;
  
  [super init];
  return self;
}

- (void) dealloc
{
  if (_name) {
    glDeleteTextures(1, &_name);
  }
  [_resourceName release];
  [super dealloc];
}

- (NSString*) description
{
  return [NSString stringWithFormat:@"<%@ = %08X | Name = %i | Dimensions = %ix%i | Coordinates = (%.2f, %.2f)>", [self class], self, _name, _width, _height, _maxS, _maxT];
}

@end

@implementation Texture2D (File)

- (id) initWithFilename:(NSString*)filename {
  return [self initWithFilename:filename silhouette:NO lighten:NO];
}

- (id) initWithFilename:(NSString*)filename silhouette:(BOOL)silhouette {
  return [self initWithFilename:filename silhouette:silhouette lighten:NO];
}

- (id) initWithFilename:(NSString*)filename lighten:(BOOL)lighten {
  return [self initWithFilename:filename silhouette:NO lighten:lighten];
}

- (id) initWithFilename:(NSString*)filename silhouette:(BOOL)silhouette lighten:(BOOL)lighten {
  _resourceName = filename;
  [_resourceName retain];
  NSString* filePath = [[NSBundle mainBundle] pathForResource:filename ofType:nil];
  NSData* data = [NSData dataWithContentsOfFile:filePath];
  unsigned char* byteData = (unsigned char*)[data bytes];
  int originalWidth  = byteData[0]  + (byteData[1] << 8)  + (byteData[2] << 16)  + (byteData[3] << 24);
  int originalHeight = byteData[4]  + (byteData[5] << 8)  + (byteData[6] << 16)  + (byteData[7] << 24);
  int textureWidth   = byteData[8]  + (byteData[9] << 8)  + (byteData[10] << 16) + (byteData[11] << 24);
  int textureHeight  = byteData[12] + (byteData[13] << 8) + (byteData[14] << 16) + (byteData[15] << 24);
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
  [self initWithData:byteData+16
       pixelFormat:kTexture2DPixelFormat_RGBA8888
      pixelsWide:textureWidth
      pixelsHigh:textureHeight
       contentSize:CGSizeMake(originalWidth, originalHeight)];
  
  return self;
}

@end

@implementation Texture2D (Image)

- (id) initWithImage:(UIImage *)uiImage
{
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
  self = [self initWithData:data pixelFormat:pixelFormat pixelsWide:width pixelsHigh:height contentSize:imageSize];
  
  CGContextRelease(context);
  free(data);
  
  return self;
}

@end

@implementation Texture2D (Text)

- (id) initWithString:(NSString*)string dimensions:(CGSize)dimensions alignment:(UITextAlignment)alignment font:(UIFont*)font
{
  NSUInteger        width,
  height,
  i;
  CGContextRef      context;
  void*          data;
  CGColorSpaceRef      colorSpace;
  
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
  [string drawInRect:CGRectMake(0, 0, dimensions.width, dimensions.height) withFont:font lineBreakMode:UILineBreakModeWordWrap alignment:alignment];
  UIGraphicsPopContext();
  
  self = [self initWithData:data pixelFormat:kTexture2DPixelFormat_A8 pixelsWide:width pixelsHigh:height contentSize:dimensions];
  
  CGContextRelease(context);
  free(data);
  
  return self;
}

@end

@implementation Texture2D (Drawing)

- (void) drawAtPoint:(CGPoint)point 
{
  [self drawAtPoint:point alpha:1.f zoom:1.f angle:0.f z:0.f];
}

- (void) drawAtPoint:(CGPoint)point alpha:(GLfloat)alpha zoom:(GLfloat)zoom angle:(GLfloat)angle z:(GLfloat)z
{
  // Swap vertical coordinate system.
  point.y = 1024.f - point.y;

  GLfloat    width = (GLfloat)_width * _maxS,
  height = (GLfloat)_height * _maxT;

  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
    point.x *= 320.0/768.0;
    point.y *= 320.0/768.0;
    if (IS_FREE) {
    } else {
      point.y += 27;
    }  
  }  

  glLoadIdentity();
  glBindTexture(GL_TEXTURE_2D, _name);
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
  glTranslatef(point.x + width/2.0, point.y - height/2.0, 0.0);
  glRotatef(angle, 0.f, 0.f, 1.f);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(1.f, 1.f, 1.f, alpha*globalAlpha); 
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

- (void) drawAtPoint:(CGPoint)point leftRatio:(CGFloat)leftRatio
{
  // Swap vertical coordinate system.
  point.y = 1024.f - point.y;
  
  GLfloat    width = (GLfloat)_width * _maxS * leftRatio,
  height = (GLfloat)_height * _maxT;
  
  coordinates[2] = coordinates[6] = leftRatio * _maxS;

  vertices[0] = -width/2.0;
  vertices[1] = -height/2.0;
  
  vertices[3] = width/2.0;
  vertices[4] = -height/2.0;
  
  vertices[6] = -width/2.0;
  vertices[7] = height/2.0;
  
  vertices[9] = width/2.0;
  vertices[10] = height/2.0;
  
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
    point.x *= 320.0/768.0;
    point.y *= 320.0/768.0;
    if (IS_FREE) {
    } else {
      point.y += 27;
    }  
    
  }  
  
  glLoadIdentity();
  glBindTexture(GL_TEXTURE_2D, _name);
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTranslatef(point.x + width/2.0, point.y - height/2.0, 0.0);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

- (void) drawAtPoint:(CGPoint)point rightRatio:(CGFloat)rightRatio
{
  // Swap vertical coordinate system.
  point.y = 1024.f - point.y;
  
  GLfloat    width = (GLfloat)_width * _maxS * rightRatio,
  height = (GLfloat)_height * _maxT;
  
  coordinates[0] = coordinates[4] = (1.0 - rightRatio) * _maxS;

  vertices[0] = -width/2.0;
  vertices[1] = -height/2.0;
  
  vertices[3] = width/2.0;
  vertices[4] = -height/2.0;
  
  vertices[6] = -width/2.0;
  vertices[7] = height/2.0;
  
  vertices[9] = width/2.0;
  vertices[10] = height/2.0;
  
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
    point.x *= 320.0/768.0;
    point.y *= 320.0/768.0;
    if (IS_FREE) {
    } else {
      point.y += 27;
    }  
    
  }  
  
  glLoadIdentity();
  glBindTexture(GL_TEXTURE_2D, _name);
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTranslatef(point.x + width/2.0 + (1.0 - rightRatio) * (GLfloat)_width * _maxS, point.y - height/2.0, 0.0);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


- (void) drawAtPoint:(CGPoint)point angle:(GLfloat)angle
{
  // Swap vertical coordinate system.
  point.y = 1024.f - point.y;
  
  GLfloat    width = (GLfloat)_width * _maxS,
  height = (GLfloat)_height * _maxT;

  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
    point.x *= 320.0/768.0;
    point.y *= 320.0/768.0;
    if (IS_FREE) {
    } else {
      point.y += 27;
    }  
    
  }
  
  glLoadIdentity();
  glBindTexture(GL_TEXTURE_2D, _name);
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTranslatef(point.x + width/2.0, point.y - height/2.0, 0.0);
  glRotatef(angle, 0.f, 0.f, 1.f);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

@end
