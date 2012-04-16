//
//  Rink.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-13.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "Rink.h"

#import "Puck.h"
#import "ResourceLoader.h"
#import "const.h"

Rink::Rink() {
  texture_ = ResourceLoader::Instance().TextureWithName("rink_bg");
  renderPoint_ = SGPointMake(0, 0);
}

Rink::~Rink() {
  ResourceLoader::Instance().ReleaseResource(texture_);
}

void Rink::Update() {
}

void Rink::Render() {
  texture_.drawAtPoint(renderPoint_);
}

void Rink::bounceOff(RoundThing *thing) {
  if (thing->isGrabbed() || !thing->isMovable()) {
    return;
  }
  
  bool dampen = false;
  if (thing->x() + thing->radius() > RINK_RIGHT_X) {
    thing->set_vx(-thing->x());
    thing->set_x(2*(RINK_RIGHT_X - thing->radius()) - thing->x());
    dampen = true;
  }
  if (thing->x() - thing->radius() < RINK_LEFT_X) {
    thing->set_vx(-thing->vx());
    thing->set_x(2*(RINK_LEFT_X + thing->radius()) - thing->x());
    dampen = true;
  }
  if (thing->y() + thing->radius() > RINK_BOTTOM_Y) {
    // Interpolate circle x (hx) when the circle reached the edge of the goal.
    double oldX = thing->x() - thing->vx();
    double oldY = thing->y() - thing->vy();
    if (oldY + thing->radius() <= RINK_BOTTOM_Y) {
      double topFraction = (thing->y() - (RINK_BOTTOM_Y - thing->radius())) / (thing->y() - oldY);
      double hx = thing->x() - (thing->x() - oldX) * topFraction;
      if (hx < GOAL_LEFT_X || hx >= GOAL_RIGHT_X) {
        thing->set_vy(-thing->vy());
        thing->set_y(2*(RINK_BOTTOM_Y - thing->radius()) - thing->y());
        dampen = true;        
      }
    } else if (thing->vx() < 0 && thing->x() - thing->radius() < GOAL_LEFT_X) {
      double outsideFraction = ((GOAL_LEFT_X + thing->radius()) - thing->x()) / (oldX - thing->x());
      double hy = thing->y() - (thing->y() - oldY) * outsideFraction;
      if (hy >= RINK_BOTTOM_Y) {
        thing->set_vx(-thing->vx());
        thing->set_x(2*(GOAL_LEFT_X + thing->radius()) - thing->x());
        dampen = true;          
      }
    } else if (thing->vx() > 0 && thing->x() + thing->radius() > GOAL_RIGHT_X) {
      double outsideFraction = (thing->x() - (GOAL_RIGHT_X - thing->radius())) / (thing->x() - oldX);
      double hy = thing->y() - (thing->y() - oldY) * outsideFraction;
      if (hy >= RINK_BOTTOM_Y) {
        thing->set_vx(-thing->vx());
        thing->set_x(2*(GOAL_RIGHT_X - thing->radius()) - thing->x());
        dampen = true;          
      }
    }
  }
  if (thing->y() - thing->radius() < RINK_TOP_Y) {
    // Interpolate circle x (hx) when the circle reached the edge of the goal.
    double oldX = thing->x() - thing->vx();
    double oldY = thing->y() - thing->vy();
    if (oldY - thing->radius() >= RINK_TOP_Y) {
      double topFraction = ((RINK_TOP_Y + thing->radius()) - thing->y()) / (oldY - thing->y());
      double hx = thing->x() - (thing->x() - oldX) * topFraction;
      if (hx < GOAL_LEFT_X || hx >= GOAL_RIGHT_X) {
        thing->set_vy(-thing->vy());
        thing->set_y(2*(RINK_TOP_Y + thing->radius()) - thing->y());
        dampen = true;        
      }
    } else if (thing->vx() < 0 && thing->x() - thing->radius() < GOAL_LEFT_X) {
      double outsideFraction = ((GOAL_LEFT_X + thing->radius()) - thing->x()) / (oldX - thing->x());
      double hy = thing->y() - (thing->y() - oldY) * outsideFraction;
      if (hy <= RINK_TOP_Y) {
        thing->set_vx(-thing->vx());
        thing->set_x(2*(GOAL_LEFT_X + thing->radius()) - thing->x());
        dampen = true;          
      }
    } else if (thing->vx() > 0 && thing->x() + thing->radius() > GOAL_RIGHT_X) {
      double outsideFraction = (thing->x() - (GOAL_RIGHT_X - thing->radius())) / (thing->x() - oldX);
      double hy = thing->y() - (thing->y() - oldY) * outsideFraction;
      if (hy <= RINK_TOP_Y) {
        thing->set_vx(-thing->vx());
        thing->set_x(2*(GOAL_RIGHT_X - thing->radius()) - thing->x());
        dampen = true;          
      }
    }
  }
  
  if (dampen) {
    thing->set_vx(thing->vx() * 0.7);
    thing->set_vy(thing->vy() * 0.7);
    // TODO: haha, again.
//    if ([thing isKindOfClass:[Puck class]]) {
//      [SoundPlayer playSound:kSoundPuckRinkBounce];
//    }
  }  
}

void Rink::moveInFromEdge(RoundThing *thing) {
  if (!thing->isMovable()) {
    return;
  }
  if (thing->x() - thing->radius() < RINK_LEFT_X) {
    thing->set_x(RINK_LEFT_X + thing->radius());
  } else if (thing->x() + thing->radius() > RINK_RIGHT_X) {
    thing->set_x(RINK_RIGHT_X - thing->radius());
  }
  if (thing->y() - thing->radius() < RINK_TOP_Y && (thing->x() < GOAL_LEFT_X || thing->x() >= GOAL_RIGHT_X)) {
    thing->set_y(RINK_TOP_Y + thing->radius());
  } else if (thing->y() + thing->radius() > RINK_BOTTOM_Y && (thing->x() < GOAL_LEFT_X || thing->x() >= GOAL_RIGHT_X)) {
    thing->set_y(RINK_BOTTOM_Y - thing->radius());
  }
}
