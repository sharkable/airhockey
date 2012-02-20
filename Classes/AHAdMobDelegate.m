//
//  AHAdMobDelegate.m
//  AirHockey
//
//  Created by Jonathan Sharkey on 10-05-02.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "AHAdMobDelegate.h"


@implementation AHAdMobDelegate

- (NSString *)publisherId {
	return @"a14bdda6dfc895a";
}

- (UIViewController *)currentViewController {
	return nil;
}



- (void)didReceiveAd:(AdMobView *)adView {
	NSLog(@"ADMOD RECEIVED AD");
}

- (void)didFailToReceiveAd:(AdMobView *)adView {
	NSLog(@"FAILED TO RECEIVE AD");
}

@end
