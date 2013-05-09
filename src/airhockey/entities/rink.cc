//
//  rink.cc
//  AirHockey
//
//  Created by Jon Sharkey on 2010-04-13.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "airhockey/entities/rink.h"

#include "airhockey/entities/round_thing.h"

Rink::Rink(GameSize size) {
  size_ = size;
}

void Rink::BounceOff(RoundThing *thing) {
  if (thing->is_grabbed() || !thing->IsMovable()) {
    return;
  }

  bool dampen = false;
  if (thing->x() + thing->radius() > RightX()) {
    thing->set_vx(-thing->vx());
    thing->set_x(2*(RightX() - thing->radius()) - thing->x());
    dampen = true;
  }
  if (thing->x() - thing->radius() < LeftX()) {
    thing->set_vx(-thing->vx());
    thing->set_x(2*(LeftX() + thing->radius()) - thing->x());
    dampen = true;
  }
  if (thing->y() + thing->radius() > BottomY()) {
    // Interpolate circle x (hx) when the circle reached the edge of the goal.
    double oldX = thing->x() - thing->vx();
    double oldY = thing->y() - thing->vy();
    if (oldY + thing->radius() <= BottomY()) {
      double topFraction = (thing->y() - (BottomY() - thing->radius())) / (thing->y() - oldY);
      double hx = thing->x() - (thing->x() - oldX) * topFraction;
      if (hx < GoalLeftX() || hx >= GoalRightX()) {
        thing->set_vy(-thing->vy());
        thing->set_y(2*(BottomY() - thing->radius()) - thing->y());
        dampen = true;
      }
    } else if (thing->vx() < 0 && thing->x() - thing->radius() < GoalLeftX()) {
      double outsideFraction = ((GoalLeftX() + thing->radius()) - thing->x()) / (oldX - thing->x());
      double hy = thing->y() - (thing->y() - oldY) * outsideFraction;
      if (hy >= BottomY()) {
        thing->set_vx(-thing->vx());
        thing->set_x(2*(GoalLeftX() + thing->radius()) - thing->x());
        dampen = true;
      }
    } else if (thing->vx() > 0 && thing->x() + thing->radius() > GoalRightX()) {
      double outsideFraction =
          (thing->x() - (GoalRightX() - thing->radius())) / (thing->x() - oldX);
      double hy = thing->y() - (thing->y() - oldY) * outsideFraction;
      if (hy >= BottomY()) {
        thing->set_vx(-thing->vx());
        thing->set_x(2*(GoalRightX() - thing->radius()) - thing->x());
        dampen = true;
      }
    }
  }
  if (thing->y() - thing->radius() < TopY()) {
    // Interpolate circle x (hx) when the circle reached the edge of the goal.
    double oldX = thing->x() - thing->vx();
    double oldY = thing->y() - thing->vy();
    if (oldY - thing->radius() >= TopY()) {
      double topFraction = ((TopY() + thing->radius()) - thing->y()) / (oldY - thing->y());
      double hx = thing->x() - (thing->x() - oldX) * topFraction;
      if (hx < GoalLeftX() || hx >= GoalRightX()) {
        thing->set_vy(-thing->vy());
        thing->set_y(2*(TopY() + thing->radius()) - thing->y());
        dampen = true;
      }
    } else if (thing->vx() < 0 && thing->x() - thing->radius() < GoalLeftX()) {
      double outsideFraction = ((GoalLeftX() + thing->radius()) - thing->x()) / (oldX - thing->x());
      double hy = thing->y() - (thing->y() - oldY) * outsideFraction;
      if (hy <= TopY()) {
        thing->set_vx(-thing->vx());
        thing->set_x(2*(GoalLeftX() + thing->radius()) - thing->x());
        dampen = true;
      }
    } else if (thing->vx() > 0 && thing->x() + thing->radius() > GoalRightX()) {
      double outsideFraction =
          (thing->x() - (GoalRightX() - thing->radius())) / (thing->x() - oldX);
      double hy = thing->y() - (thing->y() - oldY) * outsideFraction;
      if (hy <= TopY()) {
        thing->set_vx(-thing->vx());
        thing->set_x(2*(GoalRightX() - thing->radius()) - thing->x());
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
  if (thing->x() - thing->radius() < LeftX()) {
    thing->set_x(LeftX() + thing->radius());
  } else if (thing->x() + thing->radius() > RightX()) {
    thing->set_x(RightX() - thing->radius());
  }
  if (thing->y() - thing->radius() < TopY() &&
      (thing->x() < GoalLeftX() || thing->x() >= GoalRightX())) {
    thing->set_y(TopY() + thing->radius());
  } else if (thing->y() + thing->radius() > BottomY() &&
      (thing->x() < GoalLeftX() || thing->x() >= GoalRightX())) {
    thing->set_y(BottomY() - thing->radius());
  }
}


// ViewEntity

void Rink::Update() {
}

void Rink::Render(GamePoint offset) {
}
