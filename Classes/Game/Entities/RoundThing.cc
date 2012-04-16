//
//  RoundThing.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-14.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "RoundThing.h"

#include <cmath>

#include "Touch.h"
#import "Paddle.h"
#import "Puck.h"

#include "gameengine/ResourceLoader.h"

RoundThing::RoundThing() {
  active_ = true;
}

RoundThing::~RoundThing() {
  ResourceLoader::Instance().ReleaseResource(texture_);
}

void RoundThing::Update() {
  if (!isMovable() || !isActive()) {
    return;
  }
  
  if (!grabbed_) {
    x_ += vx_;
    y_ += vy_;
  } else {    
    vx_ = vx_ * 0.75 + (x_ - old_x_) * 0.25;
    vy_ = vy_ * 0.75 + (y_ - old_y_) * 0.25;
    old_x_ = x_;
    old_y_ = y_;
  }
}

void RoundThing::ApplyFriction() {
  if (!isGrabbed()) {
    vx_ *= friction_;
    vy_ *= friction_;
  }
}

void RoundThing::Render() {
  if (active_) {
    texture_.drawAtPoint(SGPointMake(x_ - texture_.contentSize().width/2, y_ - texture_.contentSize().height/2));
  }
}

void RoundThing::BounceOff(RoundThing *other) {
  // TODO optimize this function.
  // For now I'm just getting it to work.
  
  double dxFull = x_ - other->x();
  double dyFull = y_ - other->y();
  double radiusTotal = other->radius() + radius_;
//  double v = sqrt(self.vx * self.vx + self.vy * self.vy);
//  double otherV = sqrt(other.vx * other.vx + other.vy * other.vy);
  
  if (dxFull*dxFull + dyFull*dyFull <= radiusTotal*radiusTotal) {    
    // Normalize N
    double nL = sqrt(dxFull*dxFull + dyFull*dyFull);
    double dx = dxFull / nL;
    double dy = dyFull / nL;
    
    // *** Move the round things outside of each other. ***
    double vFraction = mass_ / (other->getMass() + mass_);
    
    if ((grabbed_ && !other->isGrabbed() && other->isMovable()) || !isMovable()) {
      vFraction = 1;
    } else if ((!grabbed_ && other->isGrabbed()) || !other->isMovable()) {
      vFraction = 0;
    }
    
    double diff = sqrt(radiusTotal*radiusTotal) - nL;
    other->set_x(other->x() - dx * diff * vFraction);
    other->set_y(other->y() - dy * diff * vFraction);
    vFraction = 1 - vFraction;
    x_ += dx * diff * vFraction;
    y_ += dy * diff * vFraction;
    
    // *** Now change the direction based on the bounce. ***
    
    // Based on this: http://stackoverflow.com/questions/573084/how-to-calculate-bounce-angle
    // But it had some problems.
    // Looked at Wikipedia dot product article to help.
    double vDn = vx_ * dx + vy_ * dy;
    double ux = dx * vDn;
    double uy = dy * vDn;
    double wx = vx_ - ux;
    double wy = vy_ - uy;
    
    double othervDn_ = other->vx() * dx + other->vy() * dy;
    double otherux_ = dx * othervDn_;
    double otheruy_ = dy * othervDn_;
    double otherwx_ = other->vx() - otherux_;
    double otherwy_ = other->vy() - otheruy_;
    
    double newux_ = (ux * (mass_ - other->getMass()) + 2.0 * other->getMass() * otherux_) / (mass_ + other->getMass());
    double newuy_ = (uy * (mass_ - other->getMass()) + 2.0 * other->getMass() * otheruy_) / (mass_ + other->getMass());  
    
    double newother_ux_ = (otherux_ * (other->getMass() - mass_) + 2.0 * mass_ * ux) / (mass_ + other->getMass());
    double newother_uy_ = (otheruy_ * (other->getMass() - mass_) + 2.0 * mass_ * uy) / (mass_ + other->getMass());    
    
//    if (!self.grabbed && !other.grabbed) {
//      NSLog(@"\n");
//      NSLog(@"Before total: %f", v * self.mass + otherV * other.mass);
//    }
    
    if (!isGrabbed() && isMovable()) {
      vx_ = newux_ + wx;
      vy_ = newuy_ + wy;
    }
    
    if (!other->isGrabbed() && other->isMovable()) {
      other->set_vx(newother_ux_ + otherwx_);
      other->set_vy(newother_uy_ + otherwy_);
    }

    // TODO: haha, this is soooo terrible.
//    BOOL onePaddle = [self isKindOfClass:[Paddle class]] || [other isKindOfClass:[Paddle class]];
//    BOOL twoPucks = [self isKindOfClass:[Puck class]] && [other isKindOfClass:[Puck class]];
//    BOOL onePuck = !twoPucks && ([self isKindOfClass:[Puck class]] || [other isKindOfClass:[Puck class]]);
//    
//    if (onePaddle && onePuck) {
//      [SoundPlayer playSound:kSoundPaddleHit];
//    } else if (onePuck) {
//      // The puck hit a post.
//      [SoundPlayer playSound:kSoundPuckRinkBounce];
//    } else if (twoPucks) {
//      [SoundPlayer playSound:kSoundTwoPuckHit];
//    }
    
    double newVSquared = vx_ * vx_ + vy_ * vy_;
    double newOtherVSquared = other->vx() * other->vx() + other->vy() * other->vy();
    
    if (newVSquared > (MAX_SPEED*MAX_SPEED)) {
      double newV = sqrt(newVSquared);
      double newRatio = MAX_SPEED / newV;
      vx_ *= newRatio;
      vy_ *= newRatio;
    }
    if (newOtherVSquared > (MAX_SPEED*MAX_SPEED)) {
      double newOtherV = sqrt(newOtherVSquared);
      double newRatio = MAX_SPEED / newOtherV;
      other->set_vx(other->vx() * newRatio);
      other->set_vy(other->vy() * newRatio);
    }    
  }  
}

