//
//  story_view.cc
//  AirHockeyFree
//
//  Created by Jon Sharkey on 2010-05-17.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "airhockey/views/story_view.h"

#include "gameengine/entities/simple_item.h"
#include "gameengine/coordinate_types.h"
#include "gameengine/game_engine.h"
#include "gameengine/sprite.h"
#include "soundengine/sound_player.h"

using std::vector;

StoryView::StoryView(sp<GameEngine> game_engine) : EngineView(game_engine) {
  Sprite story_image = Sprite(game_engine, "story");
  GameSize screen_size = game_engine->game_size();
  // TODO hack: because of OTHER iphone issue where the game height is > 1024.
  GamePoint button_position =
      game_point_make((768 - story_image.content_size().width) / 2,
                      (1024 - story_image.content_size().height) / 2);
  double zoom = screen_size.width / story_image.content_size().width;
  story_.reset(new SimpleItem());
  story_->add_sprite(story_image);
  story_->set_zoom(zoom);
  story_->set_position(button_position);
  AddEntity(story_);

  Sprite about_image = Sprite(game_engine, "about");
  about_.reset(new SimpleItem());
  about_->add_sprite(about_image);
  about_->set_zoom(zoom);
  about_->set_position(button_position);
}


// EngineView

void StoryView::TouchesBegan(vector<Touch> touches) {
  SoundPlayer::instance()->playSound(kSoundButton);
  if (ContainsEntity(story_)) {
    RemoveEntity(story_);
    AddEntity(about_);
  } else {
    game_engine()->PopView();
  }
}
