//
//  EAGLViewController.m
//  AirHockeyFree
//
//  Created by Jonathan Sharkey on 10-05-12.
//  Copyright 2010 Sharkable. All rights reserved.
//

#import "EAGLViewController.h"
#import "EAGLView.h"

@implementation EAGLViewController

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
  CGRect screenSize = [[UIScreen mainScreen] bounds];
  self.view = [[[EAGLView alloc] initWithFrame:screenSize] autorelease];
  ((EAGLView *)self.view).viewController = self;
}

// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
  // Return YES for supported orientations
  return (interfaceOrientation == UIInterfaceOrientationPortrait ||
          interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown);
}

- (void)didReceiveMemoryWarning {
  // Releases the view if it doesn't have a superview.
  [super didReceiveMemoryWarning];
  
  // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
  [super viewDidUnload];
  // Release any retained subviews of the main view.
  // e.g. self.myOutlet = nil;
}


- (void)dealloc {
  [super dealloc];
}

- (void) startAnimation {
  [(EAGLView *)self.view startAnimation];
}

- (void) stopAnimation {
  [(EAGLView *)self.view stopAnimation];
}

#pragma mark - Accessors

- (GameEngine *)gameEngine {
  return [(EAGLView *)self.view gameEngine];
}

- (void)setGameEngine:(GameEngine *)gameEngine {
  return [(EAGLView *)self.view setGameEngine:gameEngine];
}

@end