void RoundThing::TouchesBegan(vector<Touch> touches) {
  if (!isGrabbable() || !isActive() || isGrabbed()) {
    return;
  }
  for (int i = 0; i < touches.size(); i++) {
    if (ContainsTouch(&touches[i])) {
      grabbed_ = true;
      grabbed_touch_ = touches[i].identifier();
      TouchesMoved(touches);
      vx_ = 0;
      vy_ = 0;
      // Set oldX_ and oldY_ here so that the velocity stays around 0.
      // This is when you touch the outside of the RoundThing and it
      // snaps to center on your touch, it doesn't have a really high
      // initial velocity.
      old_x_ = x_;
      old_y_ = y_;      
    }
  }
}

void RoundThing::TouchesMoved(vector<Touch> touches) {
  Touch* correctTouch = NULL;
  for (int i = 0; i < touches.size(); i++) {
    if (touches[i].identifier() == grabbed_touch_) {
      correctTouch = &touches[i];
      break;
    }
  }
  if (grabbed_ && correctTouch != NULL) {
    SGPoint p = correctTouch->location();
    x_ = p.x;
    y_ = p.y;
  }
}

void RoundThing::TouchesEnded(vector<Touch> touches) {
  Touch* correctTouch = NULL;
  for (int i = 0; i < touches.size(); i++) {
    if (touches[i].identifier() == grabbed_touch_) {
      correctTouch = &touches[i];
      break;
    }
  }
  if (grabbed_ && correctTouch != NULL) {
    grabbed_ = false;
    grabbed_touch_ = NULL;
  }
}

void RoundThing::ClearTouches() {
  grabbed_ = false;
  grabbed_touch_ = NULL;
}

bool RoundThing::isGrabbable() {
  return false;
}

bool RoundThing::isMovable() {
  return true;
}
   
bool RoundThing::ContainsTouch(Touch *touch) {
  double dx = touch->location().x - x_;
  double dy = touch->location().y - y_;
  return (dx*dx + dy*dy <= radius_*radius_);
}

bool RoundThing::Overlaps(RoundThing * thing) {
  double dx = thing->x() - x_;
  double dy = thing->y() - y_;
  double totalRadius = thing->radius() + radius_;
  return (dx*dx + dy*dy <= totalRadius*totalRadius);
}
