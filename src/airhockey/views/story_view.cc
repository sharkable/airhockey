//
//  story_view.cc
//  AirHockeyFree
//
//  Created by Jon Sharkey on 2010-05-17.
//  Copyright 2010 Sharkable. All rights reserved.
//

#include "airhockey/views/story_view.h"

#include "sharkengine/platform.h"
#include "sharkengine/shark_engine.h"
#include "sharkengine/sprite.h"
#include "sharkengine/coordinates/coordinate_types.h"
#include "sharkengine/entities/simple_item.h"
#include "sharksound/sound.h"
#include "sharksound/sound_controller.h"

#include "airhockey/views/rink_view.h"

using std::vector;

static const int kAnimateTicks = 30;

StoryView::StoryView(SharkEngine &shark_engine, StoryViewDelegate &delegate) : delegate_(delegate) {
  bool use_pc = shark_engine.platform().input_group() == Platform::kInputGroupPC;
  Sprite story_image = Sprite(shark_engine, use_pc ? "story_pc" : "story");
  Sprite about_image = Sprite(shark_engine, use_pc ? "about_pc" : "about");

  GameSize rink_size = RinkView::RinkSizeForTextureGroup(shark_engine.platform().texture_group());
  GameSize image_size = story_image.content_size();
  double zoom = rink_size.width / image_size.width;

  resting_image_position_ = GamePoint((rink_size.width - image_size.width) / 2,
                                      (rink_size.height - image_size.height) / 2);
  starting_image_position_ = resting_image_position_;
  starting_image_position_.x += image_size.width * zoom;
  ending_image_position_ = resting_image_position_;
  ending_image_position_.x -= image_size.width * zoom;

  story_ = new SimpleItem();
  story_->add_sprite(story_image);
  story_->set_scale(zoom);
  story_->set_position(starting_image_position_);
  story_->AnimateToPosition(resting_image_position_, kAnimationTypeCubicEaseOut, kAnimateTicks);

  about_ = new SimpleItem();
  about_->set_animatable_delegate(this);
  about_->add_sprite(about_image);
  about_->set_scale(zoom);
  about_->set_position(starting_image_position_);

  beep_sound_ = shark_engine.sound()->GetSound("sounds/beep.wav");
}


#pragma mark - Simulator

void StoryView::SimulateStep() {
  story_->SimulateStep();
  about_->SimulateStep();
}


#pragma mark - Renderer

void StoryView::Render(CoordinateSystem const &coordinate_system) {
  story_->Render(coordinate_system);
  about_->Render(coordinate_system);
}


#pragma mark - InputHandler

bool StoryView::HandleInputEvent(InputEvent const &event,
                                 CoordinateSystem const &coordinate_system) {
  if (event.action() == InputEvent::kActionDown &&
      about_->position().x >= resting_image_position_.x) {
    MoveForward();
  }
  return true;
}

// TODO
//bool StoryView::HandleBackButton() {
//  MoveForward();
//  return true;
//}


#pragma mark - AnimatableDelegate

void StoryView::AnimationFinished(Animatable *animatable) {
  if (about_->position().x == ending_image_position_.x) {
    delegate_.StoryViewFinished();
  }
}


#pragma mark - private

void StoryView::MoveForward() {
  if (about_->position().x == starting_image_position_.x) {
    about_->AnimateToPosition(resting_image_position_, kAnimationTypeCubicEaseOut, kAnimateTicks);
    beep_sound_->Play();
  } else {
    story_->AnimateToPosition(ending_image_position_, kAnimationTypeCubicEaseOut, kAnimateTicks);
    about_->AnimateToPosition(ending_image_position_, kAnimationTypeCubicEaseOut, kAnimateTicks);
    beep_sound_->Play();
  }
}
