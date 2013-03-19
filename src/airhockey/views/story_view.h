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

class SimpleItem;

class StoryView : public EngineView {
 public:
  StoryView(sp<GameEngine> game_engine);

  // EngineView
  void TouchesBegan(std::vector<Touch> touches);

 private:
  sp<SimpleItem> story_;
  sp<SimpleItem> about_;
};

#endif
