//
//  StateEntity.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AirHockey_StateEntity_h
#define AirHockey_StateEntity_h

class Touch;

class StateEntity {
 public:
  virtual void update() = 0;
  virtual void render() = 0;
  virtual void touchesBegan(Touch *touches[], int numTouches) {}
  virtual void touchesMoved(Touch *touches[], int numTouches) {}
  virtual void touchesEnded(Touch *touches[], int numTouches) {}
  virtual void clearTouches() {}
};

#endif
