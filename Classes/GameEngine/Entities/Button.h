//
//  Button.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AirHockey_Button_h
#define AirHockey_Button_h

#import "StateEntity.h"
#import "Texture2D.h"

#define BUTTON_STATE_NORMAL  0
#define BUTTON_STATE_PRESSED 1

class Button : public StateEntity {
 public:
  Button(Texture2D *normalTexture, Texture2D *pressedTexture, CGPoint position);
  ~Button();
  void update();
  void render();
  void touchesBegan(Touch *touches[], int numTouches);
  void touchesEnded(Touch *touches[], int numTouches);
  bool containsPoint(CGPoint p);

  id getDelegate() { return delegate_; }
  void setDelegate(id delegate) { delegate_ = delegate; }
  SEL getSelector() { return selector_; }
  void setSelector(SEL selector) { selector_ = selector; }
  CGSize getSize();
 
 private:
  Texture2D *normalTexture_;
  Texture2D *pressedTexture_;
  CGPoint position_;
  int state_;
  id delegate_;  // weak
  SEL selector_;
};

#endif
