//
//  Rink.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-13.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "Rink.h"
#import "Puck.h"
#import "SoundPlayer.h"
#import "const.h"

Rink::Rink() {
  texture_ = [[ResourceLoader instance] getTextureWithName:@"rink_bg"];
  renderPoint_ = CGPointMake(0, 0);
}

Rink::~Rink() {
  [[ResourceLoader instance] releaseResource:texture_];
}

void Rink::update() {
}

void Rink::render() {
  [texture_ drawAtPoint:renderPoint_];
}

void Rink::bounceOff(RoundThing *thing) {
  if (thing->isGrabbed() || !thing->isMovable()) {
    return;
  }
  
  BOOL dampen = NO;
  if (thing->getX() + thing->getRadius() > RINK_RIGHT_X) {
    thing->setVX(-thing->getX());
    thing->setX(2*(RINK_RIGHT_X - thing->getRadius()) - thing->getX());
    dampen = YES;
  }
  if (thing->getX() - thing->getRadius() < RINK_LEFT_X) {
    thing->setVX(-thing->getVX());
    thing->setX(2*(RINK_LEFT_X + thing->getRadius()) - thing->getX());
    dampen = YES;
  }
  if (thing->getY() + thing->getRadius() > RINK_BOTTOM_Y) {
    // Interpolate circle x (hx) when the circle reached the edge of the goal.
    double oldX = thing->getX() - thing->getVX();
    double oldY = thing->getY() - thing->getVY();
    if (oldY + thing->getRadius() <= RINK_BOTTOM_Y) {
      double topFraction = (thing->getY() - (RINK_BOTTOM_Y - thing->getRadius())) / (thing->getY() - oldY);
      double hx = thing->getX() - (thing->getX() - oldX) * topFraction;
      if (hx < GOAL_LEFT_X || hx >= GOAL_RIGHT_X) {
        thing->setVY(-thing->getVY());
        thing->setY(2*(RINK_BOTTOM_Y - thing->getRadius()) - thing->getY());
        dampen = YES;        
      }
    } else if (thing->getVX() < 0 && thing->getX() - thing->getRadius() < GOAL_LEFT_X) {
      double outsideFraction = ((GOAL_LEFT_X + thing->getRadius()) - thing->getX()) / (oldX - thing->getX());
      double hy = thing->getY() - (thing->getY() - oldY) * outsideFraction;
      if (hy >= RINK_BOTTOM_Y) {
        thing->setVX(-thing->getVX());
        thing->setX(2*(GOAL_LEFT_X + thing->getRadius()) - thing->getX());
        dampen = YES;          
      }
    } else if (thing->getVX() > 0 && thing->getX() + thing->getRadius() > GOAL_RIGHT_X) {
      double outsideFraction = (thing->getX() - (GOAL_RIGHT_X - thing->getRadius())) / (thing->getX() - oldX);
      double hy = thing->getY() - (thing->getY() - oldY) * outsideFraction;
      if (hy >= RINK_BOTTOM_Y) {
        thing->setVX(-thing->getVX());
        thing->setX(2*(GOAL_RIGHT_X - thing->getRadius()) - thing->getX());
        dampen = YES;          
      }
    }
  }
  if (thing->getY() - thing->getRadius() < RINK_TOP_Y) {
    // Interpolate circle x (hx) when the circle reached the edge of the goal.
    double oldX = thing->getX() - thing->getVX();
    double oldY = thing->getY() - thing->getVY();
    if (oldY - thing->getRadius() >= RINK_TOP_Y) {
      double topFraction = ((RINK_TOP_Y + thing->getRadius()) - thing->getY()) / (oldY - thing->getY());
      double hx = thing->getX() - (thing->getX() - oldX) * topFraction;
      if (hx < GOAL_LEFT_X || hx >= GOAL_RIGHT_X) {
        thing->setVY(-thing->getVY());
        thing->setY(2*(RINK_TOP_Y + thing->getRadius()) - thing->getY());
        dampen = YES;        
      }
    } else if (thing->getVX() < 0 && thing->getX() - thing->getRadius() < GOAL_LEFT_X) {
      double outsideFraction = ((GOAL_LEFT_X + thing->getRadius()) - thing->getX()) / (oldX - thing->getX());
      double hy = thing->getY() - (thing->getY() - oldY) * outsideFraction;
      if (hy <= RINK_TOP_Y) {
        thing->setVX(-thing->getVX());
        thing->setX(2*(GOAL_LEFT_X + thing->getRadius()) - thing->getX());
        dampen = YES;          
      }
    } else if (thing->getVX() > 0 && thing->getX() + thing->getRadius() > GOAL_RIGHT_X) {
      double outsideFraction = (thing->getX() - (GOAL_RIGHT_X - thing->getRadius())) / (thing->getX() - oldX);
      double hy = thing->getY() - (thing->getY() - oldY) * outsideFraction;
      if (hy <= RINK_TOP_Y) {
        thing->setVX(-thing->getVX());
        thing->setX(2*(GOAL_RIGHT_X - thing->getRadius()) - thing->getX());
        dampen = YES;          
      }
    }
  }
  
  if (dampen) {
    thing->setVX(thing->getVX() * 0.7);
    thing->setVY(thing->getVY() * 0.7);
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
  if (thing->getX() - thing->getRadius() < RINK_LEFT_X) {
    thing->setX(RINK_LEFT_X + thing->getRadius());
  } else if (thing->getX() + thing->getRadius() > RINK_RIGHT_X) {
    thing->setX(RINK_RIGHT_X - thing->getRadius());
  }
  if (thing->getY() - thing->getRadius() < RINK_TOP_Y && (thing->getX() < GOAL_LEFT_X || thing->getX() >= GOAL_RIGHT_X)) {
    thing->setY(RINK_TOP_Y + thing->getRadius());
  } else if (thing->getY() + thing->getRadius() > RINK_BOTTOM_Y && (thing->getX() < GOAL_LEFT_X || thing->getX() >= GOAL_RIGHT_X)) {
    thing->setY(RINK_BOTTOM_Y - thing->getRadius());
  }
}
