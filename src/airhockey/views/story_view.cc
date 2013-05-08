//
//  story_view.cc
//  AirHockeyFree
//
//  Created by Jon Sharkey on 2010-05-17.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "airhockey/views/story_view.h"

#include "gameengine/entities/simple_item.h"
#include "gameengine/modules/sound_player.h"
#include "gameengine/coordinate_types.h"
#include "gameengine/game_engine.h"
#include "gameengine/sprite.h"

#include "airhockey/views/rink_view.h"

using std::vector;

static const int kAnimateTicks = 30;

StoryView::StoryView(sp<GameEngine> game_engine) : EngineView(game_engine) {
  Sprite story_image = Sprite(game_engine, "story");
  Sprite about_image = Sprite(game_engine, "about");

  GameSize rink_size = RinkView::RinkSizeForPlatformType(game_engine->platform_type());
  GameSize image_size = story_image.content_size();
  resting_image_position_ = game_point_make((rink_size.width - image_size.width) / 2,
                                            (rink_size.height - image_size.height) / 2);
  starting_image_position_ = resting_image_position_;
  starting_image_position_.x += image_size.width;
  ending_image_position_ = resting_image_position_;
  ending_image_position_.x -= image_size.width;

  double zoom = rink_size.width / image_size.width;

  story_.reset(new SimpleItem());
  story_->add_sprite(story_image);
  story_->set_zoom(zoom);
  story_->set_position(starting_image_position_);
  story_->AnimateToPosition(resting_image_position_, kAnimationTypeCubicEaseOut, kAnimateTicks);
  AddEntity(story_);

  about_.reset(new SimpleItem());
  about_->set_delegate(this);
  about_->add_sprite(about_image);
  about_->set_zoom(zoom);
  about_->set_position(starting_image_position_);
  AddEntity(about_);
}


// EngineView

void StoryView::TouchesBegan(vector<Touch> touches) {
  if (about_->position().x == starting_image_position_.x) {
    about_->AnimateToPosition(resting_image_position_, kAnimationTypeCubicEaseOut, kAnimateTicks);
    SoundPlayer::instance()->playSound(kSoundButton);
  } else {
    story_->AnimateToPosition(ending_image_position_, kAnimationTypeCubicEaseOut, kAnimateTicks);
    about_->AnimateToPosition(ending_image_position_, kAnimationTypeCubicEaseOut, kAnimateTicks);
    SoundPlayer::instance()->playSound(kSoundButton);
  }
}


// AnimatableDelegate

void StoryView::AnimationFinished(Animatable *animatable) {
  if (about_->position().x == ending_image_position_.x) {
    game_engine()->PopView();
  }
}
