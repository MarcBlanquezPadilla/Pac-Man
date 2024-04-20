#include "Ghost.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include "LogMessages.h"

Ghost::Ghost(const Point& p, GhostState s, GhostLook view) :
	Entity(p, GHOST_PHYSICAL_WIDTH, GHOST_PHYSICAL_HEIGHT, GHOST_FRAME_SIZE, GHOST_FRAME_SIZE)
{
	state = s;
	look = view;
	map = nullptr;
	score = 0;
}
Ghost::~Ghost()
{

}
AppStatus Ghost::Initialise()
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
	sprite->AddKeyFrame((int)GhostAnim::WALK_RIGHT, { (float)0 * n, 0, n, n });
	sprite->AddKeyFrame((int)GhostAnim::WALK_RIGHT, { (float)1 * n, 0, n, n });
	sprite->SetAnimationDelay((int)GhostAnim::WALK_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::WALK_LEFT, { (float)2 * n, 0, n, n });
	sprite->AddKeyFrame((int)GhostAnim::WALK_LEFT, { (float)3 * n, 0, n, n });
	sprite->SetAnimationDelay((int)GhostAnim::WALK_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::WALK_UP, { (float)4 * n, 0, n, n });
	sprite->AddKeyFrame((int)GhostAnim::WALK_UP, { (float)5 * n, 0, n, n });
	sprite->SetAnimationDelay((int)GhostAnim::WALK_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::WALK_DOWN, { (float)6 * n, 0, n, n });
	sprite->AddKeyFrame((int)GhostAnim::WALK_DOWN, { (float)7 * n, 0, n, n });

	sprite->SetAnimationDelay((int)GhostAnim::DIE, ANIM_DELAY);
	for (i = 0; i < 11; ++i)
		sprite->AddKeyFrame((int)GhostAnim::DIE, { (float)i * n, 1, n, n });

	state = GhostState::WALKING;
	look = GhostLook::RIGHT;
	SetAnimation((int)GhostAnim::WALK_RIGHT);

	return AppStatus::OK;
}
void Ghost::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Ghost::IsLookingRight() const
{
	return look == GhostLook::RIGHT;
}
bool Ghost::IsLookingLeft() const
{
	return look == GhostLook::LEFT;
}
bool Ghost::IsLookingUp() const
{
	return look == GhostLook::UP;
}
bool Ghost::IsLookingDown() const
{
	return look == GhostLook::DOWN;
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
	state = GhostState::IDLE;
	if (IsLookingRight())		SetAnimation((int)GhostAnim::WALK_RIGHT);
	else if (IsLookingLeft())	SetAnimation((int)GhostAnim::WALK_LEFT);
	else if (IsLookingUp())		SetAnimation((int)GhostAnim::WALK_UP);
	else						SetAnimation((int)GhostAnim::WALK_DOWN);
}
void Ghost::StartWalkingLeft()
{
	state = GhostState::WALKING;
	look = GhostLook::LEFT;
	SetAnimation((int)GhostAnim::WALK_LEFT);
}
void Ghost::StartWalkingRight()
{
	state = GhostState::WALKING;
	look = GhostLook::RIGHT;
	SetAnimation((int)GhostAnim::WALK_RIGHT);
}
void Ghost::StartWalkingUp()
{
	state = GhostState::WALKING;
	look = GhostLook::UP;
	SetAnimation((int)GhostAnim::WALK_UP);
}
void Ghost::StartWalkingDown()
{
	state = GhostState::WALKING;
	look = GhostLook::DOWN;
	SetAnimation((int)GhostAnim::WALK_DOWN);
}

void Ghost::Update()
{
	//Ghost doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
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

	if (look == GhostLook::RIGHT)
	{
		pos.x += GHOST_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == GhostState::WALKING) Stop();
		}
	}
	else if (look == GhostLook::LEFT)
	{
		pos.x += -GHOST_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			if (state == GhostState::WALKING) Stop();
		}
	}
	else if (look == GhostLook::UP)
	{
		pos.y += -GHOST_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallUp(box))
		{
			pos.y = prev_y;
			if (state == GhostState::WALKING) Stop();
		}
	}
	else
	{
		pos.y += GHOST_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallDown(box))
		{
			pos.y = prev_y;
			if (state == GhostState::WALKING) Stop();
		}
	}
}
void Ghost::ChangeMove()
{
	AABB box;
	int prev_x = pos.x;
	int prev_y = pos.y;
	bool collided = false;

	if (look == GhostLook::UP)
	{
		pos.y += -GHOST_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallUp(box))
		{
			pos.y = prev_y;
			collided = true;
		}
	}
	else if (look == GhostLook::DOWN)
	{
		pos.y += GHOST_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallDown(box))
		{
			pos.y = prev_y;
			collided = true;
		}
	}
	else if (look == GhostLook::RIGHT)
	{
		pos.x += GHOST_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			collided = true;
		}
	}
	else if (look == GhostLook::LEFT)
	{
		pos.x += -GHOST_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			collided = true;
		}
	}

	if (collided)
	{
		bool moveChanged = false;
		std::vector<int> valoresSeleccionados;
		bool found = false;
		for (int i = 0; i < 4 && !found; ++i) {
			int valorRandom;
			bool repetido;

			do {
				repetido = false;
				// Generar un valor random del 0 al 3
				valorRandom = GetRandomValue(0, 3);

				// Verificar si el valor ya ha sido seleccionado
				for (int valor : valoresSeleccionados) {
					if (valor == valorRandom) {
						repetido = true;
						break;
					}
				}
			} while (repetido);

			if (valorRandom == 0 && look != GhostLook::DOWN)
			{
				pos.y += -GHOST_SPEED;

				box = GetHitbox();
				if (map->TestCollisionWallUp(box))
				{
					pos.y = prev_y;
				}
				else
				{
					found = true;
					StartWalkingUp();
				}
			}
			else if (valorRandom == 1 && look != GhostLook::UP)
			{
				pos.y += GHOST_SPEED;

				box = GetHitbox();
				if (map->TestCollisionWallDown(box))
				{
					pos.y = prev_y;
				}
				else
				{
					found = true;
					StartWalkingDown();
				}
			}
			else if (valorRandom == 2 && look != GhostLook::LEFT)
			{
				pos.x += GHOST_SPEED;

				box = GetHitbox();
				if (map->TestCollisionWallRight(box))
				{
					pos.x = prev_x;
				}
				else
				{
					found = true;
					StartWalkingRight();
				}
			}
			else if (valorRandom == 3 && look != GhostLook::RIGHT)
			{
				pos.x += -GHOST_SPEED;

				box = GetHitbox();
				if (map->TestCollisionWallLeft(box))
				{
					pos.x = prev_x;
				}
				else
				{
					found = true;
					StartWalkingLeft();
				}
			}
			valoresSeleccionados.push_back(valorRandom);
		}
	}
	pos.x = prev_x;
	pos.y = prev_y;
}


void Ghost::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(GREEN);

	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18 * 16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}

void Ghost::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(ResourceType::IMG_GHOSTS);

	render->Release();
}