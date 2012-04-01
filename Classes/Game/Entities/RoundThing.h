//
//  RoundThing.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-14.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AirHockey_RoundThing_h
#define AirHockey_RoundThing_h

#import "StateEntity.h"
#import "Texture2D.h"
#import "ResourceLoader.h"

class RoundThing : public StateEntity {
 protected:
  Texture2D texture_;
  double   x_;
  double   y_;
  double   oldX_;
  double   oldY_;
  double   vx_;
  double   vy_;
  double   radius_;
  double   mass_;
  double   friction_;
  BOOL     grabbed_;
  void *grabbedTouch_;
  BOOL     active_;

 public:
  RoundThing();
  ~RoundThing();
  void update();
  void render();
  void touchesBegan(Touch *touches[], int numTouches);
  void touchesMoved(Touch *touches[], int numTouches);
  void touchesEnded(Touch *touches[], int numTouches);
  void clearTouches();
  
  void applyFriction();
  void bounceOff(RoundThing *other);
  bool containsTouch(Touch *touch);
  bool overlaps(RoundThing *thing);

  double getX() { return x_; }
  void setX(double x) { x_ = x; }
  double getY() { return y_; }
  void setY(double y) { y_ = y; }
  double getVX() { return vx_; }
  void setVX(double vx) { vx_ = vx; }
  double getVY() { return vy_; }
  void setVY(double vy) { vy_ = vy; }
  double getRadius() { return radius_; }
  void setRadius(double radius) { radius_ = radius; }
  double getMass() { return mass_; }
  void setMass(double mass) { mass_ = mass; }
  double getFriction() { return friction_; }
  virtual bool isGrabbable();
  bool isGrabbed() { return grabbed_; }
  virtual bool isMovable();
  bool isActive() { return active_; }
  void setIsActive(bool active) { active_ = active; }
};

#endif
