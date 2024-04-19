#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 32x32
#define PLAYER_FRAME_SIZE		32

//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	32
#define PLAYER_PHYSICAL_HEIGHT	32

//Player spawn positions
#define PLAYER_SPAWN_X	208//(LEVEL_WIDTH*TILE_SIZE)/2 - PLAYER_PHYSICAL_WIDTH/2
#define PLAYER_SPAWN_Y	295//(LEVEL_HEIGHT*TILE_SIZE)/2 + PLAYER_PHYSICAL_HEIGHT/2

//Horizontal speed and vertical speed while falling down
#define PLAYER_SPEED			2

//Logic states
enum class State { IDLE, WALKING, DEAD };
enum class Look { RIGHT, LEFT, UP, DOWN };

//Rendering states
enum class PlayerAnim {
	BITE_UP, BITE_DOWN,
	BITE_LEFT, BITE_RIGHT,
	DIE, NUM_ANIMATIONS
};

class Player : public Entity
{
public:
	Player(const Point& p, State s, Look view);
	~Player();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitScore();
	void IncrScore(int n);
	int GetScore();

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;
	bool IsLookingUp() const;
	bool IsLookingDown() const;
	bool JustOneKeyIsDown();

	//Player mechanics
	void Move();
	void ChangeMove();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartWalkingDown();
	void StartWalkingUp();
	void Stop();

	//Animation management
	void SetAnimation(int id);
	PlayerAnim GetAnimation();

	//Ladder get in/out steps
	bool IsInFirstHalfTile() const;
	bool IsInSecondHalfTile() const;

	State state;
	Look look;

	TileMap* map;

	int score;
};

