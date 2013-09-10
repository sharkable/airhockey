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

#include "gameengine/entities/animatable.h"
#include "gameengine/coordinate_types.h"
#include "gameengine/engine_view.h"

#include "sharksound/sound_player.h"

class SimpleItem;

class StoryView : public EngineView, private AnimatableDelegate {
 public:
  StoryView(GameEngine *game_engine);

  // EngineView
  bool IsCapturingTouches();
  void TouchesBegan(std::vector<Touch> touches);
  bool HandleBackButton();

  // AnimatableDelegate
  void AnimationFinished(Animatable *animatable);

 private:
  void MoveForward();

  sp<SimpleItem> story_;
  sp<SimpleItem> about_;
  GamePoint starting_image_position_;
  GamePoint resting_image_position_;
  GamePoint ending_image_position_;
  Sound *beep_sound_;
};

#endif
