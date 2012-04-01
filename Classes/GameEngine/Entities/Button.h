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

class Button;
class MainMenuState;

#define BUTTON_STATE_NORMAL 0
#define BUTTON_STATE_PRESSED 1

class ButtonDelegate {
 public:
  virtual void buttonPressed(Button *button) = 0;
};

class Button : public StateEntity {
 public:
  Button() {}
  Button(Texture2D normalTexture, Texture2D pressedTexture, CGPoint position);
  ~Button();
  void update();
  void render();
  void touchesBegan(Touch *touches[], int numTouches);
  void touchesEnded(Touch *touches[], int numTouches);
  bool containsPoint(CGPoint p);
  void setNormalTexture(Texture2D normalTexture) { normalTexture_ = normalTexture; }
  void setPressedTexture(Texture2D pressedTexture) { pressedTexture_ = pressedTexture; }
  void setPosition(CGPoint position) { position_ = position; }
  void setDelegate(ButtonDelegate *delegate) { delegate_ = delegate; }
  CGSize getSize();
 
 private:
  Texture2D normalTexture_;
  Texture2D pressedTexture_;
  CGPoint position_;
  int state_;
  ButtonDelegate *delegate_;
};

#endif
