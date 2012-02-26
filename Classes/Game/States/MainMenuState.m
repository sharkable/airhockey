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
#import "GameEngine.h"
#import "EAGLView.h"
#import "LocalStore.h"
#import "ResourceLoader.h"
#import "const.h"
#import "FlurryAnalytics.h"

@implementation MainMenuState

- (id)init {
  [super init];
  
  if (self) {
    BOOL isIPhone = UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone;
    
    Texture2D *backgroundTexture = [[ResourceLoader instance] getTextureWithName:@"rink_bg"];
    SimpleItem *background = [[[SimpleItem alloc] initWithTexture:backgroundTexture
                                                         position:CGPointMake(0, 0)] autorelease];
    [self addEntity:background];

    // Add rink left and right pieces.
    Texture2D *leftRinkBorderTexture = [[ResourceLoader instance] getTextureWithName:@"rink_left"];
    SimpleItem *leftRinkBorder = [[[SimpleItem alloc] initWithTexture:leftRinkBorderTexture
                                                             position:CGPointMake(0, 0)]
                                  autorelease];
    [self addEntity:leftRinkBorder];
    Texture2D* rightRinkBorderTexture =
        [[ResourceLoader instance] getTextureWithName:@"rink_right"];
    CGPoint leftRinkBorderPos = CGPointMake(SCREEN_WIDTH - rightRinkBorderTexture.contentSize.width,
                                            0);
    SimpleItem *rightRinkBorder = [[[SimpleItem alloc] initWithTexture:rightRinkBorderTexture
                                                              position:leftRinkBorderPos]
                                   autorelease];
    [self addEntity:rightRinkBorder];

    Texture2D *titleTexture = [[ResourceLoader instance] getTextureWithName:@"title"];
    SimpleItem *title = [[[SimpleItem alloc] initWithTexture:titleTexture
                                                    position:CGPointMake(81, 53)] autorelease];
    [self addEntity:title];  
    
    Texture2D* mainMenuTexture = [[ResourceLoader instance] getTextureWithName:@"main_menu"];
    CGPoint mainMenuPosition = CGPointMake((SCREEN_WIDTH - mainMenuTexture.contentSize.width) / 2,
                                           339);
    SimpleItem* mainMenu = [[[SimpleItem alloc] initWithTexture:mainMenuTexture
                                                       position:mainMenuPosition] autorelease];
    [self addEntity:mainMenu];  
    
    Texture2D* startButtonImage = [[ResourceLoader instance] getTextureWithName:@"start_button"];
    Texture2D* startButtonPressedImage =
        [[ResourceLoader instance] getTextureWithName:@"start_button_pressed"];
    CGPoint startButtonPosition =
        CGPointMake((SCREEN_WIDTH - startButtonImage.contentSize.width) / 2, 392);
    startButton_ = [[Button alloc] initWithNormalTexture:startButtonImage
                        pressedTexture:startButtonPressedImage
                          position:startButtonPosition];
    startButton_.delegate = self;
    startButton_.selector = @selector(pressedStart);
    [self addEntity:startButton_];
    
    Texture2D *feedbackButtonImage =
        [[ResourceLoader instance] getTextureWithName:@"feedback_button"];
    Texture2D *feedbackButtonPressedImage =
        [[ResourceLoader instance] getTextureWithName:@"feedback_button_pressed"];
    feedbackButton_ = [[Button alloc] initWithNormalTexture:feedbackButtonImage
                         pressedTexture:feedbackButtonPressedImage
                             position: isIPhone ? CGPointMake(440, 926) : CGPointMake(486, 936)];
    feedbackButton_.delegate = self;
    feedbackButton_.selector = @selector(pressedFeedback);
    [self addEntity:feedbackButton_];  

    Texture2D *storyButtonImage = [[ResourceLoader instance] getTextureWithName:@"story_button"];
    Texture2D *storyButtonPressedImage =
        [[ResourceLoader instance] getTextureWithName:@"story_button_pressed"];
    storyButton_ = [[Button alloc] initWithNormalTexture:storyButtonImage
                        pressedTexture:storyButtonPressedImage
                            position: isIPhone ? CGPointMake(86, 926) : CGPointMake(91, 936)];
    storyButton_.delegate = self;
    storyButton_.selector = @selector(pressedStory);
    [self addEntity:storyButton_];    
    
    if (NO) {
      Texture2D *upgradeButtonImage =
          [[ResourceLoader instance] getTextureWithName:@"upgrade_button"];
      Texture2D *upgradeButtonPressedImage =
          [[ResourceLoader instance] getTextureWithName:@"upgrade_button_pressed"];
      upgradeButton_ = [[Button alloc] initWithNormalTexture:upgradeButtonImage
                          pressedTexture:upgradeButtonPressedImage
                              position:CGPointMake(91, 936)];
      upgradeButton_.delegate = self;
      upgradeButton_.selector = @selector(pressedUpgrade);
      [self addEntity:upgradeButton_];    
    }
    
    numPlayersSelect_ = [[MultiSelect alloc] init];
    double playersY = isIPhone ? 570 : 511;
    Texture2D *onePlayerImage = [[ResourceLoader instance] getTextureWithName:@"1_player"];
    Texture2D *twoPlayerImage = [[ResourceLoader instance] getTextureWithName:@"2_player"];
    Texture2D *onePlayerSelectedImage =
        [[ResourceLoader instance] getTextureWithName:@"1_player_selected"];
    Texture2D *twoPlayerSelectedImage =
        [[ResourceLoader instance] getTextureWithName:@"2_player_selected"];
    CGPoint numPlayersSelectPosition =
        CGPointMake(SCREEN_WIDTH / 2 - onePlayerImage.contentSize.width, playersY);
    [numPlayersSelect_ addValueWithNormalTexture:onePlayerImage
                                 selectedTexture:onePlayerSelectedImage
                                        position:numPlayersSelectPosition];
    [numPlayersSelect_ addValueWithNormalTexture:twoPlayerImage
                                 selectedTexture:twoPlayerSelectedImage
                                        position:CGPointMake(SCREEN_WIDTH/2, playersY)];
    numPlayersSelect_.selectedValue = [LocalStore integerForKey:LS_NUM_PLAYERS];
    [self addEntity:numPlayersSelect_];
    
    double pucksY = isIPhone ? 706 : 627;
    double pucks1X = isIPhone ? 40 : 138.5;
    double pucks2X = isIPhone ? 45 : 142.5;
    double pucksXSpread = isIPhone ? 96 : 69;
    numPucksSelect_ = [[MultiSelect alloc] init];
    for (int i = 1; i <= (NO ? 4 : MAX_NUM_PUCKS); i++) {
      Texture2D *numPucksImage = [[ResourceLoader instance]
                                  getTextureWithName:[NSString stringWithFormat:@"%i", i]];
      Texture2D *numPucksSelectedImage =
          [[ResourceLoader instance]
           getTextureWithName:[NSString stringWithFormat:@"%i_selected", i]];
      CGPoint numPucksSelectPosition =
          CGPointMake(i == 1 ? pucks1X : pucks2X + pucksXSpread * (i - 1), pucksY);
      [numPucksSelect_ addValueWithNormalTexture:numPucksImage
                                 selectedTexture:numPucksSelectedImage
                                        position:numPucksSelectPosition];
    }
    numPucksSelect_.selectedValue = [LocalStore integerForKey:LS_NUM_PUCKS];
    [self addEntity:numPucksSelect_];
    
    if (NO) {
      Texture2D *upgradeForMoreTexture =
          [[ResourceLoader instance] getTextureWithName:@"upgrade_for_more"];
      CGPoint upgradeForMorePosition = CGPointMake(pucks2X + pucksXSpread * 4, pucksY);
      SimpleItem *upgradeForMore = [[[SimpleItem alloc] initWithTexture:upgradeForMoreTexture
                                                               position:upgradeForMorePosition]
                                    autorelease];
      [self addEntity:upgradeForMore];
    }
    
    
    double difficultyY = isIPhone ? 839 : 734;
    difficultySelect_ = [[MultiSelect alloc] init];
    Texture2D *badImage = [[ResourceLoader instance] getTextureWithName:@"bad"];
    Texture2D *badImageSelected = [[ResourceLoader instance] getTextureWithName:@"bad_selected"];
    Texture2D *goodImage = [[ResourceLoader instance] getTextureWithName:@"good"];
    Texture2D *goodImageSelected = [[ResourceLoader instance] getTextureWithName:@"good_selected"];
    Texture2D *excellentImage = [[ResourceLoader instance] getTextureWithName:@"excellent"];
    Texture2D *excellentImageSelected =
        [[ResourceLoader instance] getTextureWithName:@"excellent_selected"];
    Texture2D *amazingImage = [[ResourceLoader instance] getTextureWithName:@"amazing"];
    Texture2D *amazingImageSelected =
        [[ResourceLoader instance] getTextureWithName:@"amazing_selected"];
    [difficultySelect_ addValueWithNormalTexture:badImage
                                 selectedTexture:badImageSelected
                                        position:CGPointMake(isIPhone ? 38 : 138, difficultyY)];
    [difficultySelect_ addValueWithNormalTexture:goodImage
                                 selectedTexture:goodImageSelected
                                        position:CGPointMake(isIPhone ? 216 : 266, difficultyY)];
    [difficultySelect_ addValueWithNormalTexture:excellentImage
                                 selectedTexture:excellentImageSelected
                                        position:CGPointMake(isIPhone ? 380 : 384, difficultyY)];
    [difficultySelect_ addValueWithNormalTexture:amazingImage
                                 selectedTexture:amazingImageSelected
                                        position:CGPointMake(isIPhone ? 543 : 502, difficultyY)];
    if ([LocalStore hasEntryForKey:LS_DIFFICULTY]) {
      difficultySelect_.selectedValue = [LocalStore integerForKey:LS_DIFFICULTY];
    } else {
      difficultySelect_.selectedValue = caiGood;
    }
    [self addEntity:difficultySelect_];
    
    if (!isIPhone) {
      paddleSizeSelect_ = [[MultiSelect alloc] init];
      Texture2D* smallImage = [[ResourceLoader instance] getTextureWithName:@"small"];
      Texture2D* smallImageSelected =
          [[ResourceLoader instance] getTextureWithName:@"small_selected"];
      Texture2D* mediumImage = [[ResourceLoader instance] getTextureWithName:@"medium"];
      Texture2D* mediumImageSelected =
          [[ResourceLoader instance] getTextureWithName:@"medium_selected"];
      Texture2D* largeImage = [[ResourceLoader instance] getTextureWithName:@"large"];
      Texture2D* largeImageSelected =
          [[ResourceLoader instance] getTextureWithName:@"large_selected"];
      [paddleSizeSelect_ addValueWithNormalTexture:smallImage
                                   selectedTexture:smallImageSelected
                                          position:CGPointMake(139, 842)];
      [paddleSizeSelect_ addValueWithNormalTexture:mediumImage
                                   selectedTexture:mediumImageSelected
                                          position:CGPointMake(305, 842)];
      [paddleSizeSelect_ addValueWithNormalTexture:largeImage
                                   selectedTexture:largeImageSelected
                                          position:CGPointMake(464, 842)];
      if ([LocalStore hasEntryForKey:LS_PADDLE_SIZE]) {
        paddleSizeSelect_.selectedValue = [LocalStore integerForKey:LS_PADDLE_SIZE];
      } else {
        paddleSizeSelect_.selectedValue = psMedium;
      }
      [self addEntity:paddleSizeSelect_];
    }
    
    soundSlider_ = [[SoundSlider alloc] initWithPosition:CGPointMake(400, 50)];
    [self addEntity:soundSlider_];
  }
  
  return self;
}

