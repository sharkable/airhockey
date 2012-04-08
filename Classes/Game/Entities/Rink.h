//
//  Rink.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-13.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AirHockey_Rink_h
#define AirHockey_Rink_h

#import "StateEntity.h"
#import "RoundThing.h"
#import "Texture2D.h"

class Rink : public StateEntity {
 private:
  Texture2D texture_;
  SGPoint renderPoint_;
 public:
  Rink();
  ~Rink();
  void update();
  void render();
  void bounceOff(RoundThing *thing);
  void moveInFromEdge(RoundThing *thing);
};

#endif
