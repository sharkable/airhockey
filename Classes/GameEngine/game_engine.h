//
//  GameEngine.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-10.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "Touch.h"
#import "engine_state.h"

#define MAX_TOUCHES 20

//@class AdEngine;

class GameEngine {
 private:
//  AdEngine *adEngine_;
  
  vector<EngineState *> states_;
  vector<Touch> touchesBegan_;
  vector<Touch> touchesMoved_;
  vector<Touch> touchesEnded_;
  bool popOnNext_;
  bool replaceOnNext_;
  EngineState *nextState_;

 public:
  void update();  
  void render();

//  AdEngine *adEngine() { return adEngine_; }
  
  void pushState(EngineState *state);
  void popState();
  void replaceTopState(EngineState *state);
  
  void setTouchesBegan(vector<Touch> touchesBegan) { touchesBegan_ = touchesBegan; }
  void setTouchesMoved(vector<Touch> touchesMoved) { touchesMoved_ = touchesMoved; }
  void setTouchesEnded(vector<Touch> touchesEnded) { touchesEnded_ = touchesEnded; }
  void clearTouches();
//  void addUIView(UIView *view);
};
