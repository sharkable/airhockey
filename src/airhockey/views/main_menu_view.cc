//
//  main_menu_view.cc
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "airhockey/views/main_menu_view.h"

#import "const.h"
#import "airhockey/views/play_view.h"
#import "airhockey/views/story_view.h"
#import "gameengine/ad_engine.h"
#import "gameengine/game_engine.h"
#import "gameengine/local_store.h"
#import "gameengine/resource_loader.h"
#import "gameengine/sprite.h"

MainMenuView::MainMenuView(sp<GameEngine> game_engine) : EngineView(game_engine) {
  bool is_iphone = false;  // TODO UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone;
  
  Sprite background_sprite(game_engine, "rink_bg");
  SimpleItem *background = new SimpleItem(background_sprite, game_engine->position("rink_bg"));
  AddEntity(sp<SimpleItem>(background));

  // Add rink left and right pieces.
  Sprite left_rink_border_sprite(game_engine, "rink_left");
  SimpleItem *left_rink_border = new SimpleItem(left_rink_border_sprite,
                                                game_engine->position("rink_left"));
  AddEntity(left_rink_border);
  Sprite right_rink_border_sprite(game_engine, "rink_right");
  SimpleItem *right_rink_border = new SimpleItem(right_rink_border_sprite,
                                                 game_engine->position("rink_right"));
  AddEntity(right_rink_border);

  Sprite title_sprite(game_engine, "title");
  SimpleItem *title = new SimpleItem(title_sprite, game_engine->position("title"));
  AddEntity(title);
//  title->set_zoom(5);
//  title->AnimateToZoom(1, kAnimationTypeBounceEaseOut, 60);
  title->set_alpha(0);
  title->AnimateToAlpha(1, kAnimationTypeLinear, 130);

  Sprite main_menu_sprite(game_engine, "main_menu");
  GamePoint main_menu_position = game_engine->position("main_menu");
  SimpleItem *main_menu = new SimpleItem(main_menu_sprite, main_menu_position);
  AddEntity(main_menu);

  Sprite start_button_image(game_engine, "start_button");
  Sprite start_button_pressed_image(game_engine, "start_button_pressed");
  start_button_.reset(new Button());
  start_button_->set_normal_sprite(start_button_image);
  start_button_->set_pressed_sprite(start_button_pressed_image);
  start_button_->set_position(game_engine->position("start_button"));
  start_button_->set_delegate(this);
  AddEntity(start_button_);
  
  Sprite story_button_image(game_engine, "story_button");
  Sprite story_button_pressed_image(game_engine, "story_button_pressed");
  story_button_.reset(new Button());
  story_button_->set_normal_sprite(story_button_image);
  story_button_->set_pressed_sprite(story_button_pressed_image);
  story_button_->set_position(game_engine->position("story_button"));
  story_button_->set_delegate(this);
  AddEntity(story_button_);

  Sprite one_player_image(game_engine, "1_player");
  Sprite two_player_image(game_engine, "2_player");
  Sprite one_player_selected_image(game_engine, "1_player_selected");
  Sprite two_player_selected_image(game_engine, "2_player_selected");
  num_players_select_.reset(new MultiSelect());
  num_players_select_->Add(one_player_image, one_player_selected_image,
                           game_engine->position("1_player"));
  num_players_select_->Add(two_player_image, two_player_selected_image,
                           game_engine->position("2_player"));
  num_players_select_->set_selected_value(LocalStore::IntegerForKey(LS_NUM_PLAYERS));
  AddEntity(num_players_select_);

  num_pucks_select_.reset(new MultiSelect());
  for (int i = 1; i <= MAX_NUM_PUCKS; i++) {
    char pucks_str[15];
    sprintf(pucks_str, "%d", i);
    char pucks_selected_str[15];
    sprintf(pucks_selected_str, "%d_selected", i);

    Sprite num_pucks_image(game_engine, pucks_str);
    Sprite num_pucks_selected_image(game_engine, pucks_selected_str);
    GamePoint num_pucks_position = game_engine->position(pucks_str);
    num_pucks_select_->Add(num_pucks_image, num_pucks_selected_image, num_pucks_position);
  }
  num_pucks_select_->set_selected_value(LocalStore::IntegerForKey(LS_NUM_PUCKS));
  AddEntity(num_pucks_select_);

  Sprite bad_image(game_engine, "bad");
  Sprite bad_image_selected(game_engine, "bad_selected");
  Sprite good_image(game_engine, "good");
  Sprite good_image_selected(game_engine, "good_selected");
  Sprite excellent_image(game_engine, "excellent");
  Sprite excellent_image_selected(game_engine, "excellent_selected");
  Sprite amazing_image(game_engine, "amazing");
  Sprite amazing_image_selected(game_engine, "amazing_selected");
  difficulty_select_.reset(new MultiSelect());
  difficulty_select_->Add(bad_image, bad_image_selected, game_engine->position("bad"));
  difficulty_select_->Add(good_image, good_image_selected, game_engine->position("good"));
  difficulty_select_->Add(excellent_image, excellent_image_selected,
                          game_engine->position("excellent"));
  difficulty_select_->Add(amazing_image, amazing_image_selected, game_engine->position("amazing"));
  if (LocalStore::HasEntryForKey(LS_DIFFICULTY)) {
    difficulty_select_->set_selected_value(LocalStore::IntegerForKey(LS_DIFFICULTY));
  } else {
    difficulty_select_->set_selected_value(caiGood);
  }
  AddEntity(difficulty_select_);

  if (!is_iphone) {
    Sprite small_image(game_engine, "small");
    Sprite small_image_selected(game_engine, "small_selected");
    Sprite medium_image(game_engine, "medium");
    Sprite medium_image_selected(game_engine, "medium_selected");
    Sprite large_image(game_engine, "large");
    Sprite large_image_selected(game_engine, "large_selected");
    paddle_size_select_.reset(new MultiSelect());
    paddle_size_select_->Add(small_image, small_image_selected, game_point_make(139, 842));
    paddle_size_select_->Add(medium_image, medium_image_selected, game_point_make(305, 842));
    paddle_size_select_->Add(large_image, large_image_selected, game_point_make(464, 842));
    if (LocalStore::HasEntryForKey(LS_PADDLE_SIZE)) {
      paddle_size_select_->set_selected_value(LocalStore::IntegerForKey(LS_PADDLE_SIZE));
    } else {
      paddle_size_select_->set_selected_value(psMedium);
    }
    AddEntity(paddle_size_select_);
  }

  sound_slider_.reset(new SoundSlider(game_engine,
                                      game_engine->position("sound_slider_main_menu")));
  AddEntity(sound_slider_);
}


