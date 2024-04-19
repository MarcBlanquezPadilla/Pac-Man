#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include "LogMessages.h"

Player::Player(const Point& p, State s, Look view) :
	Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
{
	state = s;
	look = view;
	map = nullptr;
	score = 0;
}
Player::~Player()
{

}
AppStatus Player::Initialise()
{
	int i;
	const int n = PLAYER_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(ResourceType::IMG_PLAYER, "resources/sprites/Pac-manX2.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(ResourceType::IMG_PLAYER));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)PlayerAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)PlayerAnim::BITE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::BITE_RIGHT, { (float)0 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_RIGHT, { (float)1 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_RIGHT, { (float)2 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_RIGHT, { (float)1 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_RIGHT, { (float)0 * n, 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::BITE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::BITE_LEFT, { (float)3 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_LEFT, { (float)4 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_LEFT, { (float)5 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_LEFT, { (float)4 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_LEFT, { (float)3 * n, 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::BITE_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::BITE_UP, { (float)6 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_UP, { (float)7 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_UP, { (float)8 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_UP, { (float)7 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_UP, { (float)6 * n, 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::BITE_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::BITE_DOWN, { (float)9 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_DOWN, { (float)10 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_DOWN, { (float)11 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_DOWN, { (float)10 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_DOWN, { (float)9 * n, 0, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::DIE, ANIM_DELAY);
	for (i = 0; i < 11; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::DIE, { (float)i * n, 1, n, n });
	
	state = State::WALKING;
	look = Look::RIGHT;
	SetAnimation((int)PlayerAnim::BITE_RIGHT);

	return AppStatus::OK;
}
void Player::InitScore()
{
	score = 0;
}
void Player::IncrScore(int n)
{
	score += n;
}
int Player::GetScore()
{
	return score;
}
void Player::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Player::IsLookingRight() const
{
	return look == Look::RIGHT;
}
bool Player::IsLookingLeft() const
{
	return look == Look::LEFT;
}
bool Player::IsLookingUp() const
{
	return look == Look::UP;
}
bool Player::IsLookingDown() const
{
	return look == Look::DOWN;
}
bool Player::JustOneKeyIsDown()
{	
	int keysDown = 0;
	
	if (IsKeyDown(KEY_LEFT))
	{
		keysDown = keysDown + 1;
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		keysDown = keysDown + 1;
	}
	if (IsKeyDown(KEY_UP))
	{
		keysDown = keysDown + 1;
	}
	if (IsKeyDown(KEY_DOWN))
	{
		keysDown = keysDown + 1;
	}

	if (keysDown == 1)
		return true;
	else return false;
}

bool Player::IsInFirstHalfTile() const
{
	return pos.y % TILE_SIZE < TILE_SIZE / 2;
}
bool Player::IsInSecondHalfTile() const
{
	return pos.y % TILE_SIZE >= TILE_SIZE / 2;
}
void Player::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
PlayerAnim Player::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (PlayerAnim)sprite->GetAnimation();
}
void Player::Stop()
{
	dir = { 0,0 };
	state = State::IDLE;
	if (IsLookingRight())		SetAnimation((int)PlayerAnim::BITE_RIGHT);
	else if (IsLookingLeft())	SetAnimation((int)PlayerAnim::BITE_LEFT);
	else if (IsLookingUp())		SetAnimation((int)PlayerAnim::BITE_UP);
	else						SetAnimation((int)PlayerAnim::BITE_DOWN);
}
void Player::StartWalkingLeft()
{
	state = State::WALKING;
	look = Look::LEFT;
	SetAnimation((int)PlayerAnim::BITE_LEFT);
}
void Player::StartWalkingRight()
{
	state = State::WALKING;
	look = Look::RIGHT;
	SetAnimation((int)PlayerAnim::BITE_RIGHT);
}
void Player::StartWalkingUp()
{
	state = State::WALKING;
	look = Look::UP;
	SetAnimation((int)PlayerAnim::BITE_UP);
}
void Player::StartWalkingDown()
{
	state = State::WALKING;
	look = Look::DOWN;
	SetAnimation((int)PlayerAnim::BITE_DOWN);
}

void Player::Update()
{
	//Player doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.
	
	ChangeMove();
	Move();

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}

void Player::Move()
{
	AABB box;
	int prev_x = pos.x;
	int prev_y = pos.y;

	if (look == Look::RIGHT)
	{
		pos.x += PLAYER_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
	}
	else if (look == Look::LEFT)
	{
		pos.x += -PLAYER_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
	}
	else if (look == Look::UP)
	{
		pos.y += -PLAYER_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallUp(box))
		{
			pos.y = prev_y;
			if (state == State::WALKING) Stop();
		}
	}
	else
	{
		pos.y += PLAYER_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallDown(box))
		{
			pos.y = prev_y;
			if (state == State::WALKING) Stop();
		}
	}
}

void Player::ChangeMove()
{
	if (JustOneKeyIsDown())
	{
		AABB box;
		int prev_x = pos.x;
		int prev_y = pos.y;

		if (IsKeyDown(KEY_LEFT) && look != Look::LEFT)
		{
			pos.x += -PLAYER_SPEED;

			box = GetHitbox();
			if (!map->TestCollisionWallLeft(box))
				StartWalkingLeft();
		}
		else if (IsKeyDown(KEY_RIGHT) && look != Look::RIGHT)
		{
			pos.x += PLAYER_SPEED;

			box = GetHitbox();
			if (!map->TestCollisionWallRight(box)) StartWalkingRight();
		}
		else if (IsKeyDown(KEY_UP) && look != Look::UP)
		{
			pos.y += -PLAYER_SPEED;

			box = GetHitbox();
			if (!map->TestCollisionWallUp(box)) StartWalkingUp();
		}
		else if (IsKeyDown(KEY_DOWN) && look != Look::DOWN)
		{
			pos.y += PLAYER_SPEED;

			box = GetHitbox();
			if (!map->TestCollisionWallDown(box)) StartWalkingDown();
		}
		pos.x = prev_x;
		pos.y = prev_y;
	}
}


void Player::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);

	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18 * 16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}

void Player::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(ResourceType::IMG_PLAYER);

	render->Release();
}