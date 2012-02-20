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
#import "GameEngine.h"
#import "EAGLView.h"
#import "LocalStore.h"
#import "ResourceLoader.h"
#import "const.h"
#import "IsFree.h"
#import "FlurryAPI.h"

@implementation MainMenuState

- (id) init {
	[super init];
	
	BOOL isIPhone = UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone;
	
	Texture2D* backgroundTexture = [[ResourceLoader instance] getTextureWithName:@"rink_bg"];
	SimpleItem* background = [[[SimpleItem alloc] initWithTexture:backgroundTexture
														 position:CGPointMake(0, 0)] autorelease];
	[self addEntity:background];

	// Add rink left and right pieces.
	Texture2D* leftRinkBorderTexture = [[ResourceLoader instance] getTextureWithName:@"rink_left"];
	SimpleItem* leftRinkBorder = [[[SimpleItem alloc] initWithTexture:leftRinkBorderTexture
															 position:CGPointMake(0, 0)] autorelease];
	[self addEntity:leftRinkBorder];
	Texture2D* rightRinkBorderTexture = [[ResourceLoader instance] getTextureWithName:@"rink_right"];
	CGPoint leftRinkBorderPos = CGPointMake(SCREEN_WIDTH - rightRinkBorderTexture.contentSize.width, 0);
	SimpleItem*  rightRinkBorder = [[[SimpleItem alloc] initWithTexture:rightRinkBorderTexture
															   position:leftRinkBorderPos] autorelease];
	[self addEntity:rightRinkBorder];

	Texture2D* titleTexture = [[ResourceLoader instance] getTextureWithName:@"title"];
	SimpleItem* title = [[[SimpleItem alloc] initWithTexture:titleTexture
													position:CGPointMake(81, 53)] autorelease];
	[self addEntity:title];	
	
	Texture2D* mainMenuTexture = [[ResourceLoader instance] getTextureWithName:@"main_menu"];
	SimpleItem* mainMenu = [[[SimpleItem alloc] initWithTexture:mainMenuTexture
													   position:CGPointMake((SCREEN_WIDTH - mainMenuTexture.contentSize.width)/2, 339)] autorelease];
	[self addEntity:mainMenu];	
	
	Texture2D* startButtonImage = [[ResourceLoader instance] getTextureWithName:@"start_button"];
	Texture2D* startButtonPressedImage = [[ResourceLoader instance] getTextureWithName:@"start_button_pressed"];
	_startButton = [[Button alloc] initWithNormalTexture:startButtonImage
										  pressedTexture:startButtonPressedImage
												position:CGPointMake((SCREEN_WIDTH - startButtonImage.contentSize.width)/2, 392)];
	_startButton.delegate = self;
	_startButton.selector = @selector(pressedStart);
	[self addEntity:_startButton];
	
	Texture2D* feedbackButtonImage = [[ResourceLoader instance] getTextureWithName:@"feedback_button"];
	Texture2D* feedbackButtonPressedImage = [[ResourceLoader instance] getTextureWithName:@"feedback_button_pressed"];
	_feedbackButton = [[Button alloc] initWithNormalTexture:feedbackButtonImage
											 pressedTexture:feedbackButtonPressedImage
												   position:CGPointMake(486, 936)];
	_feedbackButton.delegate = self;
	_feedbackButton.selector = @selector(pressedFeedback);
	[self addEntity:_feedbackButton];	

	if (NO) {
		Texture2D* upgradeButtonImage = [[ResourceLoader instance] getTextureWithName:@"upgrade_button"];
		Texture2D* upgradeButtonPressedImage = [[ResourceLoader instance] getTextureWithName:@"upgrade_button_pressed"];
		_upgradeButton = [[Button alloc] initWithNormalTexture:upgradeButtonImage
												pressedTexture:upgradeButtonPressedImage
													  position:CGPointMake(91, 936)];
		_upgradeButton.delegate = self;
		_upgradeButton.selector = @selector(pressedUpgrade);
		[self addEntity:_upgradeButton];		
	}
	
	_numPlayersSelect = [[MultiSelect alloc] init];
	double playersY = isIPhone ? 570 : 511;
	Texture2D* onePlayerImage = [[ResourceLoader instance] getTextureWithName:@"1_player"];
	Texture2D* twoPlayerImage = [[ResourceLoader instance] getTextureWithName:@"2_player"];
	Texture2D* onePlayerSelectedImage = [[ResourceLoader instance] getTextureWithName:@"1_player_selected"];
	Texture2D* twoPlayerSelectedImage = [[ResourceLoader instance] getTextureWithName:@"2_player_selected"];
	[_numPlayersSelect addValueWithNormalTexture:onePlayerImage
								 selectedTexture:onePlayerSelectedImage
										position:CGPointMake(SCREEN_WIDTH/2 - onePlayerImage.contentSize.width, playersY)];
	[_numPlayersSelect addValueWithNormalTexture:twoPlayerImage
								 selectedTexture:twoPlayerSelectedImage
										position:CGPointMake(SCREEN_WIDTH/2, playersY)];
	_numPlayersSelect.selectedValue = [LocalStore integerForKey:LS_NUM_PLAYERS];
	[self addEntity:_numPlayersSelect];
	
	double pucksY = isIPhone ? 706 : 627;
	double pucks1X = isIPhone ? 40 : 138.5;
	double pucks2X = isIPhone ? 45 : 142.5;
	double pucksXSpread = isIPhone ? 96 : 69;
	_numPucksSelect = [[MultiSelect alloc] init];
	for (int i = 1; i <= (NO ? 4 : MAX_NUM_PUCKS); i++) {
		Texture2D* numPucksImage = [[ResourceLoader instance] getTextureWithName:[NSString stringWithFormat:@"%i", i]];
		Texture2D* numPucksSelectedImage = [[ResourceLoader instance] getTextureWithName:[NSString stringWithFormat:@"%i_selected", i]];
		[_numPucksSelect addValueWithNormalTexture:numPucksImage
								   selectedTexture:numPucksSelectedImage
										  position:CGPointMake(i == 1 ? pucks1X : pucks2X + pucksXSpread*(i-1), pucksY)];
	}
	_numPucksSelect.selectedValue = [LocalStore integerForKey:LS_NUM_PUCKS];
	[self addEntity:_numPucksSelect];
	
	if (NO) {
		Texture2D* upgradeForMoreTexture = [[ResourceLoader instance] getTextureWithName:@"upgrade_for_more"];
		SimpleItem* upgradeForMore = [[[SimpleItem alloc] initWithTexture:upgradeForMoreTexture
																 position:CGPointMake(pucks2X + pucksXSpread*4, pucksY)] autorelease];
		[self addEntity:upgradeForMore];
	}
	
	
	double difficultyY = isIPhone ? 839 : 734;
	_difficultySelect = [[MultiSelect alloc] init];
	Texture2D* badImage               = [[ResourceLoader instance] getTextureWithName:@"bad"];
	Texture2D* badImageSelected       = [[ResourceLoader instance] getTextureWithName:@"bad_selected"];
	Texture2D* goodImage              = [[ResourceLoader instance] getTextureWithName:@"good"];
	Texture2D* goodImageSelected      = [[ResourceLoader instance] getTextureWithName:@"good_selected"];
	Texture2D* excellentImage         = [[ResourceLoader instance] getTextureWithName:@"excellent"];
	Texture2D* excellentImageSelected = [[ResourceLoader instance] getTextureWithName:@"excellent_selected"];
	Texture2D* amazingImage           = [[ResourceLoader instance] getTextureWithName:@"amazing"];
	Texture2D* amazingImageSelected   = [[ResourceLoader instance] getTextureWithName:@"amazing_selected"];
	[_difficultySelect addValueWithNormalTexture:badImage
								 selectedTexture:badImageSelected
										position:CGPointMake(isIPhone ? 38 : 138, difficultyY)];
	[_difficultySelect addValueWithNormalTexture:goodImage
								 selectedTexture:goodImageSelected
										position:CGPointMake(isIPhone ? 216 : 266, difficultyY)];
	[_difficultySelect addValueWithNormalTexture:excellentImage
								 selectedTexture:excellentImageSelected
										position:CGPointMake(isIPhone ? 380 : 384, difficultyY)];
	[_difficultySelect addValueWithNormalTexture:amazingImage
								 selectedTexture:amazingImageSelected
										position:CGPointMake(isIPhone ? 543 : 502, difficultyY)];
	if ([LocalStore hasEntryForKey:LS_DIFFICULTY]) {
		_difficultySelect.selectedValue = [LocalStore integerForKey:LS_DIFFICULTY];
	} else {
		_difficultySelect.selectedValue = caiGood;
	}
	[self addEntity:_difficultySelect];
	
	if (!isIPhone) {
		_paddleSizeSelect = [[MultiSelect alloc] init];
		Texture2D* smallImage          = [[ResourceLoader instance] getTextureWithName:@"small"];
		Texture2D* smallImageSelected  = [[ResourceLoader instance] getTextureWithName:@"small_selected"];
		Texture2D* mediumImage         = [[ResourceLoader instance] getTextureWithName:@"medium"];
		Texture2D* mediumImageSelected = [[ResourceLoader instance] getTextureWithName:@"medium_selected"];
		Texture2D* largeImage          = [[ResourceLoader instance] getTextureWithName:@"large"];
		Texture2D* largeImageSelected  = [[ResourceLoader instance] getTextureWithName:@"large_selected"];
		[_paddleSizeSelect addValueWithNormalTexture:smallImage
								   selectedTexture:smallImageSelected
										  position:CGPointMake(139, 842)];
		[_paddleSizeSelect addValueWithNormalTexture:mediumImage
								   selectedTexture:mediumImageSelected
										  position:CGPointMake(305, 842)];
		[_paddleSizeSelect addValueWithNormalTexture:largeImage
								   selectedTexture:largeImageSelected
										  position:CGPointMake(464, 842)];
		if ([LocalStore hasEntryForKey:LS_PADDLE_SIZE]) {
			_paddleSizeSelect.selectedValue = [LocalStore integerForKey:LS_PADDLE_SIZE];
		} else {
			_paddleSizeSelect.selectedValue = psMedium;
		}
		[self addEntity:_paddleSizeSelect];
	}
	
	_soundSlider = [[SoundSlider alloc] initWithPosition:CGPointMake(400, 50)];
	[self addEntity:_soundSlider];
	
	return self;
}

