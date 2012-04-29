//
//  main_menu_state.cc
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "game/states/main_menu_state.h"

#import "const.h"
#import "game/states/play_state.h"
#import "game/states/story_state.h"
#import "gameengine/game_engine.h"
#import "gameengine/local_store.h"
#import "gameengine/ResourceLoader.h"
#import "opengl/Texture2D.h"

MainMenuState::MainMenuState(GameEngine &game_engine)
    : EngineState(game_engine),
      sound_slider_(SGPointMake(400, 50)) {
  bool is_iphone = true;  // TODO UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone;
  
  Texture2D background_texture = ResourceLoader::Instance().TextureWithName("rink_bg");
  SimpleItem *background = new SimpleItem(background_texture, SGPointMake(0, 0));
  AddEntity(*background);

  // Add rink left and right pieces.
  Texture2D left_rink_border_texture = ResourceLoader::Instance().TextureWithName("rink_left");
  SimpleItem *left_rink_border = new SimpleItem(left_rink_border_texture, SGPointMake(0, 0));
  AddEntity(*left_rink_border);
  Texture2D right_rink_border_texture =
      ResourceLoader::Instance().TextureWithName("rink_right");
  SGPoint left_rink_border_pos = SGPointMake(SCREEN_WIDTH - right_rink_border_texture.contentSize().width,
                                          0);
  SimpleItem *right_rink_border = new SimpleItem(right_rink_border_texture, left_rink_border_pos);
  AddEntity(*right_rink_border);

  Texture2D title_texture = ResourceLoader::Instance().TextureWithName("title");
  SimpleItem *title = new SimpleItem(title_texture, SGPointMake(81, 53));
  AddEntity(*title);
  
  Texture2D main_menu_texture = ResourceLoader::Instance().TextureWithName("main_menu");
  SGPoint main_menu_position = SGPointMake((SCREEN_WIDTH - main_menu_texture.contentSize().width) / 2,
                                         339);
  SimpleItem *main_menu = new SimpleItem(main_menu_texture, main_menu_position);
  AddEntity(*main_menu);
  
  Texture2D start_button_image = ResourceLoader::Instance().TextureWithName("start_button");
  Texture2D start_button_pressed_image =
      ResourceLoader::Instance().TextureWithName("start_button_pressed");
  SGPoint start_button_position =
      SGPointMake((SCREEN_WIDTH - start_button_image.contentSize().width) / 2, 392);
  start_button_.set_normal_texture(start_button_image);
  start_button_.set_pressed_texture(start_button_pressed_image);
  start_button_.set_position(start_button_position);
  start_button_.set_delegate(this);
  AddEntity(start_button_);
  
  Texture2D feedback_button_image =
      ResourceLoader::Instance().TextureWithName("feedback_button");
  Texture2D feedback_button_pressed_image =
      ResourceLoader::Instance().TextureWithName("feedback_button_pressed");
  feedback_button_.set_normal_texture(feedback_button_image);
  feedback_button_.set_pressed_texture(feedback_button_pressed_image);
  feedback_button_.set_position(is_iphone ? SGPointMake(440, 926) : SGPointMake(486, 936));
  feedback_button_.set_delegate(this);
  AddEntity(feedback_button_);

  Texture2D story_button_image = ResourceLoader::Instance().TextureWithName("story_button");
  Texture2D story_button_pressed_image =
      ResourceLoader::Instance().TextureWithName("story_button_pressed");
  story_button_.set_normal_texture(story_button_image);
  story_button_.set_pressed_texture(story_button_pressed_image);
  story_button_.set_position(is_iphone ? SGPointMake(86, 926) : SGPointMake(91, 936));
  story_button_.set_delegate(this);
  AddEntity(story_button_);
  
  if (false) {
    Texture2D upgrade_button_image =
        ResourceLoader::Instance().TextureWithName("upgrade_button");
    Texture2D upgrade_button_pressed_image =
        ResourceLoader::Instance().TextureWithName("upgrade_button_pressed");
    upgrade_button_.set_normal_texture(upgrade_button_image);
    upgrade_button_.set_pressed_texture(upgrade_button_pressed_image);
    upgrade_button_.set_position(SGPointMake(91, 936));
    upgrade_button_.set_delegate(this);
    AddEntity(upgrade_button_);
  }
  
  double players_y = is_iphone ? 570 : 511;
  Texture2D one_player_image = ResourceLoader::Instance().TextureWithName("1_player");
  Texture2D two_player_image = ResourceLoader::Instance().TextureWithName("2_player");
  Texture2D one_player_selected_image =
      ResourceLoader::Instance().TextureWithName("1_player_selected");
  Texture2D two_player_selected_image =
      ResourceLoader::Instance().TextureWithName("2_player_selected");
  SGPoint num_players_select_position =
      SGPointMake(SCREEN_WIDTH / 2 - one_player_image.contentSize().width, players_y);
  num_players_select_.Add(one_player_image, one_player_selected_image, num_players_select_position);
  num_players_select_.Add(two_player_image, two_player_selected_image, SGPointMake(SCREEN_WIDTH/2, players_y));
  num_players_select_.set_selected_value(LocalStore::IntegerForKey(LS_NUM_PLAYERS));
  AddEntity(num_players_select_);
  
  double pucks_y = is_iphone ? 706 : 627;
  double pucks_1_x = is_iphone ? 40 : 138.5;
  double pucks_2_x = is_iphone ? 45 : 142.5;
  double pucks_x_spread = is_iphone ? 96 : 69;
  for (int i = 1; i <= (false ? 4 : MAX_NUM_PUCKS); i++) {
    char pucks_str[15];
    sprintf(pucks_str, "%d", i);
    char pucks_selected_str[15];
    sprintf(pucks_selected_str, "%d_selected", i);

    Texture2D num_pucks_image = ResourceLoader::Instance().TextureWithName(pucks_str);
    Texture2D num_pucks_selected_image =
        ResourceLoader::Instance().TextureWithName(pucks_selected_str);
    SGPoint num_pucks_select_position =
        SGPointMake(i == 1 ? pucks_1_x : pucks_2_x + pucks_x_spread * (i - 1), pucks_y);
    num_pucks_select_.Add(num_pucks_image, num_pucks_selected_image, num_pucks_select_position);
  }
  num_pucks_select_.set_selected_value(LocalStore::IntegerForKey(LS_NUM_PUCKS));
  AddEntity(num_pucks_select_);
  
  if (false) {
    Texture2D upgrade_for_more_texture =
        ResourceLoader::Instance().TextureWithName("upgrade_for_more");
    SGPoint upgrade_for_more_position = SGPointMake(pucks_2_x + pucks_x_spread * 4, pucks_y);
    SimpleItem *upgrade_for_more = new SimpleItem(upgrade_for_more_texture, upgrade_for_more_position);
    AddEntity(*upgrade_for_more);
  }
  
  
  double difficulty_y = is_iphone ? 839 : 734;
  Texture2D bad_image = ResourceLoader::Instance().TextureWithName("bad");
  Texture2D bad_image_selected = ResourceLoader::Instance().TextureWithName("bad_selected");
  Texture2D good_image = ResourceLoader::Instance().TextureWithName("good");
  Texture2D good_image_selected = ResourceLoader::Instance().TextureWithName("good_selected");
  Texture2D excellent_image = ResourceLoader::Instance().TextureWithName("excellent");
  Texture2D excellent_image_selected =
      ResourceLoader::Instance().TextureWithName("excellent_selected");
  Texture2D amazing_image = ResourceLoader::Instance().TextureWithName("amazing");
  Texture2D amazing_image_selected =
      ResourceLoader::Instance().TextureWithName("amazing_selected");
  difficulty_select_.Add(bad_image, bad_image_selected, SGPointMake(is_iphone ? 38 : 138, difficulty_y));
  difficulty_select_.Add(good_image, good_image_selected, SGPointMake(is_iphone ? 216 : 266, difficulty_y));
  difficulty_select_.Add(excellent_image, excellent_image_selected, SGPointMake(is_iphone ? 380 : 384, difficulty_y));
  difficulty_select_.Add(amazing_image, amazing_image_selected, SGPointMake(is_iphone ? 543 : 502, difficulty_y));
  if (LocalStore::HasEntryForKey(LS_DIFFICULTY)) {
    difficulty_select_.set_selected_value(LocalStore::IntegerForKey(LS_DIFFICULTY));
  } else {
    difficulty_select_.set_selected_value(caiGood);
  }
  AddEntity(difficulty_select_);
  
  if (!is_iphone) {
    Texture2D small_image = ResourceLoader::Instance().TextureWithName("small");
    Texture2D small_image_selected =
        ResourceLoader::Instance().TextureWithName("small_selected");
    Texture2D medium_image = ResourceLoader::Instance().TextureWithName("medium");
    Texture2D medium_image_selected =
        ResourceLoader::Instance().TextureWithName("medium_selected");
    Texture2D large_image = ResourceLoader::Instance().TextureWithName("large");
    Texture2D large_image_selected =
        ResourceLoader::Instance().TextureWithName("large_selected");
    paddle_size_select_.Add(small_image, small_image_selected, SGPointMake(139, 842));
    paddle_size_select_.Add(medium_image, medium_image_selected, SGPointMake(305, 842));
    paddle_size_select_.Add(large_image, large_image_selected, SGPointMake(464, 842));
    if (LocalStore::HasEntryForKey(LS_PADDLE_SIZE)) {
      paddle_size_select_.set_selected_value(LocalStore::IntegerForKey(LS_PADDLE_SIZE));
    } else {
      paddle_size_select_.set_selected_value(psMedium);
    }
    AddEntity(paddle_size_select_);
  }
  
  AddEntity(sound_slider_);
}


