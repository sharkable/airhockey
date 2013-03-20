//
//  rink.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2010-04-13.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "airhockey/entities/rink.h"

#include "airhockey/entities/round_thing.h"

static const int kRinkEdgeWidth = 23;

const int Rink::kRinkTotalWidth = 768;
const int Rink::kRinkTotalHeight = 1024;
const int Rink::kRinkLeftX = kRinkEdgeWidth;
const int Rink::kRinkCenterX = kRinkTotalWidth / 2;
const int Rink::kRinkRightX = (kRinkTotalWidth - kRinkEdgeWidth);
const int Rink::kRinkTopY = kRinkEdgeWidth;
const int Rink::kRinkCenterY = kRinkTotalHeight / 2;
const int Rink::kRinkBottomY = (kRinkTotalHeight - kRinkEdgeWidth);
const int Rink::kGoalLeftX = 200;
const int Rink::kGoalRightX = 568;

void Rink::BounceOff(RoundThing *thing) {
  if (thing->is_grabbed() || !thing->IsMovable()) {
    return;
  }

  bool dampen = false;
  if (thing->x() + thing->radius() > kRinkRightX) {
    thing->set_vx(-thing->vx());
    thing->set_x(2*(kRinkRightX - thing->radius()) - thing->x());
    dampen = true;
  }
  if (thing->x() - thing->radius() < kRinkLeftX) {
    thing->set_vx(-thing->vx());
    thing->set_x(2*(kRinkLeftX + thing->radius()) - thing->x());
    dampen = true;
  }
  if (thing->y() + thing->radius() > kRinkBottomY) {
    // Interpolate circle x (hx) when the circle reached the edge of the goal.
    double oldX = thing->x() - thing->vx();
    double oldY = thing->y() - thing->vy();
    if (oldY + thing->radius() <= kRinkBottomY) {
      double topFraction = (thing->y() - (kRinkBottomY - thing->radius())) / (thing->y() - oldY);
      double hx = thing->x() - (thing->x() - oldX) * topFraction;
      if (hx < kGoalLeftX || hx >= kGoalRightX) {
        thing->set_vy(-thing->vy());
        thing->set_y(2*(kRinkBottomY - thing->radius()) - thing->y());
        dampen = true;
      }
    } else if (thing->vx() < 0 && thing->x() - thing->radius() < kGoalLeftX) {
      double outsideFraction = ((kGoalLeftX + thing->radius()) - thing->x()) / (oldX - thing->x());
      double hy = thing->y() - (thing->y() - oldY) * outsideFraction;
      if (hy >= kRinkBottomY) {
        thing->set_vx(-thing->vx());
        thing->set_x(2*(kGoalLeftX + thing->radius()) - thing->x());
        dampen = true;
      }
    } else if (thing->vx() > 0 && thing->x() + thing->radius() > kGoalRightX) {
      double outsideFraction = (thing->x() - (kGoalRightX - thing->radius())) / (thing->x() - oldX);
      double hy = thing->y() - (thing->y() - oldY) * outsideFraction;
      if (hy >= kRinkBottomY) {
        thing->set_vx(-thing->vx());
        thing->set_x(2*(kGoalRightX - thing->radius()) - thing->x());
        dampen = true;
      }
    }
  }
  if (thing->y() - thing->radius() < kRinkTopY) {
    // Interpolate circle x (hx) when the circle reached the edge of the goal.
    double oldX = thing->x() - thing->vx();
    double oldY = thing->y() - thing->vy();
    if (oldY - thing->radius() >= kRinkTopY) {
      double topFraction = ((kRinkTopY + thing->radius()) - thing->y()) / (oldY - thing->y());
      double hx = thing->x() - (thing->x() - oldX) * topFraction;
      if (hx < kGoalLeftX || hx >= kGoalRightX) {
        thing->set_vy(-thing->vy());
        thing->set_y(2*(kRinkTopY + thing->radius()) - thing->y());
        dampen = true;
      }
    } else if (thing->vx() < 0 && thing->x() - thing->radius() < kGoalLeftX) {
      double outsideFraction = ((kGoalLeftX + thing->radius()) - thing->x()) / (oldX - thing->x());
      double hy = thing->y() - (thing->y() - oldY) * outsideFraction;
      if (hy <= kRinkTopY) {
        thing->set_vx(-thing->vx());
        thing->set_x(2*(kGoalLeftX + thing->radius()) - thing->x());
        dampen = true;
      }
    } else if (thing->vx() > 0 && thing->x() + thing->radius() > kGoalRightX) {
      double outsideFraction = (thing->x() - (kGoalRightX - thing->radius())) / (thing->x() - oldX);
      double hy = thing->y() - (thing->y() - oldY) * outsideFraction;
      if (hy <= kRinkTopY) {
        thing->set_vx(-thing->vx());
        thing->set_x(2*(kGoalRightX - thing->radius()) - thing->x());
        dampen = true;
      }
    }
  }

  if (dampen) {
    thing->set_vx(thing->vx() * 0.7);
    thing->set_vy(thing->vy() * 0.7);
    thing->DidBounceOff(this, thing->velocity());
  }
}

void Rink::MoveInFromEdge(RoundThing *thing) {
  if (!thing->IsMovable()) {
    return;
  }
  if (thing->x() - thing->radius() < kRinkLeftX) {
    thing->set_x(kRinkLeftX + thing->radius());
  } else if (thing->x() + thing->radius() > kRinkRightX) {
    thing->set_x(kRinkRightX - thing->radius());
  }
  if (thing->y() - thing->radius() < kRinkTopY && (thing->x() < kGoalLeftX || thing->x() >= kGoalRightX)) {
    thing->set_y(kRinkTopY + thing->radius());
  } else if (thing->y() + thing->radius() > kRinkBottomY && (thing->x() < kGoalLeftX || thing->x() >= kGoalRightX)) {
    thing->set_y(kRinkBottomY - thing->radius());
  }
}


// ViewEntity

void Rink::Update() {
}

void Rink::Render() {
}
