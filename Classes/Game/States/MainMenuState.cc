//
//  MainMenuState.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "MainMenuState.h"

#import "Texture2D.h"
#import "PlayState.h"
#import "StoryState.h"
#import "game_engine.h"
#import "LocalStore.h"
#import "ResourceLoader.h"
#import "const.h"

MainMenuState::MainMenuState(GameEngine &gameEngine) : EngineState(gameEngine), soundSlider_(SGPointMake(400, 50)) {
  bool isIPhone = true;  // TODO UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone;
  
  Texture2D backgroundTexture = ResourceLoader::instance()->getTextureWithName("rink_bg");
  SimpleItem *background = new SimpleItem(backgroundTexture, SGPointMake(0, 0));
  AddEntity(*background);

  // Add rink left and right pieces.
  Texture2D leftRinkBorderTexture = ResourceLoader::instance()->getTextureWithName("rink_left");
  SimpleItem *leftRinkBorder = new SimpleItem(leftRinkBorderTexture, SGPointMake(0, 0));
  AddEntity(*leftRinkBorder);
  Texture2D rightRinkBorderTexture =
      ResourceLoader::instance()->getTextureWithName("rink_right");
  SGPoint leftRinkBorderPos = SGPointMake(SCREEN_WIDTH - rightRinkBorderTexture.contentSize().width,
                                          0);
  SimpleItem *rightRinkBorder = new SimpleItem(rightRinkBorderTexture, leftRinkBorderPos);
  AddEntity(*rightRinkBorder);

  Texture2D titleTexture = ResourceLoader::instance()->getTextureWithName("title");
  SimpleItem *title = new SimpleItem(titleTexture, SGPointMake(81, 53));
  AddEntity(*title);
  
  Texture2D mainMenuTexture = ResourceLoader::instance()->getTextureWithName("main_menu");
  SGPoint mainMenuPosition = SGPointMake((SCREEN_WIDTH - mainMenuTexture.contentSize().width) / 2,
                                         339);
  SimpleItem *mainMenu = new SimpleItem(mainMenuTexture, mainMenuPosition);
  AddEntity(*mainMenu);
  
  Texture2D startButtonImage = ResourceLoader::instance()->getTextureWithName("start_button");
  Texture2D startButtonPressedImage =
      ResourceLoader::instance()->getTextureWithName("start_button_pressed");
  SGPoint startButtonPosition =
      SGPointMake((SCREEN_WIDTH - startButtonImage.contentSize().width) / 2, 392);
  startButton_.set_normal_texture(startButtonImage);
  startButton_.set_pressed_texture(startButtonPressedImage);
  startButton_.set_position(startButtonPosition);
  startButton_.set_delegate(this);
  AddEntity(startButton_);
  
  Texture2D feedbackButtonImage =
      ResourceLoader::instance()->getTextureWithName("feedback_button");
  Texture2D feedbackButtonPressedImage =
      ResourceLoader::instance()->getTextureWithName("feedback_button_pressed");
  feedbackButton_.set_normal_texture(feedbackButtonImage);
  feedbackButton_.set_pressed_texture(feedbackButtonPressedImage);
  feedbackButton_.set_position(isIPhone ? SGPointMake(440, 926) : SGPointMake(486, 936));
  feedbackButton_.set_delegate(this);
  AddEntity(feedbackButton_);

  Texture2D storyButtonImage = ResourceLoader::instance()->getTextureWithName("story_button");
  Texture2D storyButtonPressedImage =
      ResourceLoader::instance()->getTextureWithName("story_button_pressed");
  storyButton_.set_normal_texture(storyButtonImage);
  storyButton_.set_pressed_texture(storyButtonPressedImage);
  storyButton_.set_position(isIPhone ? SGPointMake(86, 926) : SGPointMake(91, 936));
  storyButton_.set_delegate(this);
  AddEntity(storyButton_);
  
  if (false) {
    Texture2D upgradeButtonImage =
        ResourceLoader::instance()->getTextureWithName("upgrade_button");
    Texture2D upgradeButtonPressedImage =
        ResourceLoader::instance()->getTextureWithName("upgrade_button_pressed");
    upgradeButton_.set_normal_texture(upgradeButtonImage);
    upgradeButton_.set_pressed_texture(upgradeButtonPressedImage);
    upgradeButton_.set_position(SGPointMake(91, 936));
    upgradeButton_.set_delegate(this);
    AddEntity(upgradeButton_);
  }
  
  double playersY = isIPhone ? 570 : 511;
  Texture2D onePlayerImage = ResourceLoader::instance()->getTextureWithName("1_player");
  Texture2D twoPlayerImage = ResourceLoader::instance()->getTextureWithName("2_player");
  Texture2D onePlayerSelectedImage =
      ResourceLoader::instance()->getTextureWithName("1_player_selected");
  Texture2D twoPlayerSelectedImage =
      ResourceLoader::instance()->getTextureWithName("2_player_selected");
  SGPoint numPlayersSelectPosition =
      SGPointMake(SCREEN_WIDTH / 2 - onePlayerImage.contentSize().width, playersY);
  numPlayersSelect_.Add(onePlayerImage, onePlayerSelectedImage, numPlayersSelectPosition);
  numPlayersSelect_.Add(twoPlayerImage, twoPlayerSelectedImage, SGPointMake(SCREEN_WIDTH/2, playersY));
  numPlayersSelect_.set_selected_value(LocalStore::IntegerForKey(LS_NUM_PLAYERS));
  AddEntity(numPlayersSelect_);
  
  double pucksY = isIPhone ? 706 : 627;
  double pucks1X = isIPhone ? 40 : 138.5;
  double pucks2X = isIPhone ? 45 : 142.5;
  double pucksXSpread = isIPhone ? 96 : 69;
  for (int i = 1; i <= (false ? 4 : MAX_NUM_PUCKS); i++) {
    char pucksstr[15];
    sprintf(pucksstr, "%d", i);
    char pucksselectedstr[15];
    sprintf(pucksselectedstr, "%d_selected", i);

    Texture2D numPucksImage = ResourceLoader::instance()->getTextureWithName(pucksstr);
    Texture2D numPucksSelectedImage =
        ResourceLoader::instance()->getTextureWithName(pucksselectedstr);
    SGPoint numPucksSelectPosition =
        SGPointMake(i == 1 ? pucks1X : pucks2X + pucksXSpread * (i - 1), pucksY);
    numPucksSelect_.Add(numPucksImage, numPucksSelectedImage, numPucksSelectPosition);
  }
  numPucksSelect_.set_selected_value(LocalStore::IntegerForKey(LS_NUM_PUCKS));
  AddEntity(numPucksSelect_);
  
  if (false) {
    Texture2D upgradeForMoreTexture =
        ResourceLoader::instance()->getTextureWithName("upgrade_for_more");
    SGPoint upgradeForMorePosition = SGPointMake(pucks2X + pucksXSpread * 4, pucksY);
    SimpleItem *upgradeForMore = new SimpleItem(upgradeForMoreTexture, upgradeForMorePosition);
    AddEntity(*upgradeForMore);
  }
  
  
  double difficultyY = isIPhone ? 839 : 734;
  Texture2D badImage = ResourceLoader::instance()->getTextureWithName("bad");
  Texture2D badImageSelected = ResourceLoader::instance()->getTextureWithName("bad_selected");
  Texture2D goodImage = ResourceLoader::instance()->getTextureWithName("good");
  Texture2D goodImageSelected = ResourceLoader::instance()->getTextureWithName("good_selected");
  Texture2D excellentImage = ResourceLoader::instance()->getTextureWithName("excellent");
  Texture2D excellentImageSelected =
      ResourceLoader::instance()->getTextureWithName("excellent_selected");
  Texture2D amazingImage = ResourceLoader::instance()->getTextureWithName("amazing");
  Texture2D amazingImageSelected =
      ResourceLoader::instance()->getTextureWithName("amazing_selected");
  difficultySelect_.Add(badImage, badImageSelected, SGPointMake(isIPhone ? 38 : 138, difficultyY));
  difficultySelect_.Add(goodImage, goodImageSelected, SGPointMake(isIPhone ? 216 : 266, difficultyY));
  difficultySelect_.Add(excellentImage, excellentImageSelected, SGPointMake(isIPhone ? 380 : 384, difficultyY));
  difficultySelect_.Add(amazingImage, amazingImageSelected, SGPointMake(isIPhone ? 543 : 502, difficultyY));
  if (LocalStore::HasEntryForKey(LS_DIFFICULTY)) {
    difficultySelect_.set_selected_value(LocalStore::IntegerForKey(LS_DIFFICULTY));
  } else {
    difficultySelect_.set_selected_value(caiGood);
  }
  AddEntity(difficultySelect_);
  
  if (!isIPhone) {
    Texture2D smallImage = ResourceLoader::instance()->getTextureWithName("small");
    Texture2D smallImageSelected =
        ResourceLoader::instance()->getTextureWithName("small_selected");
    Texture2D mediumImage = ResourceLoader::instance()->getTextureWithName("medium");
    Texture2D mediumImageSelected =
        ResourceLoader::instance()->getTextureWithName("medium_selected");
    Texture2D largeImage = ResourceLoader::instance()->getTextureWithName("large");
    Texture2D largeImageSelected =
        ResourceLoader::instance()->getTextureWithName("large_selected");
    paddleSizeSelect_.Add(smallImage, smallImageSelected, SGPointMake(139, 842));
    paddleSizeSelect_.Add(mediumImage, mediumImageSelected, SGPointMake(305, 842));
    paddleSizeSelect_.Add(largeImage, largeImageSelected, SGPointMake(464, 842));
    if (LocalStore::HasEntryForKey(LS_PADDLE_SIZE)) {
      paddleSizeSelect_.set_selected_value(LocalStore::IntegerForKey(LS_PADDLE_SIZE));
    } else {
      paddleSizeSelect_.set_selected_value(psMedium);
    }
    AddEntity(paddleSizeSelect_);
  }
  
  AddEntity(soundSlider_);
}

