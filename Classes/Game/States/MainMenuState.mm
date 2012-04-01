//
//  MainMenuState.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-04-30.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "MainMenuState.h"

#import "AdEngine.h"
#import "Texture2D.h"
#import "PlayState.h"
#import "StoryState.h"
#import "GameEngine.h"
#import "EAGLView.h"
#import "LocalStore.h"
#import "ResourceLoader.h"
#import "const.h"
#import "FlurryAnalytics.h"

MainMenuState::MainMenuState(GameEngine *gameEngine) : EngineState(gameEngine), soundSlider_(CGPointMake(400, 50)) {
  BOOL isIPhone = UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone;
  
  Texture2D backgroundTexture = ResourceLoader::instance()->getTextureWithName("rink_bg");
  SimpleItem *background = new SimpleItem(backgroundTexture, CGPointMake(0, 0));
  addEntity(background);

  // Add rink left and right pieces.
  Texture2D leftRinkBorderTexture = ResourceLoader::instance()->getTextureWithName("rink_left");
  SimpleItem *leftRinkBorder = new SimpleItem(leftRinkBorderTexture, CGPointMake(0, 0));
  addEntity(leftRinkBorder);
  Texture2D rightRinkBorderTexture =
      ResourceLoader::instance()->getTextureWithName("rink_right");
  CGPoint leftRinkBorderPos = CGPointMake(SCREEN_WIDTH - rightRinkBorderTexture.contentSize().width,
                                          0);
  SimpleItem *rightRinkBorder = new SimpleItem(rightRinkBorderTexture, leftRinkBorderPos);
  addEntity(rightRinkBorder);

  Texture2D titleTexture = ResourceLoader::instance()->getTextureWithName("title");
  SimpleItem *title = new SimpleItem(titleTexture, CGPointMake(81, 53));
  addEntity(title);
  
  Texture2D mainMenuTexture = ResourceLoader::instance()->getTextureWithName("main_menu");
  CGPoint mainMenuPosition = CGPointMake((SCREEN_WIDTH - mainMenuTexture.contentSize().width) / 2,
                                         339);
  SimpleItem *mainMenu = new SimpleItem(mainMenuTexture, mainMenuPosition);
  addEntity(mainMenu);
  
  Texture2D startButtonImage = ResourceLoader::instance()->getTextureWithName("start_button");
  Texture2D startButtonPressedImage =
      ResourceLoader::instance()->getTextureWithName("start_button_pressed");
  CGPoint startButtonPosition =
      CGPointMake((SCREEN_WIDTH - startButtonImage.contentSize().width) / 2, 392);
  startButton_.setNormalTexture(&startButtonImage);
  startButton_.setPressedTexture(&startButtonPressedImage);
  startButton_.setPosition(startButtonPosition);
  startButton_.setDelegate(this);
  addEntity(&startButton_);
  
  Texture2D feedbackButtonImage =
      ResourceLoader::instance()->getTextureWithName("feedback_button");
  Texture2D feedbackButtonPressedImage =
      ResourceLoader::instance()->getTextureWithName("feedback_button_pressed");
  feedbackButton_.setNormalTexture(&feedbackButtonImage);
  feedbackButton_.setPressedTexture(&feedbackButtonPressedImage);
  feedbackButton_.setPosition(isIPhone ? CGPointMake(440, 926) : CGPointMake(486, 936));
  feedbackButton_.setDelegate(this);
  addEntity(&feedbackButton_);

  Texture2D storyButtonImage = ResourceLoader::instance()->getTextureWithName("story_button");
  Texture2D storyButtonPressedImage =
      ResourceLoader::instance()->getTextureWithName("story_button_pressed");
  storyButton_.setNormalTexture(&storyButtonImage);
  storyButton_.setPressedTexture(&storyButtonPressedImage);
  storyButton_.setPosition(isIPhone ? CGPointMake(86, 926) : CGPointMake(91, 936));
  storyButton_.setDelegate(this);
  addEntity(&storyButton_);
  
  if (NO) {
    Texture2D upgradeButtonImage =
        ResourceLoader::instance()->getTextureWithName("upgrade_button");
    Texture2D upgradeButtonPressedImage =
        ResourceLoader::instance()->getTextureWithName("upgrade_button_pressed");
    upgradeButton_.setNormalTexture(&upgradeButtonImage);
    upgradeButton_.setPressedTexture(&upgradeButtonPressedImage);
    upgradeButton_.setPosition(CGPointMake(91, 936));
    upgradeButton_.setDelegate(this);
    addEntity(&upgradeButton_);
  }
  
  double playersY = isIPhone ? 570 : 511;
  Texture2D onePlayerImage = ResourceLoader::instance()->getTextureWithName("1_player");
  Texture2D twoPlayerImage = ResourceLoader::instance()->getTextureWithName("2_player");
  Texture2D onePlayerSelectedImage =
      ResourceLoader::instance()->getTextureWithName("1_player_selected");
  Texture2D twoPlayerSelectedImage =
      ResourceLoader::instance()->getTextureWithName("2_player_selected");
  CGPoint numPlayersSelectPosition =
      CGPointMake(SCREEN_WIDTH / 2 - onePlayerImage.contentSize().width, playersY);
  numPlayersSelect_.add(onePlayerImage, onePlayerSelectedImage, numPlayersSelectPosition);
  numPlayersSelect_.add(twoPlayerImage, twoPlayerSelectedImage, CGPointMake(SCREEN_WIDTH/2, playersY));
  numPlayersSelect_.setSelectedValue(LocalStore::integerForKey(LS_NUM_PLAYERS));
  addEntity(&numPlayersSelect_);
  
  double pucksY = isIPhone ? 706 : 627;
  double pucks1X = isIPhone ? 40 : 138.5;
  double pucks2X = isIPhone ? 45 : 142.5;
  double pucksXSpread = isIPhone ? 96 : 69;
  for (int i = 1; i <= (NO ? 4 : MAX_NUM_PUCKS); i++) {
    Texture2D numPucksImage = ResourceLoader::instance()->getTextureWithName(string("%i", i));
    Texture2D numPucksSelectedImage =
        ResourceLoader::instance()->getTextureWithName(string("%i_selected", i));
    CGPoint numPucksSelectPosition =
        CGPointMake(i == 1 ? pucks1X : pucks2X + pucksXSpread * (i - 1), pucksY);
    numPucksSelect_.add(numPucksImage, numPucksSelectedImage, numPucksSelectPosition);
  }
  numPucksSelect_.setSelectedValue(LocalStore::integerForKey(LS_NUM_PUCKS));
  addEntity(&numPucksSelect_);
  
  if (NO) {
    Texture2D upgradeForMoreTexture =
        ResourceLoader::instance()->getTextureWithName("upgrade_for_more");
    CGPoint upgradeForMorePosition = CGPointMake(pucks2X + pucksXSpread * 4, pucksY);
    SimpleItem *upgradeForMore = new SimpleItem(upgradeForMoreTexture, upgradeForMorePosition);
    addEntity(upgradeForMore);
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
  difficultySelect_.add(badImage, badImageSelected, CGPointMake(isIPhone ? 38 : 138, difficultyY));
  difficultySelect_.add(goodImage, goodImageSelected, CGPointMake(isIPhone ? 216 : 266, difficultyY));
  difficultySelect_.add(excellentImage, excellentImageSelected, CGPointMake(isIPhone ? 380 : 384, difficultyY));
  difficultySelect_.add(amazingImage, amazingImageSelected, CGPointMake(isIPhone ? 543 : 502, difficultyY));
  if (LocalStore::hasEntryForKey(LS_DIFFICULTY)) {
    difficultySelect_.setSelectedValue(LocalStore::integerForKey(LS_DIFFICULTY));
  } else {
    difficultySelect_.setSelectedValue(caiGood);
  }
  addEntity(&difficultySelect_);
  
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
    paddleSizeSelect_.add(smallImage, smallImageSelected, CGPointMake(139, 842));
    paddleSizeSelect_.add(mediumImage, mediumImageSelected, CGPointMake(305, 842));
    paddleSizeSelect_.add(largeImage, largeImageSelected, CGPointMake(464, 842));
    if (LocalStore::hasEntryForKey(LS_PADDLE_SIZE)) {
      paddleSizeSelect_.setSelectedValue(LocalStore::integerForKey(LS_PADDLE_SIZE));
    } else {
      paddleSizeSelect_.setSelectedValue(psMedium);
    }
    addEntity(&paddleSizeSelect_);
  }
  
  addEntity(&soundSlider_);
}

