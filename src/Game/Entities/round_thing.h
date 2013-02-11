//
//  round_thing.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-14.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAME_ENTITIES_ROUNDTHING_H_
#define AIRHOCKEY_GAME_ENTITIES_ROUNDTHING_H_

#include "gameengine/sprite.h"
#include "gameengine/view_entity.h"

class RoundThing : public ViewEntity {
 public:
  RoundThing(sp<GameEngine> game_engine);
  RoundThing(sp<GameEngine> game_engine, string texture_name);
  ~RoundThing();
  
  void ApplyFriction();
  void MaybeBounceOff(RoundThing *other);
  // Will get called for one of the two round things when they bounce.
  virtual void DidBounceOff(ViewEntity *other) {};
  bool ContainsTouch(Touch *touch);
  bool Overlaps(RoundThing *thing);
  virtual bool IsGrabbable();
  virtual bool IsMovable();
  
  double x() { return x_; }
  void set_x(double x) { x_ = x; }
  double y() { return y_; }
  void set_y(double y) { y_ = y; }
  double vx() { return vx_; }
  void set_vx(double vx) { vx_ = vx; }
  double vy() { return vy_; }
  void set_vy(double vy) { vy_ = vy; }
  double radius() { return radius_; }
  void set_radius(double radius) { radius_ = radius; }
  double mass() { return mass_; }
  void set_mass(double mass) { mass_ = mass; }
  double friction() { return friction_; }
  bool is_grabbed() { return grabbed_; }
  bool is_active() { return active_; }
  void set_active(bool active) { active_ = active; }

  // ViewEntity
  void Update();
  void Render();
  void TouchesBegan(vector<Touch> touches);
  void TouchesMoved(vector<Touch> touches);
  void TouchesEnded(vector<Touch> touches);
  void ClearTouches();

 protected:
  Sprite sprite_;
  double x_;
  double y_;
  double old_x_;
  double old_y_;
  double vx_;
  double vy_;
  double radius_;
  double mass_;
  double friction_;
  bool grabbed_;
  void *grabbed_touch_;
  bool active_;
};

#endif
