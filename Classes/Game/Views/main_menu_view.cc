//
//  main_menu_view.cc
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "game/views/main_menu_view.h"

#import "const.h"
#import "game/views/play_view.h"
#import "game/views/story_view.h"
#import "gameengine/game_engine.h"
#import "gameengine/local_store.h"
#import "gameengine/resource_loader.h"
#import "gameengine/sprite.h"

MainMenuView::MainMenuView(sp<GameEngine> game_engine) : EngineView(game_engine) {
  bool is_iphone = true;  // TODO UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone;
  
  Sprite background_sprite(game_engine, "rink_bg");
  SimpleItem *background = new SimpleItem(background_sprite, game_point_make(0, 0));
  AddEntity(sp<SimpleItem>(background));

  // Add rink left and right pieces.
  Sprite left_rink_border_sprite(game_engine, "rink_left");
  SimpleItem *left_rink_border = new SimpleItem(left_rink_border_sprite, game_point_make(0, 0));
  AddEntity(left_rink_border);
  Sprite right_rink_border_sprite(game_engine, "rink_right");
  GamePoint left_rink_border_pos =
      game_point_make(SCREEN_WIDTH - right_rink_border_sprite.content_size().width, 0);
  SimpleItem *right_rink_border = new SimpleItem(right_rink_border_sprite, left_rink_border_pos);
  AddEntity(right_rink_border);

  Sprite title_sprite(game_engine, "title");
  SimpleItem *title = new SimpleItem(title_sprite, game_point_make(81, 53));
  AddEntity(title);

  Sprite main_menu_sprite(game_engine, "main_menu");
  GamePoint main_menu_position =
      game_point_make((SCREEN_WIDTH - main_menu_sprite.content_size().width) / 2, 339);
  SimpleItem *main_menu = new SimpleItem(main_menu_sprite, main_menu_position);
  AddEntity(main_menu);

  Sprite start_button_image(game_engine, "start_button");
  Sprite start_button_pressed_image(game_engine, "start_button_pressed");
  GamePoint start_button_position =
      game_point_make((SCREEN_WIDTH - start_button_image.content_size().width) / 2, 392);
  start_button_.reset(new Button());
  start_button_->set_normal_sprite(start_button_image);
  start_button_->set_pressed_sprite(start_button_pressed_image);
  start_button_->set_position(start_button_position);
  start_button_->set_delegate(this);
  AddEntity(start_button_);
  
  Sprite feedback_button_image(game_engine, "feedback_button");
  Sprite feedback_button_pressed_image(game_engine, "feedback_button_pressed");
  feedback_button_.reset(new Button());
  feedback_button_->set_normal_sprite(feedback_button_image);
  feedback_button_->set_pressed_sprite(feedback_button_pressed_image);
  feedback_button_->set_position(is_iphone ? game_point_make(440, 926) : game_point_make(486, 936));
  feedback_button_->set_delegate(this);
  AddEntity(feedback_button_);

  Sprite story_button_image(game_engine, "story_button");
  Sprite story_button_pressed_image(game_engine, "story_button_pressed");
  story_button_.reset(new Button());
  story_button_->set_normal_sprite(story_button_image);
  story_button_->set_pressed_sprite(story_button_pressed_image);
  story_button_->set_position(is_iphone ? game_point_make(86, 926) : game_point_make(91, 936));
  story_button_->set_delegate(this);
  AddEntity(story_button_);
  
  if (false) {
    Sprite upgrade_button_image(game_engine, "upgrade_button");
    Sprite upgrade_button_pressed_image(game_engine, "upgrade_button_pressed");
    upgrade_button_.reset(new Button());
    upgrade_button_->set_normal_sprite(upgrade_button_image);
    upgrade_button_->set_pressed_sprite(upgrade_button_pressed_image);
    upgrade_button_->set_position(game_point_make(91, 936));
    upgrade_button_->set_delegate(this);
    AddEntity(upgrade_button_);
  }
  
  double players_y = is_iphone ? 570 : 511;
  Sprite one_player_image(game_engine, "1_player");
  Sprite two_player_image(game_engine, "2_player");
  Sprite one_player_selected_image(game_engine, "1_player_selected");
  Sprite two_player_selected_image(game_engine, "2_player_selected");
  GamePoint num_players_select_position =
      game_point_make(SCREEN_WIDTH / 2 - one_player_image.content_size().width, players_y);
  num_players_select_.reset(new MultiSelect());
  num_players_select_->Add(one_player_image, one_player_selected_image, num_players_select_position);
  num_players_select_->Add(two_player_image, two_player_selected_image,
                           game_point_make(SCREEN_WIDTH/2, players_y));
  num_players_select_->set_selected_value(LocalStore::IntegerForKey(LS_NUM_PLAYERS));
  AddEntity(num_players_select_);
  
  double pucks_y = is_iphone ? 706 : 627;
  double pucks_1_x = is_iphone ? 40 : 138.5;
  double pucks_2_x = is_iphone ? 45 : 142.5;
  double pucks_x_spread = is_iphone ? 96 : 69;
  num_pucks_select_.reset(new MultiSelect());
  for (int i = 1; i <= (false ? 4 : MAX_NUM_PUCKS); i++) {
    char pucks_str[15];
    sprintf(pucks_str, "%d", i);
    char pucks_selected_str[15];
    sprintf(pucks_selected_str, "%d_selected", i);

    Sprite num_pucks_image(game_engine, pucks_str);
    Sprite num_pucks_selected_image(game_engine, pucks_selected_str);
    GamePoint num_pucks_select_position =
        game_point_make(i == 1 ? pucks_1_x : pucks_2_x + pucks_x_spread * (i - 1), pucks_y);
    num_pucks_select_->Add(num_pucks_image, num_pucks_selected_image, num_pucks_select_position);
  }
  num_pucks_select_->set_selected_value(LocalStore::IntegerForKey(LS_NUM_PUCKS));
  AddEntity(num_pucks_select_);
  
  if (false) {
    Sprite upgrade_for_more_image(game_engine, "upgrade_for_more");
    GamePoint upgrade_for_more_position =
        game_point_make(pucks_2_x + pucks_x_spread * 4, pucks_y);
    SimpleItem *upgrade_for_more =
        new SimpleItem(upgrade_for_more_image, upgrade_for_more_position);
    AddEntity(upgrade_for_more);
  }
  
  
  double difficulty_y = is_iphone ? 839 : 734;
  Sprite bad_image(game_engine, "bad");
  Sprite bad_image_selected(game_engine, "bad_selected");
  Sprite good_image(game_engine, "good");
  Sprite good_image_selected(game_engine, "good_selected");
  Sprite excellent_image(game_engine, "excellent");
  Sprite excellent_image_selected(game_engine, "excellent_selected");
  Sprite amazing_image(game_engine, "amazing");
  Sprite amazing_image_selected(game_engine, "amazing_selected");
  difficulty_select_.reset(new MultiSelect());
  difficulty_select_->Add(bad_image, bad_image_selected, game_point_make(is_iphone ? 38 : 138, difficulty_y));
  difficulty_select_->Add(good_image, good_image_selected, game_point_make(is_iphone ? 216 : 266, difficulty_y));
  difficulty_select_->Add(excellent_image, excellent_image_selected, game_point_make(is_iphone ? 380 : 384, difficulty_y));
  difficulty_select_->Add(amazing_image, amazing_image_selected,
                          game_point_make(is_iphone ? 543 : 502, difficulty_y));
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

  sound_slider_.reset(new SoundSlider(game_point_make(400, 50)));
  AddEntity(sound_slider_);
}


