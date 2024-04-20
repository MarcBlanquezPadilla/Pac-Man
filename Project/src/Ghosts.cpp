#include "Ghosts.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include "LogMessages.h"


Ghost::Ghost(const Point& p, StateG s, LookG view) :
	Entity(p, GHOST_PHYSICAL_WIDTH, GHOST_PHYSICAL_HEIGHT, GHOST_FRAME_SIZE, GHOST_FRAME_SIZE)
{
	state = s;
	look = view;
	map = nullptr;
	
}
Ghost::~Ghost()
{

}

AppStatus Ghost::Initialise()
{
	int i;
	const int n = GHOST_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(ResourceType::IMG_GHOST, "resources/sprites/GhostsX2.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(ResourceType::IMG_GHOST));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)GhostAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)GhostAnim::BITE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::BITE_RIGHT, { (float)0 * n, 0, n, n });
	sprite->AddKeyFrame((int)GhostAnim::BITE_RIGHT, { (float)1 * n, 0, n, n });
	sprite->AddKeyFrame((int)GhostAnim::BITE_RIGHT, { (float)2 * n, 0, n, n });
	sprite->AddKeyFrame((int)GhostAnim::BITE_RIGHT, { (float)1 * n, 0, n, n });
	sprite->AddKeyFrame((int)GhostAnim::BITE_RIGHT, { (float)0 * n, 0, n, n });
	sprite->SetAnimationDelay((int)GhostAnim::BITE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::BITE_LEFT, { (float)3 * n, 0, n, n });
	sprite->AddKeyFrame((int)GhostAnim::BITE_LEFT, { (float)4 * n, 0, n, n });
	sprite->AddKeyFrame((int)GhostAnim::BITE_LEFT, { (float)5 * n, 0, n, n });
	sprite->AddKeyFrame((int)GhostAnim::BITE_LEFT, { (float)4 * n, 0, n, n });
	sprite->AddKeyFrame((int)GhostAnim::BITE_LEFT, { (float)3 * n, 0, n, n });
	sprite->SetAnimationDelay((int)GhostAnim::BITE_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::BITE_UP, { (float)6 * n, 0, n, n });
	sprite->AddKeyFrame((int)GhostAnim::BITE_UP, { (float)7 * n, 0, n, n });
	sprite->AddKeyFrame((int)GhostAnim::BITE_UP, { (float)8 * n, 0, n, n });
	sprite->AddKeyFrame((int)GhostAnim::BITE_UP, { (float)7 * n, 0, n, n });
	sprite->AddKeyFrame((int)GhostAnim::BITE_UP, { (float)6 * n, 0, n, n });
	sprite->SetAnimationDelay((int)GhostAnim::BITE_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::BITE_DOWN, { (float)9 * n, 0, n, n });
	sprite->AddKeyFrame((int)GhostAnim::BITE_DOWN, { (float)10 * n, 0, n, n });
	sprite->AddKeyFrame((int)GhostAnim::BITE_DOWN, { (float)11 * n, 0, n, n });
	sprite->AddKeyFrame((int)GhostAnim::BITE_DOWN, { (float)10 * n, 0, n, n });
	sprite->AddKeyFrame((int)GhostAnim::BITE_DOWN, { (float)9 * n, 0, n, n });

	sprite->SetAnimationDelay((int)GhostAnim::DIE, ANIM_DELAY);
	for (i = 0; i < 11; ++i)
		sprite->AddKeyFrame((int)GhostAnim::DIE, { (float)i * n, 1, n, n });

	state = StateG::WALKINGG;
	look = LookG::RIGHTG;
	SetAnimation((int)GhostAnim::BITE_RIGHT);

	return AppStatus::OK;

}




void Ghost::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Ghost::IsLookingRight() const
{
	return look == LookG::RIGHTG;
}
bool Ghost::IsLookingLeft() const
{
	return look == LookG::LEFTG;
}
bool Ghost::IsLookingUp() const
{
	return look == LookG::UPG;
}
bool Ghost::IsLookingDown() const
{
	return look == LookG::DOWNG;
}
bool Ghost::JustOneKeyIsDown()
{
	int keysDown = 0;

	if (IsKeyDown(KEY_A))
	{
		keysDown = keysDown + 1;
	}
	if (IsKeyDown(KEY_D))
	{
		keysDown = keysDown + 1;
	}
	if (IsKeyDown(KEY_W))
	{
		keysDown = keysDown + 1;
	}
	if (IsKeyDown(KEY_S))
	{
		keysDown = keysDown + 1;
	}

	if (keysDown == 1)
		return true;
	else return false;
}

bool Ghost::IsInFirstHalfTile() const
{
	return pos.y % TILE_SIZE < TILE_SIZE / 2;
}
bool Ghost::IsInSecondHalfTile() const
{
	return pos.y % TILE_SIZE >= TILE_SIZE / 2;
}
void Ghost::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
GhostAnim Ghost::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (GhostAnim)sprite->GetAnimation();
}

