#pragma once

#include "Entity.h"
#include "TileMap.h"


//Representation model size: 32x32
#define GHOST_FRAME_SIZE		32

//Logical model size: 12x28
#define GHOST_PHYSICAL_WIDTH	32
#define GHOST_PHYSICAL_HEIGHT	32

//Player spawn positions
#define GHOST_SPAWN_X	248//(LEVEL_WIDTH*TILE_SIZE)/2 - PLAYER_PHYSICAL_WIDTH/2
#define GHOST_SPAWN_Y	264//(LEVEL_HEIGHT*TILE_SIZE)/2 + PLAYER_PHYSICAL_HEIGHT/2

//Horizontal speed and vertical speed while falling down
#define GHOST_SPEED			2

//Logic states
enum class StateG { IDLEG, WALKINGG, DEADG };
enum class LookG { RIGHTG, LEFTG, UPG, DOWNG };



//Rendering states
enum class GhostAnim {
	BITE_UP, BITE_DOWN,
	BITE_LEFT, BITE_RIGHT,
	DIE, NUM_ANIMATIONS
};



class Ghost : public Entity
{
public:
	Ghost(const Point& p, StateG s, LookG view);
	~Ghost();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);
//
//	void InitScore();
//	void IncrScore(int n);
//	int GetScore();
//
	void Update();
	void DrawDebug(const Color& col) const;
	void Release();
//
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
	GhostAnim GetAnimation();

	//Ladder get in/out steps
	bool IsInFirstHalfTile() const;
	bool IsInSecondHalfTile() const;

	StateG state;
	LookG look;

	TileMap* map;
};
