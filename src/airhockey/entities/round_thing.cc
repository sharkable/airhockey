//
//  round_thing.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2010-04-14.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "airhockey/entities/round_thing.h"

#include "gameengine/coordinate_types.h"
#include "gameengine/game_engine.h"
#include "gameengine/resource_loader.h"
#include "gameengine/touch.h"

using std::string;
using std::vector;

static const double kMaxSpeed = 60;

RoundThing::RoundThing(GameEngine &game_engine)
    : sprite_(game_engine),
      x_(0),
      y_(0),
      old_x_(0),
      old_y_(0),
      vx_(0),
      vy_(0),
      radius_(0),
      mass_(0),
      friction_(0),
      grabbed_(false),
      grabbed_touch_(NULL),
      active_(true) {
}

RoundThing::RoundThing(GameEngine &game_engine, string texture_name)
    : sprite_(game_engine, texture_name),
      x_(0),
      y_(0),
      old_x_(0),
      old_y_(0),
      vx_(0),
      vy_(0),
      radius_(0),
      mass_(0),
      friction_(0),
      grabbed_(false),
      grabbed_touch_(NULL),
      active_(true) {
}

RoundThing::~RoundThing() {
  if (sprite_.texture().data_loaded()) {
    // TODO ResourceLoader::Instance().ReleaseResource(sprite_.texture());
  }
}

void RoundThing::ApplyFriction() {
  if (!is_grabbed()) {
    vx_ *= friction_;
    vy_ *= friction_;
  }
}

void RoundThing::MaybeBounceOff(RoundThing *other) {
  // TODO optimize this function.
  // For now I'm just getting it to work.

  double dx = x_ - other->x();
  double dy = y_ - other->y();
  double radius_total = other->radius() + radius_;

  double distance_squared = dx * dx + dy * dy;
  if (distance_squared <= radius_total * radius_total) {
    // Normalize N

    double distance = sqrt(distance_squared);
    double dx_normal = dx / distance;
    double dy_normal = dy / distance;

    // Move the round things outside of each other.

    double v_fraction = mass_ / (other->mass() + mass_);

    if ((is_grabbed() && !other->is_grabbed() && other->IsMovable()) || !IsMovable()) {
      v_fraction = 1;
    } else if ((!is_grabbed() && other->is_grabbed()) || !other->IsMovable()) {
      v_fraction = 0;
    }

    double overlap = radius_total - distance;
    double other_move_distance = overlap * v_fraction;
    other->set_x(other->x() - dx_normal * other_move_distance);
    other->set_y(other->y() - dy_normal * other_move_distance);

    double move_distance = overlap - other_move_distance + 1;
    x_ += dx_normal * move_distance;
    y_ += dy_normal * move_distance;

    // Now change the direction based on the bounce.

    // Based on this: http://stackoverflow.com/questions/573084/how-to-calculate-bounce-angle
    // But it had some problems.
    // Looked at Wikipedia dot product article to help.
    double v_dot_n = vx_ * dx_normal + vy_ * dy_normal;
    double ux = dx_normal * v_dot_n;
    double uy = dy_normal * v_dot_n;
    double wx = vx_ - ux;
    double wy = vy_ - uy;

    double other_v_dot_n = other->vx() * -dx_normal + other->vy() * -dy_normal;
    double other_ux = -dx_normal * other_v_dot_n;
    double other_uy = -dy_normal * other_v_dot_n;
    double other_wx = other->vx() - other_ux;
    double other_wy = other->vy() - other_uy;

    double new_ux = (ux * fabs(mass_ - other->mass()) - 2.0 * other->mass() * other_ux) /
        (mass_ + other->mass());
    double new_uy = (uy * fabs(mass_ - other->mass()) - 2.0 * other->mass() * other_uy) /
        (mass_ + other->mass());

    double new_other_ux = (other_ux * fabs(other->mass() - mass_) - 2.0 * mass_ * ux) /
        (mass_ + other->mass());
    double new_other_uy = (other_uy * fabs(other->mass() - mass_) - 2.0 * mass_ * uy) /
        (mass_ + other->mass());

    if (!is_grabbed() && IsMovable()) {
      vx_ = wx - new_ux;
      vy_ = wy - new_uy;
    }

    if (!other->is_grabbed() && other->IsMovable()) {
      other->set_vx(other_wx - new_other_ux);
      other->set_vy(other_wy - new_other_uy);
    }

    double v_squared = vx_ * vx_ + vy_ * vy_;
    double other_v_squared = other->vx() * other->vx() + other->vy() * other->vy();

    if (v_squared > (kMaxSpeed * kMaxSpeed)) {
      double new_v = sqrt(v_squared);
      double new_ratio = kMaxSpeed / new_v;
      vx_ *= new_ratio;
      vy_ *= new_ratio;
    }
    if (other_v_squared > (kMaxSpeed * kMaxSpeed)) {
      double new_other_v = sqrt(other_v_squared);
      double new_ratio = kMaxSpeed / new_other_v;
      other->set_vx(other->vx() * new_ratio);
      other->set_vy(other->vy() * new_ratio);
    }

    DidBounceOff(other, velocity() + other->velocity());
  }
}

