//
//  const.h
//  AirHockey
//
//  Created by Jon Sharkey on 2010-04-26.
//  Copyright 2010 Sharkable. All rights reserved.
//

#ifndef AIRHOCKEY_CONST_H_
#define AIRHOCKEY_CONST_H_

#define SCREEN_WIDTH  768
#define SCREEN_HEIGHT 1024

#define AD_WIDTH  320
#define AD_HEIGHT 48

#define PLAYER_1 0
#define PLAYER_2 1

#define MAX_NUM_PUCKS 7
#define WIN_SCORE 7

#define RINK_EDGE_WIDTH 23
#define RINK_LEFT_X RINK_EDGE_WIDTH
#define RINK_RIGHT_X (SCREEN_WIDTH - RINK_EDGE_WIDTH)
#define RINK_TOP_Y RINK_EDGE_WIDTH
#define RINK_BOTTOM_Y (SCREEN_HEIGHT - RINK_EDGE_WIDTH)
#define GOAL_LEFT_X 200
#define GOAL_RIGHT_X 568

#define PADDLE_2_X (SCREEN_WIDTH / 2)
#define PADDLE_2_Y 130
#define PADDLE_1_X PADDLE_2_X
#define PADDLE_1_Y (SCREEN_HEIGHT - PADDLE_2_Y - 1)
#define PADDLE_MASS 100
#define PADDLE_FRICTION 0.1
#define PADDLE_AI_FRICTION 0.999
extern double PADDLE_RADIUS[];

#define PLAYER_2_PUCK_Y (SCREEN_HEIGHT / 3)
#define PLAYER_1_PUCK_Y (SCREEN_HEIGHT - PLAYER_2_PUCK_Y - 1)            
#define PUCK_X_SEPARATION 150
#define PUCK_MASS 10
#define PUCK_FRICTION 0.995
#define PUCK_RADIUS 43.5

#define PUCK_GOAL_MIN_DROP_SPEED 5

#define MAX_SPEED 60

#define PUCK_FADE_TICKS 10

#define WAIT_TICKS 60

#define GET_READY_TICKS_TOTAL 120
#define SHOW_GET_READY_MESSAGE_TICKS 90
#define SHOW_GO_MESSAGE_TICKS 30

typedef enum {
  caiBad = 0,
  caiGood,
  caiExcellent,
  caiAmazing,
} ComputerAI;

typedef enum {
  psSmall = 0,
  psMedium,
  psLarge,
} PaddleSize;

// Locale Store keys
#define LS_NUM_PLAYERS "ls_num_players"
#define LS_DIFFICULTY "ls_difficulty"
#define LS_NUM_PUCKS "ls_num_pucks"
#define LS_PADDLE_SIZE "ls_paddle_size"
#define LS_VOLUME "ls_volume"


#if (__IPHONE_OS_VERSION_MAX_ALLOWED < 30200)

#define UI_USER_INTERFACE_IDIOM() 0
#define UIUserInterfaceIdiomPhone 0
#define UIUserInterfaceIdiomPad   1

#endif

#endif