- (void) dealloc {
	[_startButton      release];
	[_feedbackButton   release];
	[_upgradeButton    release];
	[_numPlayersSelect release];
	[_numPucksSelect   release];
	[_difficultySelect release];
	[_paddleSizeSelect release];
	[_soundSlider      release];
	
	[super dealloc];
	
}

- (void) stateIsShown {
	if (IS_FREE) {
		[EAGLView addAdAtPoint:CGPointMake(0, 0)];	
	}
}

- (void) pressedStart {
	BOOL isIPhone = UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone;

	[LocalStore setInteger:_numPlayersSelect.selectedValue forKey:LS_NUM_PLAYERS];
	[LocalStore setInteger:_numPucksSelect.selectedValue   forKey:LS_NUM_PUCKS];
	[LocalStore setInteger:_difficultySelect.selectedValue forKey:LS_DIFFICULTY];
	if (!isIPhone) {
		[LocalStore setInteger:_paddleSizeSelect.selectedValue forKey:LS_PADDLE_SIZE];
	}

	NSMutableDictionary* flurryData = [[[NSMutableDictionary alloc] initWithCapacity:4] autorelease];
	[flurryData setObject:[NSNumber numberWithInt:[_numPlayersSelect selectedValue] + 1] forKey:@"NumPlayers"];
	[flurryData setObject:[NSNumber numberWithInt:[_numPucksSelect selectedValue] + 1] forKey:@"NumPucks"];
	[flurryData setObject:[NSNumber numberWithInt:[_difficultySelect selectedValue]] forKey:@"Difficulty"];
	if (!isIPhone) {
		[flurryData setObject:[NSNumber numberWithInt:[_paddleSizeSelect selectedValue]] forKey:@"PaddleSize"];
	}
	[FlurryAPI logEvent:@"START_GAME" withParameters:flurryData];

	[[GameEngine instance] replaceTopState:[[[PlayState alloc] initWithNumPlayers:[_numPlayersSelect selectedValue] + 1
																		 numPucks:[_numPucksSelect selectedValue] + 1
																	   difficulty:[_difficultySelect selectedValue]
																	   paddleSize:isIPhone ? psLarge : [_paddleSizeSelect selectedValue]] autorelease]];
}

- (void) pressedFeedback {
	NSString* free;
	NSString* device;
	
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

	NSString* url = [NSString stringWithFormat:@"mailto:feedback@sharkable.com?subject=Glide%%20Hockey%%20HD%@%%20feedback%%20(%@)", free, device];
	
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
}

- (void) pressedUpgrade {
	UIAlertView* alert = [[[UIAlertView alloc] initWithTitle:@"Upgrade"
													 message:@"You can remove ads and have up to seven pucks!\nJust upgrade to the paid version.\nIt's cheap!"
													delegate:self
										   cancelButtonTitle:@"No thanks!"
										   otherButtonTitles:@"OK!", nil] autorelease];
	[alert show];
}

- (void) alertView:(UIAlertView*)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
	if (buttonIndex == 1) {
		NSString* url =  @"http://itunes.com/app/GlideHockeyHD";
		[[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
	}
}

@end
