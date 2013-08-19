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

StoryView::StoryView(GameEngine *game_engine) : EngineView(game_engine) {
  bool use_pc = game_engine->platform_type() == kPlatformTypePC;
  Sprite story_image = Sprite(game_engine, use_pc ? "story_pc" : "story");
  Sprite about_image = Sprite(game_engine, use_pc ? "about_pc" : "about");

  GameSize rink_size = RinkView::RinkSizeForPlatformType(game_engine->platform_type());
  GameSize image_size = story_image.content_size();
  double zoom = rink_size.width / image_size.width;

  resting_image_position_ = game_point_make((rink_size.width - image_size.width) / 2,
                                            (rink_size.height - image_size.height) / 2);
  starting_image_position_ = resting_image_position_;
  starting_image_position_.x += image_size.width * zoom;
  ending_image_position_ = resting_image_position_;
  ending_image_position_.x -= image_size.width * zoom;

  story_.reset(new SimpleItem());
  story_->add_sprite(story_image);
  story_->set_zoom(zoom);
  story_->set_position(starting_image_position_);
  story_->AnimateToPosition(resting_image_position_, kAnimationTypeCubicEaseOut, kAnimateTicks);
  AddEntity(story_);

  about_.reset(new SimpleItem());
  about_->set_animatable_delegate(this);
  about_->add_sprite(about_image);
  about_->set_zoom(zoom);
  about_->set_position(starting_image_position_);
  AddEntity(about_);
}


#pragma mark - EngineView

bool StoryView::IsCapturingTouches() {
  return about_->position().x >= resting_image_position_.x;
}

void StoryView::TouchesBegan(vector<Touch> touches) {
  MoveForward();
}

bool StoryView::HandleBackButton() {
  MoveForward();
  return true;
}


#pragma mark - AnimatableDelegate

void StoryView::AnimationFinished(Animatable *animatable) {
  if (about_->position().x == ending_image_position_.x) {
    game_engine()->RemoveView(this);
  }
}


#pragma mark - private

void StoryView::MoveForward() {
  if (about_->position().x == starting_image_position_.x) {
    about_->AnimateToPosition(resting_image_position_, kAnimationTypeCubicEaseOut, kAnimateTicks);
    SoundPlayer::instance()->playSound(kSoundButton);
  } else {
    story_->AnimateToPosition(ending_image_position_, kAnimationTypeCubicEaseOut, kAnimateTicks);
    about_->AnimateToPosition(ending_image_position_, kAnimationTypeCubicEaseOut, kAnimateTicks);
    SoundPlayer::instance()->playSound(kSoundButton);
  }
}
