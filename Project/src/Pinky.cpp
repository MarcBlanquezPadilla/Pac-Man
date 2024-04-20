#include "Pinky.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include "LogMessages.h"

Pinky::Pinky(const Point& p, GhostState s, GhostLook view) : Ghost(p, s, view)
{
	state = s;
	look = view;
	map = nullptr;
	score = 0;
}

AppStatus Pinky::Initialise()
{
	int i;
	const int n = GHOST_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(ResourceType::IMG_GHOSTS, "resources/sprites/GhostsX2.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(ResourceType::IMG_GHOSTS));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)GhostAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)GhostAnim::WALK_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::WALK_RIGHT, { 0 * n, 1 * n, n, n });
	sprite->AddKeyFrame((int)GhostAnim::WALK_RIGHT, { 1 * n, 1 * n, n, n });
	sprite->SetAnimationDelay((int)GhostAnim::WALK_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::WALK_LEFT, { 2 * n, 1 * n, n, n });
	sprite->AddKeyFrame((int)GhostAnim::WALK_LEFT, { 3 * n, 1 * n, n, n });
	sprite->SetAnimationDelay((int)GhostAnim::WALK_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::WALK_UP, { 4 * n, 1 * n, n, n });
	sprite->AddKeyFrame((int)GhostAnim::WALK_UP, { 5 * n, 1 * n, n, n });
	sprite->SetAnimationDelay((int)GhostAnim::WALK_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::WALK_DOWN, { 6 * n, 1 * n, n, n });
	sprite->AddKeyFrame((int)GhostAnim::WALK_DOWN, { 7 * n, 1 * n, n, n });

	sprite->SetAnimationDelay((int)GhostAnim::DIE, ANIM_DELAY);
	for (i = 0; i < 11; ++i)
		sprite->AddKeyFrame((int)GhostAnim::DIE, { (float)i * n, 1, n, n });

	state = GhostState::WALKING;
	look = GhostLook::RIGHT;
	SetAnimation((int)GhostAnim::WALK_RIGHT);

	return AppStatus::OK;
}