// EngineState

void MainMenuState::StateIsShown() {
  if (IS_FREE) {
//    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
//      [getGameEngine()->adEngine() addAdAtPoint:SGPointMake(0, 0)];  
//    } else {
//      [getGameEngine()->adEngine() addAdAtPoint:SGPointMake(45, 40)];  
//    }
  }
}


// ButtonDelegate

void MainMenuState::ButtonPressed(Button *button) {
  if (button == &start_button_) {
    PressedStart();
  } else if (button == &feedback_button_) {
    PressedFeedback();
  } else if (button == &story_button_) {
    PressedStory();
  } else if (button == &upgrade_button_) {
    PressedUpgrade();
  }
}


// private

void MainMenuState::PressedStart() {
  bool is_iphone = true; // TODO UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone;

  LocalStore::SetInteger(num_players_select_.selected_value(), LS_NUM_PLAYERS);
  LocalStore::SetInteger(num_pucks_select_.selected_value(), LS_NUM_PUCKS);
  LocalStore::SetInteger(difficulty_select_.selected_value(), LS_DIFFICULTY);
  if (!is_iphone) {
    LocalStore::SetInteger(paddle_size_select_.selected_value(), LS_PADDLE_SIZE);
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

  PaddleSize paddle_size = PaddleSize(is_iphone ? psLarge : paddle_size_select_.selected_value());
  PlayState *play_state = new PlayState(game_engine(),
                                        num_players_select_.selected_value() + 1,
                                        num_pucks_select_.selected_value() + 1,
                                        ComputerAI(difficulty_select_.selected_value()),
                                        paddle_size);
  game_engine().ReplaceTopState(play_state);
}

void MainMenuState::PressedFeedback() {
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

void MainMenuState::PressedStory() {
//  [FlurryAnalytics logEvent:@"STORY_PRESSED"];
//  [getGameEngine()->adEngine() removeAd];
  game_engine().PushState(new StoryState(game_engine()));
}

void MainMenuState::PressedUpgrade() {
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