void MainMenuState::stateIsShown() {
  if (IS_FREE) {
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
      [getGameEngine().adEngine addAdAtPoint:CGPointMake(0, 0)];  
    } else {
      [getGameEngine().adEngine addAdAtPoint:CGPointMake(45, 40)];  
    }
  }
}

void MainMenuState::pressedStart() {
  BOOL isIPhone = UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone;

  LocalStore::setInteger(numPlayersSelect_.getSelectedValue(), LS_NUM_PLAYERS);
  LocalStore::setInteger(numPucksSelect_.getSelectedValue(), LS_NUM_PUCKS);
  LocalStore::setInteger(difficultySelect_.getSelectedValue(), LS_DIFFICULTY);
  if (!isIPhone) {
    LocalStore::setInteger(paddleSizeSelect_.getSelectedValue(), LS_PADDLE_SIZE);
  }

  NSMutableDictionary *flurryData = [[[NSMutableDictionary alloc] initWithCapacity:4] autorelease];
  [flurryData setObject:[NSNumber numberWithInt:numPlayersSelect_.getSelectedValue() + 1]
                 forKey:@"NumPlayers"];
  [flurryData setObject:[NSNumber numberWithInt:numPucksSelect_.getSelectedValue() + 1]
                 forKey:@"NumPucks"];
  [flurryData setObject:[NSNumber numberWithInt:difficultySelect_.getSelectedValue()]
                 forKey:@"Difficulty"];
  if (!isIPhone) {
    [flurryData setObject:[NSNumber numberWithInt:paddleSizeSelect_.getSelectedValue()]
                   forKey:@"PaddleSize"];
  }
  [FlurryAnalytics logEvent:@"START_GAME" withParameters:flurryData];

  PaddleSize paddleSize = PaddleSize(isIPhone ? psLarge : paddleSizeSelect_.getSelectedValue());
  PlayState *playState = new PlayState(getGameEngine(),
                                       numPlayersSelect_.getSelectedValue() + 1,
                                       numPucksSelect_.getSelectedValue() + 1,
                                       ComputerAI(difficultySelect_.getSelectedValue()),
                                       paddleSize);
  [getGameEngine() replaceTopState:playState];
}

void MainMenuState::pressedFeedback() {
  NSString *free;
  NSString *device;
  
  [FlurryAnalytics logEvent:@"FEEDBACK_PRESSED"];

  if (IS_FREE) {
    free = @"%20Free";
  } else {
    free = @"";
  }
  
  if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
    device = @"iPad";
  } else {
    device = @"iPhone/iPod";
  }

  NSString *url =
      [NSString stringWithFormat:@"mailto:feedback@sharkable.com?"
                                  "subject=Glide%%20Hockey%%20HD%@%%20feedback%%20(%@)",
                                     free, device];
  
  [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
}

void MainMenuState::pressedStory() {
  [FlurryAnalytics logEvent:@"STORY_PRESSED"];
  [getGameEngine().adEngine removeAd];
  [getGameEngine() pushState:new StoryState(getGameEngine())];
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

void MainMenuState::buttonPressed(Button *button) {
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
