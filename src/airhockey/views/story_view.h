//
//  story_view.h
//  AirHockeyFree
//
//  Created by Jon Sharkey on 2010-05-17.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_VIEWS_STORYVIEW_H_
#define AIRHOCKEY_VIEWS_STORYVIEW_H_

#include "gameengine/engine_view.h"
#include "gameengine/entities/button.h"

class StoryView : public EngineView, ButtonDelegate {
 public:
  StoryView(sp<GameEngine> game_engine);

  // ButtonDelegate
  void ButtonPressed(Button *button);

 private:  
  sp<Button> story_button_;
  sp<Button> about_button_;
};

#endif
