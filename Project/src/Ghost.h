#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 32x32
#define GHOST_FRAME_SIZE		32

//Logical model size: 12x28
#define GHOST_PHYSICAL_WIDTH	32
#define GHOST_PHYSICAL_HEIGHT	32

//Horizontal speed and vertical speed while falling down
#define GHOST_SPEED			2

//Logic states
enum class GhostState { IDLE, WALKING, DEAD };
enum class GhostLook { RIGHT, LEFT, UP, DOWN };

//Rendering states
enum class GhostAnim {
	WALK_UP, WALK_DOWN,
	WALK_LEFT, WALK_RIGHT,
	DIE, NUM_ANIMATIONS
};

class Ghost : public Entity
{
public:
	Ghost(const Point& p, GhostState s, GhostLook view);
	~Ghost();

	virtual AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

protected:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;
	bool IsLookingUp() const;
	bool IsLookingDown() const;

	//Player mechanics
	void Move();
	virtual void ChangeMove();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartWalkingDown();
	void StartWalkingUp();
	void Stop();

	//Animation management
	void SetAnimation(int id);
	GhostAnim GetAnimation();

	GhostState state;
	GhostLook look;

	TileMap* map;

	int score;
};
