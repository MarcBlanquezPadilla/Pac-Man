#pragma once
//Displaying scale factor
#define GAME_SCALE_FACTOR	1.0f

//Function return values
enum class AppStatus { OK = 0, ERROR, QUIT };
enum class Directions { NONE = -1, RIGHT, LEFT, UP, DOWN };

//Main return values
#define EXIT_SUCCESS		0
#define EXIT_FAILURE		1

//Window size
#define WINDOW_WIDTH		640
#define WINDOW_HEIGHT		836

//Game constants
#define TILE_SIZE			8
#define LEVEL_WIDTH			68
#define LEVEL_HEIGHT		62
#define MARGIN_GUI_X		(WINDOW_WIDTH/2)-(LEVEL_WIDTH*TILE_SIZE)/2
#define MARGIN_GUI_Y		(WINDOW_HEIGHT/2)-(LEVEL_HEIGHT*TILE_SIZE)/2 + 80 //(EXTRA DISPLAÇMENT)
#define PELETS_TO_WIN		244
#define TIME_IN_SCATTER		7
#define TIME_IN_CHASE		10
#define TIME_IN_FRIGHTENED	10
#define SPAWN_POS_Y			184
#define RIGHT_SPAWN_POS_X	280
#define LEFT_SPAWN_POS_X	264

//Teleports
#define RIGHT_TP_POS_X		496
#define RIGHT_TP_POS_Y		216
#define LEFT_TP_POS_X		16	
#define LEFT_TP_POS_Y		216

//Navigation Mesh
#define PATH_SIZE			16
#define MAX_ITERATIONS		200

//Entities animation delay
#define ANIM_DELAY			4