// EngineView

void MainMenuView::ViewIsShown() {
  if (IS_FREE) {
//    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
    game_engine()->ad_engine()->SetAdAtPoint(kScreenPointZero);
//    } else {
//      [getGameEngine()->adEngine() addAdAtPoint:screen_point_make(45, 40)];  
//    }
  }
}


// ButtonDelegate

void MainMenuView::ButtonPressed(Button *button) {
  if (button == start_button_.get()) {
    PressedStart();
  } else if (button == story_button_.get()) {
    PressedStory();
  }
}


// private

void MainMenuView::PressedStart() {
  bool is_iphone = false; // TODO UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone;

  LocalStore::SetInteger(num_players_select_->selected_value(), LS_NUM_PLAYERS);
  LocalStore::SetInteger(num_pucks_select_->selected_value(), LS_NUM_PUCKS);
  LocalStore::SetInteger(difficulty_select_->selected_value(), LS_DIFFICULTY);
  if (!is_iphone) {
    LocalStore::SetInteger(paddle_size_select_->selected_value(), LS_PADDLE_SIZE);
  }

//  NSMutableDictionary *flurryData = [[[NSMutableDictionary alloc] initWithCapacity:4] autorelease];
//  [flurryData setObject:[NSNumber numberWithInt:num_players_select_.selected_value() + 1]
//                 forKey:@"NumPlayers"];
//  [flurryData setObject:[NSNumber numberWithInt:num_pucks_select_.selected_value() + 1]
//                 forKey:@"NumPucks"];
//  [flurryData setObject:[NSNumber numberWithInt:difficulty_select_.selected_value()]
//                 forKey:@"Difficulty"];
//  if (!is_iphone) {
//    [flurryData setObject:[NSNumber numberWithInt:paddle_size_select_.selected_value()]
//                   forKey:@"PaddleSize"];
//  }
//  [FlurryAnalytics logEvent:@"START_GAME" withParameters:flurryData];

  PaddleSize paddle_size = PaddleSize(is_iphone ? psLarge : paddle_size_select_->selected_value());
  PlayView *play_view = new PlayView(game_engine(),
                                     num_players_select_->selected_value() + 1,
                                     num_pucks_select_->selected_value() + 1,
                                     ComputerAI(difficulty_select_->selected_value()),
                                     paddle_size);
  game_engine()->SetRootView(sp<EngineView>(play_view));
}

void MainMenuView::PressedStory() {
//  [FlurryAnalytics logEvent:@"STORY_PRESSED"];
//  [getGameEngine()->adEngine() removeAd];
  game_engine()->PushView(sp<EngineView>(new StoryView(game_engine())));
}
