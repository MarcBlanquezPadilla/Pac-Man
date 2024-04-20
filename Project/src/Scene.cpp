#include "Scene.h"
#include <stdio.h>
#include "Globals.h"
#include "LogMessages.h"

Scene::Scene()
{
	returnMainMenu = false; //PROVISIONAL
	player = nullptr;
	blinky = nullptr;
	pinky = nullptr;
	inky = nullptr;
	clyde = nullptr;
	level = nullptr;

	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { MARGIN_GUI_X, MARGIN_GUI_Y };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom

	peletsCollected = 0;

	debug = DebugMode::OFF;
}
Scene::~Scene()
{
	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
	if (blinky != nullptr)
	{
		blinky->Release();
		delete blinky;
		blinky = nullptr;
	}
	if (pinky != nullptr)
	{
		pinky->Release();
		delete pinky;
		pinky = nullptr;
	}
	if (inky != nullptr)
	{
		inky->Release();
		delete inky;
		inky = nullptr;
	}
	if (clyde != nullptr)
	{
		clyde->Release();
		delete clyde;
		clyde = nullptr;
	}
	if (level != nullptr)
	{
		level->Release();
		delete level;
		level = nullptr;
	}
	for (Entity* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}
AppStatus Scene::Init()
{
	//Create player
	player = new Player({ 0, 0 }, PlayerState::IDLE, PlayerLook::RIGHT);
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::ERROR;
	}
	//Initialise player
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}

	//Create blinky
	blinky = new Blinky({ 0, 0 }, GhostState::WALKING, GhostLook::RIGHT);
	if (blinky == nullptr)
	{
		LOG("Failed to allocate memory for blinky");
		return AppStatus::ERROR;
	}
	//Initialise blinky
	if (blinky->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise blinky");
		return AppStatus::ERROR;
	}
	//Create pinky
	pinky = new Pinky({ 0, 0 }, GhostState::WALKING, GhostLook::RIGHT);
	if (pinky == nullptr)
	{
		LOG("Failed to allocate memory for pinky");
		return AppStatus::ERROR;
	}
	//Initialise pinky
	if (pinky->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise pinky");
		return AppStatus::ERROR;
	}
	inky = new Inky({ 0, 0 }, GhostState::WALKING, GhostLook::RIGHT);
	if (inky == nullptr)
	{
		LOG("Failed to allocate memory for inky");
		return AppStatus::ERROR;
	}
	//Initialise pinky
	if (inky->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise inky");
		return AppStatus::ERROR;
	}
	clyde = new Clyde({ 0, 0 }, GhostState::WALKING, GhostLook::RIGHT);
	if (clyde == nullptr)
	{
		LOG("Failed to allocate memory for clyde");
		return AppStatus::ERROR;
	}
	//Initialise pinky
	if (clyde->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise clyde");
		return AppStatus::ERROR;
	}

	//Create level 
	level = new TileMap();
	if (level == nullptr)
	{
		LOG("Failed to allocate memory for Level");
		return AppStatus::ERROR;
	}
	//Initialise level
	if (level->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	//Load level
	if (LoadLevel(1) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::ERROR;
	}

	//Assign the tile map reference to the player to check collisions while navigating
	player->SetTileMap(level);
	blinky->SetTileMap(level);
	pinky->SetTileMap(level);
	inky->SetTileMap(level);
	clyde->SetTileMap(level);

	return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int* map = nullptr;
	Object* obj;

	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if (stage == 1)
	{
		map = new int[size] {
			0,	0,	0,	0,	0,	0,	30,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	2,	2,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	31,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	101,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	102,0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	7,	2,	2,	2,	2,	6,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	7,	2,	2,	2,	2,	6,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	0,	5,	0,	0,	0,	0,	4,	0,	51,	0,	0,	5,	0,	0,	0,	0,	0,	0,	4,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	5,	0,	0,	0,	0,	0,	0,	4,	0,	51,	0,	0,	5,	0,	0,	0,	0,	4,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	5,	0,	0,	0,	0,	4,	0,	0,	0,	0,	5,	0,	0,	0,	0,	0,	0,	4,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	5,	0,	0,	0,	0,	0,	0,	4,	0,	0,	0,	0,	5,	0,	0,	0,	0,	4,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	0,	9,	3,	3,	3,	3,	8,	0,	51,	0,	0,	9,	3,	3,	3,	3,	3,	3,	8,	0,	51,	0,	0,	9,	8,	0,	51,	0,	0,	9,	3,	3,	3,	3,	3,	3,	8,	0,	51,	0,	0,	9,	3,	3,	3,	3,	8,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	7,	2,	2,	2,	2,	6,	0,	0,	0,	0,	7,	6,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	7,	6,	0,	0,	0,	0,	7,	2,	2,	2,	2,	6,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	0,	9,	3,	3,	3,	3,	8,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	9,	3,	3,	3,	3,	3,	0,	0,	3,	3,	3,	3,	3,	8,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	9,	3,	3,	3,	3,	8,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	32,	24,	24,	24,	24,	24,	24,	24,	24,	24,	31,	0,	0,	0,	0,	5,	0,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	0,	4,	0,	0,	0,	0,	30,	24,	24,	24,	24,	24,	24,	24,	24,	24,	32,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	51,	0,	0,	5,	0,	3,	3,	3,	3,	3,	8,	0,	0,	0,	0,	9,	8,	0,	0,	0,	0,	9,	3,	3,	3,	3,	3,	0,	4,	0,	51,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	51,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	51,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	51,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	51,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	23,	24,	24,	24,	35,	50,	50,	50,	50,	36,	24,	24,	24,	25,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			1,	1,	1,	1,	1,	1,	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 33,	0,	51,	0,	0,	9,	8,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	9,	8,	0,	51,	0,	0,	32,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	1,	1,	1,	1,	1,	1,
			1,	53,	-1,-1,	-1,	-1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	-1,-1,	-1,	-1, 54,	1,
			1,	-1,	-1,	-1,	-1,	-1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	-1,	-1,	-1,	-1, -1,	1,
			1,	-1,	-1,	-1,	-1,	-1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	-1,	-1,	-1,	-1, -1,	1,
			1,	-1,	-1,	-1,	-1,	-1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	-1,	-1,	-1,	-1, -1,	1,
			1,	1,	1,	1,	1,	1,	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 31,	0,	0,	0,	0,	7,	6,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	7,	6,	0,	0,	0,	0,	30,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	1,	1,	1,	1,	1,	1,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	51,	0,	0,	5,	4,	0,	0,	0,	0,	29,	28,	28,	28,	28,	28,	28,	28,	28,	28,	28,	28,	28,	27,	0,	0,	0,	0,	5,	4,	0,	51,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	100,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	51,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	51,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	51,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	51,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	30, 24, 24, 24, 24, 24, 24, 24, 24, 24, 33,	0,	51,	0,	0,	9,	8,	0,	0,	0,	0,	9,	3,	3,	3,	3,	3,	0,	0,	3,	3,	3,	3,	3,	8,	0,	0,	0,	0,	9,	8,	0,	51,	0,	0,	32,	24,	24,	24,	24,	24,	24,	24,	24,	24,	31,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	7,	2,	2,	2,	2,	6,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	7,	2,	2,	2,	2,	6,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	0,	9,	3,	3,	3,	0,	4,	0,	51,	0,	0,	9,	3,	3,	3,	3,	3,	3,	8,	0,	51,	0,	0,	9,	8,	0,	51,	0,	0,	9,	3,	3,	3,	3,	3,	3,	8,	0,	51,	0,	0,	5,	0,	3,	3,	3,	8,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	5,	2,	2,	2,	6,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	7,	6,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	7,	6,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	7,	2,	2,	2,	4,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	5,	3,	3,	3,	8,	0,	51,	0,	0,	9,	8,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	9,	3,	3,	3,	3,	3,	0,	0,	3,	3,	3,	3,	3,	8,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	9,	8,	0,	51,	0,	0,	9,	3,	3,	3,	4,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	2,	2,	2,	2,	0,	0,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	0,	0,	2,	2,	2,	2,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	0,	9,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	8,	0,	51,	0,	0,	9,	8,	0,	51,	0,	0,	9,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	8,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	103,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	104,0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	32, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 33,	0,	0,	0,	0,	0,	0
		};
		player->InitScore();
	}
	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;
	}

	//Entities and objects
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)map[i];
			if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE;
				player->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::BLINKY)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE;
				blinky->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::PINKY)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE;
				pinky->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::INKY)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE;
				inky->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::CLYDE)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE;
				clyde->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::SMALL_PELET)
			{
				pos.x = x * TILE_SIZE + SMALL_OBJECT_SIZE / 2;
				pos.y = y * TILE_SIZE + SMALL_OBJECT_SIZE / 2;
				Point pHitBox;
				pHitBox.x = SMALL_OBJECT_SIZE / 4;
				pHitBox.y = SMALL_OBJECT_SIZE / 4;
				obj = new Object(pos, pHitBox, ObjectType::SMALL_PELET, SMALL_OBJECT_SIZE, SMALL_OBJECT_PHYSICAL_SIZE/2 );
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::TP_LEFT)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE;

				obj = new Object(pos, ObjectType::LEFT_TELEPORTER, TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE * 4);
				objects.push_back(obj);
				map[i] = -1;
			}
			else if (tile == Tile::TP_RIGHT)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE;

				obj = new Object(pos, ObjectType::RIGHT_TELEPORTER, TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE * 4);
				objects.push_back(obj);
				map[i] = -1;
			}
			++i;
		}
	}
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);

	delete map;

	return AppStatus::OK;
}
void Scene::Update()
{
	Point p1, p2;
	AABB box;

	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}
	//Debug levels instantly
	if (IsKeyPressed(KEY_ONE))		LoadLevel(1);
	else if (IsKeyPressed(KEY_TWO))	LoadLevel(2);

	level->Update();
	player->Update();
	blinky->Update();
	pinky->Update();
	inky->Update();
	clyde->Update();
	CheckCollisions();
}
void Scene::Render()
{
	BeginMode2D(camera);

	level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects();
		player->Draw();
		blinky->Draw();
		pinky->Draw();
		inky->Draw();
		clyde->Draw();
	}
	level->RenderEmptys();
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
		blinky->DrawDebug(RED);
		pinky->DrawDebug(RED);
		inky->DrawDebug(RED);
		clyde->DrawDebug(RED);
	}

	EndMode2D();

	RenderGUI();
}
void Scene::Release()
{
	level->Release();
	player->Release();
	blinky->Release();
	pinky->Release();
	inky->Release();
	clyde->Release();
	ClearLevel();
}
void Scene::CheckCollisions()
{
	AABB player_box, obj_box, blinky_box, pinky_box, inky_box, clyde_box;

	player_box = player->GetHitbox();
	
	auto it = objects.begin();
	while (it != objects.end())
	{
		obj_box = (*it)->GetHitbox();
		if (player_box.TestAABB(obj_box))
		{
			ObjectType type = (*it)->GetObjectType();
			if (type == ObjectType::SMALL_PELET)
			{
				player->IncrScore((*it)->Points());
				delete* it;
				it = objects.erase(it);
				peletsCollected++;
				if (peletsCollected == PELETS_TO_WIN)	returnMainMenu = true;
			}
			else if (type == ObjectType::LARGE_PELET)
			{

			}
			else if (type == ObjectType::RIGHT_TELEPORTER)
			{
				player->SetPos(Point({ LEFT_TP_POS_X, LEFT_TP_POS_Y }));
				it++;
			}
			else if (type == ObjectType::LEFT_TELEPORTER)
			{
				player->SetPos(Point({ RIGHT_TP_POS_X,RIGHT_TP_POS_Y }));
				it++;
			}
		}
		else it++;
	}
	blinky_box = blinky->GetHitbox();
	pinky_box = blinky->GetHitbox();
	inky_box = blinky->GetHitbox();
	clyde_box = clyde->GetHitbox();

	if (player_box.TestAABB(blinky_box)) returnMainMenu = true;
	if (player_box.TestAABB(pinky_box)) returnMainMenu = true;
	if (player_box.TestAABB(inky_box)) returnMainMenu = true;
	if (player_box.TestAABB(clyde_box)) returnMainMenu = true;

}
bool Scene::GetReturnMainMenu()
{
	return returnMainMenu;
}

void Scene::ClearLevel()
{
	for (Object* obj : objects)
	{
		delete obj;
	}
	objects.clear();
	
}
void Scene::RenderObjects() const
{
	for (Object* obj : objects)
	{
		obj->Draw();
	}
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (Object* obj : objects)
	{
		obj->DrawDebug(col);
	}
}
void Scene::RenderGUI() const
{
	//Temporal approach
	DrawText(TextFormat("SCORE : %d", player->GetScore()), 10, 10, 8, LIGHTGRAY);
}