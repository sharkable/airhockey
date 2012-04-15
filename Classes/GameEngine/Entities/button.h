//
//  Button.h
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_GAMEENGINE_ENTITIES_BUTTON_H_
#define AIRHOCKEY_GAMEENGINE_ENTITIES_BUTTON_H_

#include "gameengine/StateEntity.h"
#include "OpenGL/Texture2D.h"

typedef enum {
  kButtonStateNormal,
  kButtonStatePressed
} ButtonState;

class Button;
class MainMenuState;

class ButtonDelegate {
 public:
  virtual void ButtonPressed(Button *button) = 0;
};

class Button : public StateEntity {
 public:
  Button() : state_(kButtonStateNormal) {}
  Button(Texture2D normal_texture, Texture2D pressed_texture, SGPoint position);
  ~Button();
  
  bool ContainsPoint(SGPoint p);

  // StateEntity
  void Update();
  void Render();
  void TouchesBegan(vector<Touch> touches);
  void TouchesEnded(vector<Touch> touches);

  // Accessors
  void set_normal_texture(Texture2D normal_texture) { normal_texture_ = normal_texture; }
  void set_pressed_texture(Texture2D pressed_texture) { pressed_texture_ = pressed_texture; }
  void set_position(SGPoint position) { position_ = position; }
  void set_delegate(ButtonDelegate *delegate) { delegate_ = delegate; }
  SGSize size() { return normal_texture_.contentSize(); }
 
 private:
  Texture2D normal_texture_;
  Texture2D pressed_texture_;
  SGPoint position_;
  int state_;
  ButtonDelegate *delegate_;
};

#endif