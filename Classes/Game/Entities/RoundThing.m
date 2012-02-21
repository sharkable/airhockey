//
//  RoundThing.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-14.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "RoundThing.h"
#import "SoundPlayer.h"
#import "Paddle.h"
#import "Puck.h"

@implementation RoundThing

@synthesize x=_x, y=_y, vx=vx_, vy=vy_, radius=radius_, mass=mass_, friction=friction_, active=active_;

- (id) init {
  [super init];
  
  active_ = YES;
  
  return self;
}

- (void) dealloc {
  [[ResourceLoader instance] releaseResource:texture_];
  
  [super dealloc];
}

- (void) update {
  if (!self.movable || !self.active) {
    return;
  }
  
  if (!grabbed_) {
    _x += vx_;
    _y += vy_;
  } else {    
    vx_ = vx_ * 0.75 + (_x - oldX_) * 0.25;
    vy_ = vy_ * 0.75 + (_y - oldY_) * 0.25;
    oldX_ = _x;
    oldY_ = _y;
  }
}

- (void) applyFriction {
  if (!self.grabbed) {
    vx_ *= friction_;
    vy_ *= friction_;
  }
}

- (void) render {
  if (active_) {
    [texture_ drawAtPoint:CGPointMake(_x - texture_.contentSize.width/2, _y - texture_.contentSize.height/2)];
  }
}

- (void) bounceOff:(RoundThing*)other {
  // TODO optimize this function.
  // For now I'm just getting it to work.
  
  double dxFull = self.x - other.x;
  double dyFull = self.y - other.y;
  double radiusTotal = other.radius + self.radius;
//  double v = sqrt(self.vx * self.vx + self.vy * self.vy);
//  double otherV = sqrt(other.vx * other.vx + other.vy * other.vy);
  
  if (dxFull*dxFull + dyFull*dyFull <= radiusTotal*radiusTotal) {    
    // Normalize N
    double nL = sqrt(dxFull*dxFull + dyFull*dyFull);
    double dx = dxFull / nL;
    double dy = dyFull / nL;
    
    // *** Move the round things outside of each other. ***
    double vFraction = self.mass / (other.mass + self.mass);
    
    if ((self.grabbed && !other.grabbed && other.movable) || !self.movable) {
      vFraction = 1;
    } else if ((!self.grabbed && other.grabbed) || !other.movable) {
      vFraction = 0;
    }
    
    double diff = sqrt(radiusTotal*radiusTotal) - nL;
    other.x -= dx * diff * vFraction;
    other.y -= dy * diff * vFraction;
    vFraction = 1 - vFraction;
    self.x += dx * diff * vFraction;
    self.y += dy * diff * vFraction;
    
    // *** Now change the direction based on the bounce. ***
    
    // Based on this: http://stackoverflow.com/questions/573084/how-to-calculate-bounce-angle
    // But it had some problems.
    // Looked at Wikipedia dot product article to help.
    double vDn = self.vx*dx + self.vy*dy;
    double ux = dx * vDn;
    double uy = dy * vDn;
    double wx = self.vx - ux;
    double wy = self.vy - uy;
    
    double othervDn_ = other.vx*dx + other.vy*dy;
    double otherux_ = dx * othervDn_;
    double otheruy_ = dy * othervDn_;
    double otherwx_ = other.vx - otherux_;
    double otherwy_ = other.vy - otheruy_;
    
    double newux_ = (ux * (self.mass - other.mass) + 2.0 * other.mass * otherux_) / (self.mass + other.mass);
    double newuy_ = (uy * (self.mass - other.mass) + 2.0 * other.mass * otheruy_) / (self.mass + other.mass);  
    
    double newother_ux_ = (otherux_ * (other.mass - self.mass) + 2.0 * self.mass * ux) / (self.mass + other.mass);
    double newother_uy_ = (otheruy_ * (other.mass - self.mass) + 2.0 * self.mass * uy) / (self.mass + other.mass);    
    
//    if (!self.grabbed && !other.grabbed) {
//      NSLog(@"\n");
//      NSLog(@"Before total: %f", v * self.mass + otherV * other.mass);
//    }
    
    if (!self.grabbed && self.movable) {
      self.vx = newux_ + wx;
      self.vy = newuy_ + wy;
    }
    
    if (!other.grabbed && other.movable) {
      other.vx = newother_ux_ + otherwx_;
      other.vy = newother_uy_ + otherwy_;
    }

    BOOL onePaddle = [self isKindOfClass:[Paddle class]] || [other isKindOfClass:[Paddle class]];
    BOOL twoPucks  = [self isKindOfClass:[Puck class]] && [other isKindOfClass:[Puck class]];
    BOOL onePuck   = !twoPucks && ([self isKindOfClass:[Puck class]] || [other isKindOfClass:[Puck class]]);
    
    if (onePaddle && onePuck) {
      [SoundPlayer playSound:kSoundPaddleHit];
    } else if (onePuck) {
      // The puck hit a post.
      [SoundPlayer playSound:kSoundPuckRinkBounce];
    } else if (twoPucks) {
      [SoundPlayer playSound:kSoundTwoPuckHit];
    }
    
    double newVSquared = self.vx * self.vx + self.vy * self.vy;
    double newOtherVSquared = other.vx * other.vx + other.vy * other.vy;
    
    if (newVSquared > (MAX_SPEED*MAX_SPEED)) {
      double newV = sqrt(newVSquared);
      double newRatio = MAX_SPEED / newV;
      self.vx *= newRatio;
      self.vy *= newRatio;
    }
    if (newOtherVSquared > (MAX_SPEED*MAX_SPEED)) {
      double newOtherV = sqrt(newOtherVSquared);
      double newRatio = MAX_SPEED / newOtherV;
      other.vx *= newRatio;
      other.vy *= newRatio;
    }    
  }  
}