void Ghost::Stop()
{
	dir = { 0,0 };
	state = StateG::IDLEG;
	if (IsLookingRight())		SetAnimation((int)GhostAnim::BITE_RIGHT);
	else if (IsLookingLeft())	SetAnimation((int)GhostAnim::BITE_LEFT);
	else if (IsLookingUp())		SetAnimation((int)GhostAnim::BITE_UP);
	else						SetAnimation((int)GhostAnim::BITE_DOWN);
}


void Ghost::StartWalkingLeft()
{
	state = StateG::WALKINGG;
	look = LookG::LEFTG;
	SetAnimation((int)GhostAnim::BITE_LEFT);
}
void Ghost::StartWalkingRight()
{
	state = StateG::WALKINGG;
	look = LookG::RIGHTG;
	SetAnimation((int)GhostAnim::BITE_RIGHT);
}
void Ghost::StartWalkingUp()
{
	state = StateG::WALKINGG;
	look = LookG::UPG;
	SetAnimation((int)GhostAnim::BITE_UP);
}
void Ghost::StartWalkingDown()
{
	state = StateG::WALKINGG;
	look = LookG::DOWNG;
	SetAnimation((int)GhostAnim::BITE_DOWN);
}

void Ghost::Update()
{
	//Player doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.

	ChangeMove();
	Move();

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}


void Ghost::Move()
{
	AABB box;
	int prev_x = pos.x;
	int prev_y = pos.y;

	if (look == LookG::RIGHTG)
	{
		pos.x += GHOST_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == StateG::WALKINGG) Stop();
		}
	}
	else if (look == LookG::LEFTG)
	{
		pos.x += -GHOST_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			if (state == StateG::WALKINGG) Stop();
		}
	}
	else if (look == LookG::UPG)
	{
		pos.y += -GHOST_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallUp(box))
		{
			pos.y = prev_y;
			if (state == StateG::WALKINGG) Stop();
		}
	}
	else
	{
		pos.y += GHOST_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallDown(box))
		{
			pos.y = prev_y;
			if (state == StateG::WALKINGG) Stop();
		}
	}
}



void Ghost::ChangeMove()
{
	if (JustOneKeyIsDown())
	{
		AABB box;
		int prev_x = pos.x;
		int prev_y = pos.y;

		if (IsKeyDown(KEY_LEFT) && look != LookG::LEFTG)
		{
			pos.x += -GHOST_SPEED;

			box = GetHitbox();
			if (!map->TestCollisionWallLeft(box))
				StartWalkingLeft();
		}
		else if (IsKeyDown(KEY_RIGHT) && look != LookG::RIGHTG)
		{
			pos.x += GHOST_SPEED;

			box = GetHitbox();
			if (!map->TestCollisionWallRight(box)) StartWalkingRight();
		}
		else if (IsKeyDown(KEY_UP) && look != LookG::UPG)
		{
			pos.y += -GHOST_SPEED;

			box = GetHitbox();
			if (!map->TestCollisionWallUp(box)) StartWalkingUp();
		}
		else if (IsKeyDown(KEY_DOWN) && look != LookG::DOWNG)
		{
			pos.y += GHOST_SPEED;

			box = GetHitbox();
			if (!map->TestCollisionWallDown(box)) StartWalkingDown();
		}
		pos.x = prev_x;
		pos.y = prev_y;
	}
}


void Ghost::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);

	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18 * 16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}

void Ghost::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(ResourceType::IMG_GHOST);

	render->Release();
}