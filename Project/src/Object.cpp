#include "Object.h"
#include "StaticImage.h"
#include "LogMessages.h"
#include "ResourceManager.h"

Object::Object(const Point& p, ObjectType t, int objectSize) : Entity(p, objectSize, objectSize, objectSize, objectSize)
{
	type = t;

	Rectangle rc;
	const int n = TILE_SIZE;
	switch (type)
	{
	case ObjectType::SMALL_PELET: rc = { 34 * n, 0, (float)objectSize, (float)objectSize }; break;
	case ObjectType::LARGE_PELET: rc = { 32 * n, 0, (float)objectSize, (float)objectSize }; break;
	case ObjectType::LEFT_TELEPORTER: rc = { 35 * n, 0, (float)objectSize, (float)objectSize }; break;
	case ObjectType::RIGHT_TELEPORTER: rc = { 35 * n, 0, (float)objectSize, (float)objectSize }; break;

	default: LOG("Internal error: object creation of invalid type");
	}

	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(ResourceType::IMG_ITEMS), rc); //REVISAR IMG_MAP
}
Object::Object(const Point& p, ObjectType t, int sizeX, int sizeY, int hitBoxSizeX, int hitBoxSizeY) : Entity(p, hitBoxSizeX, hitBoxSizeY, sizeX, sizeY)
{
	type = t;

	Rectangle rc;
	const int n = TILE_SIZE;
	switch (type)
	{
	case ObjectType::SMALL_PELET: rc = { 34 * n, 0, (float)sizeX, (float)sizeY }; break;
	case ObjectType::LARGE_PELET: rc = { 32 * n, 0, (float)sizeX, (float)sizeY }; break;
	case ObjectType::LEFT_TELEPORTER: rc = { 35 * n, 0, (float)sizeX, (float)sizeY }; break;
	case ObjectType::RIGHT_TELEPORTER: rc = { 35 * n, 0, (float)sizeX, (float)sizeY }; break;

	default: LOG("Internal error: object creation of invalid type");
	}

	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(ResourceType::IMG_ITEMS), rc); //REVISAR IMG_MAP
}
Object::Object(const Point& pos, const Point& hitBoxPosition, ObjectType t, int objectSize, int hitBoxSize) : Entity(pos,hitBoxPosition, hitBoxSize, hitBoxSize, objectSize, objectSize)
{
	type = t;

	Rectangle rc;
	const int n = TILE_SIZE;
	switch (type)
	{
	case ObjectType::SMALL_PELET: rc = { 34 * n, 0, (float)objectSize, (float)objectSize }; break;
	case ObjectType::LARGE_PELET: rc = { 32 * n, 0, (float)objectSize, (float)objectSize }; break;
	case ObjectType::LEFT_TELEPORTER: rc = { 35 * n, 0, (float)objectSize, (float)objectSize }; break;
	case ObjectType::RIGHT_TELEPORTER: rc = { 35 * n, 0, (float)objectSize, (float)objectSize }; break;

	default: LOG("Internal error: object creation of invalid type");
	}

	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(ResourceType::IMG_ITEMS), rc); //REVISAR IMG_MAP
}
Object::~Object()
{
}
ObjectType Object::GetObjectType()
{
	return type;
}
void Object::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(col);
}
int Object::Points() const
{
	if (type == ObjectType::SMALL_PELET) return POINTS_SMALL_PELET;
	else if (type == ObjectType::LARGE_PELET) {}
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}