- (void) touchesBegan:(Touch*[])touches numTouches:(int)numTouches {
  if (!self.grabbable || !self.active || self.grabbed) {
    return;
  }
  for (int i = 0; i < numTouches; i++) {
    if ([self containsTouch:touches[i]]) {
      grabbed_ = YES;
      grabbedTouch_ = touches[i].identifier;
      [self touchesMoved:touches numTouches:numTouches];
      vx_ = 0;
      vy_ = 0;
      // Set oldX_ and oldY_ here so that the velocity stays around 0.
      // This is when you touch the outside of the RoundThing and it
      // snaps to center on your touch, it doesn't have a really high
      // initial velocity.
      oldX_ = _x;
      oldY_ = _y;      
    }
  }
}

- (void) touchesMoved:(Touch*[])touches numTouches:(int)numTouches {
  Touch* correctTouch = nil;
  for (int i = 0; i < numTouches; i++) {
    if (touches[i].identifier == grabbedTouch_) {
      correctTouch = touches[i];
      break;
    }
  }
  if (grabbed_ && correctTouch != nil) {
    CGPoint p = correctTouch.location;
    _x = p.x;
    _y = p.y;
  }
}

- (void) touchesEnded:(Touch*[])touches numTouches:(int)numTouches {
  Touch* correctTouch = nil;
  for (int i = 0; i < numTouches; i++) {
    if (touches[i].identifier == grabbedTouch_) {
      correctTouch = touches[i];
      break;
    }
  }
  if (grabbed_ && correctTouch != nil) {
    grabbed_ = NO;
    grabbedTouch_ = nil;
  }
}

- (void) clearTouches {
  grabbed_ = NO;
  grabbedTouch_ = nil;
}

- (BOOL) grabbable {
  return NO;
}

- (BOOL) grabbed {
  return grabbed_;
}

- (BOOL) movable {
  return YES;
}
   
- (BOOL) containsTouch:(Touch*)touch {
  double dx = touch.location.x - _x;
  double dy = touch.location.y - _y;
  return (dx*dx + dy*dy <= self.radius*self.radius);
}

- (BOOL) overlaps:(RoundThing*)thing {
  double dx = thing.x - _x;
  double dy = thing.y - _y;
  double totalRadius = thing.radius + radius_;
  return (dx*dx + dy*dy <= totalRadius*totalRadius);
}

@end