- (void)dealloc {
  [startButton_ release];
  [feedbackButton_ release];
  [storyButton_ release];
  [upgradeButton_ release];
  [numPlayersSelect_ release];
  [numPucksSelect_ release];
  [difficultySelect_ release];
  [paddleSizeSelect_ release];
  [soundSlider_ release];
  
  [super dealloc];
}

- (void)stateIsShown {
  if (IS_FREE) {
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
      [EAGLView addAdAtPoint:CGPointMake(0, 0)];  
    } else {
      [EAGLView addAdAtPoint:CGPointMake(45, 40)];  
    }
  }
}

- (void)pressedStart {
  BOOL isIPhone = UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone;

  [LocalStore setInteger:numPlayersSelect_.selectedValue forKey:LS_NUM_PLAYERS];
  [LocalStore setInteger:numPucksSelect_.selectedValue forKey:LS_NUM_PUCKS];
  [LocalStore setInteger:difficultySelect_.selectedValue forKey:LS_DIFFICULTY];
  if (!isIPhone) {
    [LocalStore setInteger:paddleSizeSelect_.selectedValue forKey:LS_PADDLE_SIZE];
  }

  NSMutableDictionary *flurryData = [[[NSMutableDictionary alloc] initWithCapacity:4] autorelease];
  [flurryData setObject:[NSNumber numberWithInt:[numPlayersSelect_ selectedValue] + 1]
                 forKey:@"NumPlayers"];
  [flurryData setObject:[NSNumber numberWithInt:[numPucksSelect_ selectedValue] + 1]
                 forKey:@"NumPucks"];
  [flurryData setObject:[NSNumber numberWithInt:[difficultySelect_ selectedValue]]
                 forKey:@"Difficulty"];
  if (!isIPhone) {
    [flurryData setObject:[NSNumber numberWithInt:[paddleSizeSelect_ selectedValue]]
                   forKey:@"PaddleSize"];
  }
  [FlurryAnalytics logEvent:@"START_GAME" withParameters:flurryData];

  PaddleSize paddleSize = isIPhone ? psLarge : [paddleSizeSelect_ selectedValue];
  PlayState *playState =
      [[[PlayState alloc] initWithNumPlayers:[numPlayersSelect_ selectedValue] + 1
                                    numPucks:[numPucksSelect_ selectedValue] + 1
                                  difficulty:[difficultySelect_ selectedValue]
                                  paddleSize:paddleSize] autorelease];
  [self.gameEngine replaceTopState:playState];
}

- (void)pressedFeedback {
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

- (void)pressedStory {
  [FlurryAnalytics logEvent:@"STORY_PRESSED"];
  [EAGLView removeAd];
  [self.gameEngine pushState:[[[StoryState alloc] init] autorelease]];
}

- (void)pressedUpgrade {
  UIAlertView *alert = [[[UIAlertView alloc] initWithTitle:@"Upgrade"
                                                   message:@"You can remove ads and have up to "
                                                            "seven pucks!\nJust upgrade to the paid"
                                                            " version.\nIt's cheap!"
                                                  delegate:self
                                         cancelButtonTitle:@"No thanks!"
                                         otherButtonTitles:@"OK!", nil] autorelease];
  [alert show];
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
  if (buttonIndex == 1) {
    NSString * url = @"http://itunes.com/app/GlideHockeyHD";
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
  }
}

@end
