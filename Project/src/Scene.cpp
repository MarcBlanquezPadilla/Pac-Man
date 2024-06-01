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
	navMesh = nullptr;

	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { MARGIN_GUI_X, MARGIN_GUI_Y };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom

	totalPelets = 0;
	peletsCollected = 0;
	munch = 1;

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
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(ResourceType::IMG_ITEMS, "resources/sprites/ObjectsX2.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::SOUND_MUNCH_1, "resources/sounds/FX/munch_1.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::SOUND_MUNCH_2, "resources/sounds/FX/munch_2.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::MUSIC_START, "resources/sounds/Music/game_start.ogg") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::SOUND_RETREATING, "resources/sounds/FX/retreating.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	
	if (data.LoadSound(ResourceType::SOUND_POWER_PELLET, "resources/sounds/FX/power_pellet.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::SOUND_SIREN_1, "resources/sounds/FX/siren_1.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::SOUND_SIREN_2, "resources/sounds/FX/siren_2.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::SOUND_SIREN_3, "resources/sounds/FX/siren_3.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::SOUND_SIREN_4, "resources/sounds/FX/siren_4.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::SOUND_SIREN_5, "resources/sounds/FX/siren_5.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	startMusic = data.GetSound(ResourceType::MUSIC_START);
	power_pellet = data.GetSound(ResourceType::SOUND_POWER_PELLET);
	retreating = data.GetSound(ResourceType::SOUND_RETREATING);
	siren_1 = data.GetSound(ResourceType::SOUND_SIREN_1);
	siren_2 = data.GetSound(ResourceType::SOUND_SIREN_2);
	siren_3 = data.GetSound(ResourceType::SOUND_SIREN_3);
	siren_4 = data.GetSound(ResourceType::SOUND_SIREN_4);
	siren_5 = data.GetSound(ResourceType::SOUND_SIREN_5);
	munch_1 = data.GetSound(ResourceType:: SOUND_MUNCH_1);
	munch_2 = data.GetSound(ResourceType:: SOUND_MUNCH_2);

	//Create player
	player = new Player({ 0, 0 }, PlayerState::IDLE, Directions::RIGHT);
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
	blinky = new Blinky({ 0, 0 }, GhostState::SCATTLE, Directions::RIGHT);
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
	pinky = new Pinky({ 0, 0 }, GhostState::SCATTLE, Directions::RIGHT);
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
	inky = new Inky({ 0, 0 }, GhostState::SCATTLE, Directions::RIGHT);
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
	clyde = new Clyde({ 0, 0 }, GhostState::SCATTLE, Directions::RIGHT);
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
	//Create navMesh
	navMesh = new NavMesh();
	if (navMesh == nullptr)
	{
		LOG("Failed to allocate memory for Navigation Mesh");
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
	blinky->SetNavMesh(navMesh);
	blinky->SetPlayer(player);
	pinky->SetTileMap(level);
	pinky->SetNavMesh(navMesh);
	pinky->SetPlayer(player);
	inky->SetTileMap(level);
	inky->SetNavMesh(navMesh);
	inky->SetPlayer(player);
	inky->SetBlinky(blinky);
	clyde->SetNavMesh(navMesh);
	clyde->SetTileMap(level);
	clyde->SetPlayer(player);

	started = false;
	ghostState = GhostState::SCATTLE;
	lastStateChangeTime = 0;
	PlaySound(*startMusic);
	return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int* map = nullptr;
	int* navMeshMap = nullptr;
	Object* obj;

	ClearLevel();

	if (stage == 1)
	{
		size = LEVEL_WIDTH * LEVEL_HEIGHT;
		map = new int[size] {
			0,	0,	0,	0,	0,	0,	30,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	2,	2,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	31,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	101,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	102,0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	7,	2,	2,	2,	2,	6,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	7,	2,	2,	2,	2,	6,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	52,	0,	0,	5,	0,	0,	0,	0,	4,	0,	51,	0,	0,	5,	0,	0,	0,	0,	0,	0,	4,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	5,	0,	0,	0,	0,	0,	0,	4,	0,	51,	0,	0,	5,	0,	0,	0,	0,	4,	0,	52,	0,	0,	26,	0,	0,	0,	0,	0,	0,
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
			0,	0,	0,	0,	0,	0,	32,	24,	24,	24,	24,	24,	24,	24,	24,	24,	31,	0,	0,	0,	0,	5,	0,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	0,	4,	0,	0,	0,	0,	30,	24,	24,	24,	24,	24,	24,	24,	24,	24, 33,	0,	0,	0,	0,	0,	0,
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
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
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
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	100,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	52,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	0,	0,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	52,	0,	0,	26,	0,	0,	0,	0,	0,	0,
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
	
		size = (LEVEL_WIDTH / 2) * (LEVEL_HEIGHT / 2);
		navMeshMap = new int[size] {
			1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,
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
				obj = new Object(pos, pHitBox, ObjectType::SMALL_PELET, SMALL_OBJECT_SIZE, SMALL_OBJECT_SIZE / 2 );
				objects.push_back(obj);
				map[i] = 0;
				totalPelets++;
			}
			else if (tile == Tile::LARGE_PELET)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE;
				obj = new Object(pos, ObjectType::LARGE_PELET, MEDIUM_OBJECT_SIZE);
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
	
	//Nav mesh map
	navMesh->Load(navMeshMap, LEVEL_WIDTH / 2, LEVEL_HEIGHT / 2);

	delete map;
	delete navMeshMap;

	return AppStatus::OK;
}
void Scene::Update()
{
	started = !IsSoundPlaying(*startMusic);

	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}

	if (started)
	{
		level->Update();
		player->Update();
		UpdateGhostState();
		blinky->Update();
		pinky->Update();
		inky->Update();
		clyde->Update();
		CheckCollisions();
		PlaySounds();
	}
}
void Scene::PlaySounds()
{
	const Sound* currentSound = nullptr;

	if (blinky->GetState() == GhostState::EATEN || pinky->GetState() == GhostState::EATEN || inky->GetState() == GhostState::EATEN || clyde->GetState() == GhostState::EATEN)
	{
		currentSound = retreating;
	}
	else if (blinky->GetState() == GhostState::FRIGHTENED || pinky->GetState() == GhostState::FRIGHTENED || inky->GetState() == GhostState::FRIGHTENED || clyde->GetState() == GhostState::FRIGHTENED)
	{
		currentSound = power_pellet;
	}
	else
	{
		float percent = static_cast<float>(peletsCollected) / static_cast<float>(totalPelets) * 100;

		if (percent < 30)
		{
			currentSound = siren_1;
		}
		else if (percent < 50)
		{
			currentSound = siren_2;
		}
		else if (percent < 65)
		{
			currentSound = siren_3;
		}
		else if (percent < 80)
		{
			currentSound = siren_4;
		}
		else if (percent < 90)
		{
			currentSound = siren_5;
		}
	}

	// Stop all sounds except the current one
	StopSoundsInException(currentSound);

	// Play the current sound if it's not already playing
	if (currentSound && !IsSoundPlaying(*currentSound))
	{
		PlaySound(*currentSound);
	}
}

void Scene::StopSoundsInException(const Sound* sound)
{
	if (retreating != sound && IsSoundPlaying(*retreating)) StopSound(*retreating);
	if (power_pellet != sound && IsSoundPlaying(*power_pellet)) StopSound(*power_pellet);
	if (siren_1 != sound && IsSoundPlaying(*siren_1)) StopSound(*siren_1);
	if (siren_2 != sound && IsSoundPlaying(*siren_2)) StopSound(*siren_2);
	if (siren_3 != sound && IsSoundPlaying(*siren_3)) StopSound(*siren_3);
	if (siren_4 != sound && IsSoundPlaying(*siren_4)) StopSound(*siren_4);
	if (siren_5 != sound && IsSoundPlaying(*siren_5)) StopSound(*siren_5);
}
void Scene::Render()
{
	BeginMode2D(camera);

	level->Render();

	if (debug == DebugMode::SPRITES_AND_NAVMESH)
	{
		navMesh->DrawDebug(RED);
	}
	if (debug == DebugMode::SPRITES_AND_NAVMESH_ROUTES || debug == DebugMode::SPRITES_AND_NAVMESH)
	{
		blinky->DrawDebugNavmesh(RED);
		pinky->DrawDebugNavmesh(PINK);
		inky->DrawDebugNavmesh(SKYBLUE);
		clyde->DrawDebugNavmesh(ORANGE);
	}
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::SPRITES_AND_NAVMESH || debug == DebugMode::SPRITES_AND_NAVMESH_ROUTES || debug == DebugMode::SPRITES_AND_NAVMESH)
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
		DrawPixel(0, 0, WHITE);
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
void Scene::UpdateGhostState()
{
	if (lastStateChangeTime == 0) lastStateChangeTime = static_cast<float>(GetTime());
	float currentTime = static_cast<float>(GetTime());
	float elapsedTime = currentTime - lastStateChangeTime;

	if (ghostState == GhostState::SCATTLE && elapsedTime >= TIME_IN_SCATTER) {
		ghostState = GhostState::CHASE;
		blinky->ChangeCommonState(ghostState);
		pinky->ChangeCommonState(ghostState);
		inky->ChangeCommonState(ghostState);
		clyde->ChangeCommonState(ghostState);
		lastStateChangeTime = currentTime;
	}
	else if (ghostState == GhostState::CHASE && elapsedTime >= TIME_IN_CHASE) {
		ghostState = GhostState::SCATTLE;
		blinky->ChangeCommonState(ghostState);
		pinky->ChangeCommonState(ghostState);
		inky->ChangeCommonState(ghostState);
		clyde->ChangeCommonState(ghostState);
		lastStateChangeTime = currentTime;
	}
}
void Scene::CheckCollisions()
{
	AABB player_box, obj_box, blinky_box, pinky_box, inky_box, clyde_box;

	player_box = player->GetHitbox();
	blinky_box = blinky->GetHitbox();
	pinky_box = pinky->GetHitbox();
	inky_box = inky->GetHitbox();
	clyde_box = clyde->GetHitbox();
	
	auto it = objects.begin();
	while (it != objects.end())
	{
		obj_box = (*it)->GetHitbox();
		ObjectType type = (*it)->GetObjectType();
		if (player_box.TestAABB(obj_box))
		{
			if (type == ObjectType::SMALL_PELET)
			{
				if (munch_1 != nullptr) {
					if (!IsSoundPlaying(*munch_1) && !IsSoundPlaying(*munch_2))
						if (munch == 1) 
						{ 
							PlaySound(*munch_1); 
							munch = 2; 
						}
						else if (munch == 2) 
						{ 
							PlaySound(*munch_2); 
							munch = 1; 
						}
				}
				player->IncrScore((*it)->Points());
				delete* it;
				it = objects.erase(it);
				peletsCollected++;
				if (peletsCollected == totalPelets)	returnMainMenu = true;
			}
			else if (type == ObjectType::LARGE_PELET)
			{
				blinky->ChangeState(GhostState::FRIGHTENED);
				pinky->ChangeState(GhostState::FRIGHTENED);
				inky->ChangeState(GhostState::FRIGHTENED);
				clyde->ChangeState(GhostState::FRIGHTENED);
				delete* it;
				it = objects.erase(it);
			}
			else if (type == ObjectType::RIGHT_TELEPORTER)
			{
				player->SetPos(Point({ LEFT_TP_POS_X, LEFT_TP_POS_Y }));
			}
			else if (type == ObjectType::LEFT_TELEPORTER)
			{
				player->SetPos(Point({ RIGHT_TP_POS_X,RIGHT_TP_POS_Y }));
			}
		}
		if (blinky_box.TestAABB(obj_box))
		{
			if (type == ObjectType::RIGHT_TELEPORTER)
			{
				blinky->SetPos(Point({ LEFT_TP_POS_X, LEFT_TP_POS_Y }));
			}
			else if (type == ObjectType::LEFT_TELEPORTER)
			{
				blinky->SetPos(Point({ RIGHT_TP_POS_X,RIGHT_TP_POS_Y }));
			}
		}
		if (pinky_box.TestAABB(obj_box))
		{
			if (type == ObjectType::RIGHT_TELEPORTER)
			{
				pinky->SetPos(Point({ LEFT_TP_POS_X, LEFT_TP_POS_Y }));
			}
			else if (type == ObjectType::LEFT_TELEPORTER)
			{
				pinky->SetPos(Point({ RIGHT_TP_POS_X,RIGHT_TP_POS_Y }));
			}
		}
		if (inky_box.TestAABB(obj_box))
		{
			if (type == ObjectType::RIGHT_TELEPORTER)
			{
				inky->SetPos(Point({ LEFT_TP_POS_X, LEFT_TP_POS_Y }));
			}
			else if (type == ObjectType::LEFT_TELEPORTER)
			{
				inky->SetPos(Point({ RIGHT_TP_POS_X,RIGHT_TP_POS_Y }));
			}
		}
		if (clyde_box.TestAABB(obj_box))
		{
			if (type == ObjectType::RIGHT_TELEPORTER)
			{
				clyde->SetPos(Point({ LEFT_TP_POS_X, LEFT_TP_POS_Y }));
			}
			else if (type == ObjectType::LEFT_TELEPORTER)
			{
				clyde->SetPos(Point({ RIGHT_TP_POS_X,RIGHT_TP_POS_Y }));
			}
		}
		
		if (type == ObjectType::SMALL_PELET)
		{
			if (!player_box.TestAABB(obj_box)) it++;
		}
		else if (type == ObjectType::LARGE_PELET)
		{
			if (!player_box.TestAABB(obj_box)) it++;
		}
		else if (type == ObjectType::RIGHT_TELEPORTER)
		{
			it++;
		}
		else if (type == ObjectType::LEFT_TELEPORTER)
		{
			it++;
		}
	}

	if (player_box.TestAABB(blinky_box))
	{
		if (blinky->GetState() == GhostState::FRIGHTENED) //COMIDO
		{
			blinky->ChangeState(GhostState::EATEN); 
		}
		else if (blinky->GetState() == GhostState::SCATTLE || blinky->GetState() == GhostState::CHASE) returnMainMenu = true;
	}
	if (player_box.TestAABB(pinky_box)) 
	{
		if (pinky->GetState() == GhostState::FRIGHTENED) //COMIDO
		{
			pinky->ChangeState(GhostState::EATEN); 
		}
		else if (pinky->GetState() == GhostState::SCATTLE || pinky->GetState() == GhostState::CHASE) returnMainMenu = true;
	}
	if (player_box.TestAABB(inky_box)) 
	{
		if (inky->GetState() == GhostState::FRIGHTENED) //COMIDO
		{
			inky->ChangeState(GhostState::EATEN); 
		}
		else if (inky->GetState() == GhostState::SCATTLE || inky->GetState() == GhostState::CHASE) returnMainMenu = true;
	}
	if (player_box.TestAABB(clyde_box)) 
	{
		if (clyde->GetState() == GhostState::FRIGHTENED) //COMIDO
		{
			clyde->ChangeState(GhostState::EATEN); 
		}
		else if (clyde->GetState() == GhostState::SCATTLE || clyde->GetState() == GhostState::CHASE) returnMainMenu = true;
	}

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