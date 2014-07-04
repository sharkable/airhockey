//
//  story_view.h
//  AirHockeyFree
//
//  Created by Jon Sharkey on 2010-05-17.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_VIEWS_STORYVIEW_H_
#define AIRHOCKEY_VIEWS_STORYVIEW_H_

#include <vector>

#include "gameengine/coordinates/coordinate_types.h"
#include "gameengine/entities/animatable.h"
#include "gameengine/input/input_handler.h"
#include "gameengine/render/renderer.h"
#include "gameengine/simulation/simulator.h"

class GameEngine;

class SimpleItem;
namespace SharkSound {
  class Sound;
}

class StoryViewDelegate {
 public:
  virtual void StoryViewFinished() = 0;
};

class StoryView : public Simulator, public Renderer, public InputHandler,
    private AnimatableDelegate {
 public:
  StoryView(GameEngine &game_engine, StoryViewDelegate &delegate);

// TODO  bool HandleBackButton();

  // Simulator
  virtual void SimulateStep();

  // Renderer
  virtual void Render(CoordinateSystem const &coordinate_system);

  // InputHandler
  virtual bool HandleInputEvent(InputEvent const &event, CoordinateSystem const &coordinate_system);

  // AnimatableDelegate
  void AnimationFinished(Animatable *animatable);

 private:
  void MoveForward();

  StoryViewDelegate &delegate_;

  SimpleItem *story_;  // weak
  SimpleItem *about_;  // weak
  GamePoint starting_image_position_;
  GamePoint resting_image_position_;
  GamePoint ending_image_position_;
  SharkSound::Sound *beep_sound_;
};

#endif
