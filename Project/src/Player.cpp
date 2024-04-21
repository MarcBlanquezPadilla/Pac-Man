#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include "LogMessages.h"

Player::Player(const Point& p, PlayerState s, PlayerLook view) :
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
	
	state = PlayerState::WALKING;
	look = PlayerLook::RIGHT;
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
	return look == PlayerLook::RIGHT;
}
bool Player::IsLookingLeft() const
{
	return look == PlayerLook::LEFT;
}
bool Player::IsLookingUp() const
{
	return look == PlayerLook::UP;
}
bool Player::IsLookingDown() const
{
	return look == PlayerLook::DOWN;
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
void Player::SetCurrentDelayToAnimation(int i)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetCurrentDelay(i);
}
void Player::Stop()
{
	dir = { 0,0 };
	state = PlayerState::IDLE;
}
void Player::StartWalkingLeft()
{
	state = PlayerState::WALKING;
	look = PlayerLook::LEFT;
	SetAnimation((int)PlayerAnim::BITE_LEFT);
}
void Player::StartWalkingRight()
{
	state = PlayerState::WALKING;
	look = PlayerLook::RIGHT;
	SetAnimation((int)PlayerAnim::BITE_RIGHT);
}
void Player::StartWalkingUp()
{
	state = PlayerState::WALKING;
	look = PlayerLook::UP;
	SetAnimation((int)PlayerAnim::BITE_UP);
}
void Player::StartWalkingDown()
{
	state = PlayerState::WALKING;
	look = PlayerLook::DOWN;
	SetAnimation((int)PlayerAnim::BITE_DOWN);
}

void Player::Update()
{
	//Player doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.
	
	ChangeMove();
	if (state == PlayerState::WALKING)
	{
		Move();
	}
	else if (state == PlayerState::IDLE)
	{
		SetCurrentDelayToAnimation(1000); //PAUSAR ANIMACIO
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}

void Player::Move()
{
	AABB box;
	int prev_x = pos.x;
	int prev_y = pos.y;

	if (look == PlayerLook::RIGHT)
	{
		pos.x += PLAYER_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == PlayerState::WALKING) Stop();
		}
	}
	else if (look == PlayerLook::LEFT)
	{
		pos.x += -PLAYER_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			if (state == PlayerState::WALKING) Stop();
		}
	}
	else if (look == PlayerLook::UP)
	{
		pos.y += -PLAYER_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallUp(box))
		{
			pos.y = prev_y;
			if (state == PlayerState::WALKING) Stop();
		}
	}
	else
	{
		pos.y += PLAYER_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallDown(box))
		{
			pos.y = prev_y;
			if (state == PlayerState::WALKING) Stop();
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

		if (IsKeyDown(KEY_LEFT) && look != PlayerLook::LEFT)
		{
			pos.x += -PLAYER_SPEED;

			box = GetHitbox();
			if (!map->TestCollisionWallLeft(box))
				StartWalkingLeft();
		}
		else if (IsKeyDown(KEY_RIGHT) && look != PlayerLook::RIGHT)
		{
			pos.x += PLAYER_SPEED;

			box = GetHitbox();
			if (!map->TestCollisionWallRight(box)) StartWalkingRight();
		}
		else if (IsKeyDown(KEY_UP) && look != PlayerLook::UP)
		{
			pos.y += -PLAYER_SPEED;

			box = GetHitbox();
			if (!map->TestCollisionWallUp(box)) StartWalkingUp();
		}
		else if (IsKeyDown(KEY_DOWN) && look != PlayerLook::DOWN)
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
	Entity::DrawHitbox(GREEN);

	DrawText(TextFormat("Player\nPosition: (%d,%d)\nSize: %dx%d\nFrame: %dx%d\n", pos.x, pos.y, width, height, frame_width, frame_height), 0, -100, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}

void Player::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(ResourceType::IMG_PLAYER);

	render->Release();
}