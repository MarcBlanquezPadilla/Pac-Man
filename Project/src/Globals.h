#pragma once
//Displaying scale factor
#define GAME_SCALE_FACTOR	1.0f

//Function return values
enum class AppStatus { OK = 0, ERROR, QUIT };

//Main return values
#define EXIT_SUCCESS	0
#define EXIT_FAILURE	1

//Window size
#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   836

//Game constants
#define TILE_SIZE		8
#define LEVEL_WIDTH		68
#define LEVEL_HEIGHT	62
#define MARGIN_GUI_X	(WINDOW_WIDTH/2)-(LEVEL_WIDTH*TILE_SIZE)/2
#define MARGIN_GUI_Y	(WINDOW_HEIGHT/2)-(LEVEL_HEIGHT*TILE_SIZE)/2 + 80 //(EXTRA DISPLA�MENT)

//Teleports
#define RIGHT_TP_POS_X	496
#define RIGHT_TP_POS_Y	216
#define LEFT_TP_POS_X	16	
#define LEFT_TP_POS_Y	216

//Entities animation delay
#define ANIM_DELAY		4