void MainMenuState::stateIsShown() {
  if (IS_FREE) {
//    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
//      [getGameEngine()->adEngine() addAdAtPoint:SGPointMake(0, 0)];  
//    } else {
//      [getGameEngine()->adEngine() addAdAtPoint:SGPointMake(45, 40)];  
//    }
  }
}

void MainMenuState::pressedStart() {
  bool isIPhone = true; // TODO UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone;

  LocalStore::SetInteger(numPlayersSelect_.selected_value(), LS_NUM_PLAYERS);
  LocalStore::SetInteger(numPucksSelect_.selected_value(), LS_NUM_PUCKS);
  LocalStore::SetInteger(difficultySelect_.selected_value(), LS_DIFFICULTY);
  if (!isIPhone) {
    LocalStore::SetInteger(paddleSizeSelect_.selected_value(), LS_PADDLE_SIZE);
  }

//  NSMutableDictionary *flurryData = [[[NSMutableDictionary alloc] initWithCapacity:4] autorelease];
//  [flurryData setObject:[NSNumber numberWithInt:numPlayersSelect_.selected_value() + 1]
//                 forKey:@"NumPlayers"];
//  [flurryData setObject:[NSNumber numberWithInt:numPucksSelect_.selected_value() + 1]
//                 forKey:@"NumPucks"];
//  [flurryData setObject:[NSNumber numberWithInt:difficultySelect_.selected_value()]
//                 forKey:@"Difficulty"];
//  if (!isIPhone) {
//    [flurryData setObject:[NSNumber numberWithInt:paddleSizeSelect_.selected_value()]
//                   forKey:@"PaddleSize"];
//  }
//  [FlurryAnalytics logEvent:@"START_GAME" withParameters:flurryData];

  PaddleSize paddleSize = PaddleSize(isIPhone ? psLarge : paddleSizeSelect_.selected_value());
  PlayState *playState = new PlayState(game_engine(),
                                       numPlayersSelect_.selected_value() + 1,
                                       numPucksSelect_.selected_value() + 1,
                                       ComputerAI(difficultySelect_.selected_value()),
                                       paddleSize);
  game_engine().ReplaceTopState(playState);
}

void MainMenuState::pressedFeedback() {
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

void MainMenuState::pressedStory() {
//  [FlurryAnalytics logEvent:@"STORY_PRESSED"];
//  [getGameEngine()->adEngine() removeAd];
  game_engine().PushState(new StoryState(game_engine()));
}

void MainMenuState::pressedUpgrade() {
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


#pragma mark - ButtonDelegate

void MainMenuState::ButtonPressed(Button *button) {
  if (button == &startButton_) {
    pressedStart();
  } else if (button == &feedbackButton_) {
    pressedFeedback();
  } else if (button == &storyButton_) {
    pressedStory();
  } else if (button == &upgradeButton_) {
    pressedUpgrade();
  }
}