bool RoundThing::ContainsPoint(GamePoint point) {
  double dx = point.x - x_;
  double dy = point.y - y_;
  return (dx * dx + dy * dy <= radius_ * radius_);
}

bool RoundThing::Overlaps(RoundThing *thing) {
  double dx = thing->x() - x_;
  double dy = thing->y() - y_;
  double totalRadius = thing->radius() + radius_;
  return (dx*dx + dy*dy <= totalRadius*totalRadius);
}

bool RoundThing::IsGrabbable() {
  return false;
}

bool RoundThing::IsMovable() {
  return true;
}


#pragma mark - ViewEntity

void RoundThing::Update() {
  if (!IsMovable() || !is_active()) {
    return;
  }

  if (!is_grabbed()) {
    x_ += vx_;
    y_ += vy_;
  } else {
    vx_ = vx_ * 0.75 + (x_ - old_x_) * 0.25;
    vy_ = vy_ * 0.75 + (y_ - old_y_) * 0.25;
    old_x_ = x_;
    old_y_ = y_;
  }
}

void RoundThing::Render(GamePoint offset) {
  if (active_) {
    sprite_.DrawAtPoint(game_point_make(x_ - sprite_.content_size().width / 2 + offset.x,
                                        y_ - sprite_.content_size().height / 2 + offset.y));
  }
}

bool RoundThing::TouchBegan(GamePoint offset, Touch touch) {
  if (!IsGrabbable() || !is_active() || is_grabbed()) {
    return false;
  }
  if (ContainsPoint(touch.location() - offset)) {
    grabbed_ = true;
    grabbed_touch_ = touch.identifier();
    TouchMoved(offset, touch);
    vx_ = 0;
    vy_ = 0;
    // Set oldX_ and oldY_ here so that the velocity stays around 0.
    // This is when you touch the outside of the RoundThing and it
    // snaps to center on your touch, it doesn't have a really high
    // initial velocity.
    old_x_ = x_;
    old_y_ = y_;
    return true;
  }
  return false;
}

void RoundThing::TouchMoved(GamePoint offset, Touch touch) {
  if (!IsMovable()) {
    return;
  }
  if (is_grabbed() && touch.identifier() == grabbed_touch_) {
    GamePoint p = touch.location() - offset;
    x_ = p.x;
    y_ = p.y;
  }
}

void RoundThing::TouchEnded(GamePoint offset, Touch touch) {
  if (is_grabbed() && touch.identifier() == grabbed_touch_) {
    grabbed_ = false;
    grabbed_touch_ = NULL;
  }
}

void RoundThing::ClearTouches() {
  grabbed_ = false;
  grabbed_touch_ = NULL;
}

void RoundThing::HandleMouseDelta(float delta_x, float delta_y) {
  if (!IsGrabbable() || !is_active() || !IsMovable()) {
    return;
  }
  static const float kMaxDelta = 80.f;
  float length = sqrt(delta_x * delta_x + delta_y * delta_y);
  if (length > kMaxDelta) {
    delta_x *= kMaxDelta;
    delta_x /= length;
    delta_y *= kMaxDelta;
    delta_y /= length;
  }
  x_ += delta_x;
  y_ += delta_y;
}

//bool RoundThing::isGrabbable() {
//  return false;
//}
//
//bool RoundThing::isMovable() {
//  return true;
//}
//
//bool RoundThing::containsTouch(Touch *touch) {
//  double dx = touch->location().x - x_;
//  double dy = touch->location().y - y_;
//  return (dx*dx + dy*dy <= radius_*radius_);
//}
//
//bool RoundThing::overlaps(RoundThing * thing) {
//  double dx = thing->getX() - x_;
//  double dy = thing->getY() - y_;
//  double totalRadius = thing->getRadius() + radius_;
//  return (dx*dx + dy*dy <= totalRadius*totalRadius);
//}