// EngineView

void MainMenuView::ViewIsShown() {
  if (IS_FREE) {
//    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
//      [getGameEngine()->adEngine() addAdAtPoint:screen_point_make(0, 0)];  
//    } else {
//      [getGameEngine()->adEngine() addAdAtPoint:screen_point_make(45, 40)];  
//    }
  }
}


// ButtonDelegate

void MainMenuView::ButtonPressed(Button *button) {
  if (button == start_button_.get()) {
    PressedStart();
  } else if (button == feedback_button_.get()) {
    PressedFeedback();
  } else if (button == story_button_.get()) {
    PressedStory();
  } else if (button == upgrade_button_.get()) {
    PressedUpgrade();
  }
}


// private

void MainMenuView::PressedStart() {
  bool is_iphone = true; // TODO UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone;

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

void MainMenuView::PressedFeedback() {
//  NSString *free;
//  NSString *device;
//  
//  [FlurryAnalytics logEvent:@"FEEDBACK_PRESSED"];
//
//  if (IS_FREE) {
//    free = @"%20Free";
//  } else {
//    free = @"";
//  }
//  
//  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//    device = @"iPad";
//  } else {
//    device = @"iPhone/iPod";
//  }
//
//  NSString *url =
//      [NSString stringWithFormat:@"mailto:feedback@sharkable.com?"
//                                  "subject=Glide%%20Hockey%%20HD%@%%20feedback%%20(%@)",
//                                     free, device];
//  
//  [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
}

void MainMenuView::PressedStory() {
//  [FlurryAnalytics logEvent:@"STORY_PRESSED"];
//  [getGameEngine()->adEngine() removeAd];
  game_engine()->PushView(sp<EngineView>(new StoryView(game_engine())));
}

void MainMenuView::PressedUpgrade() {
// TODO
//  UIAlertView *alert = [[[UIAlertView alloc] initWithTitle:@"Upgrade"
//                                                   message:@"You can remove ads and have up to "
//                                                            "seven pucks!\nJust upgrade to the paid"
//                                                            " version.\nIt's cheap!"
//                                                  delegate:self
//                                         cancelButtonTitle:@"No thanks!"
//                                         otherButtonTitles:@"OK!", nil] autorelease];
//  [alert show];
}

//- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
//  if (buttonIndex == 1) {
//    NSString * url = @"http://itunes.com/app/GlideHockeyHD";
//    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
//  }
//}
