#pragma once
#include "Entity.h"

#define SMALL_OBJECT_SIZE 8
#define SMALL_OBJECT_PHYSICAL_SIZE 8
#define MEDIUM_OBJECT_SIZE 16
#define MEDIUM_OBJECT_PHYSICAL_SIZE 16
#define LARGE_OBJECT_SIZE 32
#define LARGE_OBJECT_PHYSICAL_SIZE 32

#define POINTS_SMALL_PELET	10

enum class ObjectType { SMALL_PELET, LARGE_PELET, RIGHT_TELEPORTER, LEFT_TELEPORTER };

class Object : public Entity
{
public:
	Object(const Point& p, const Point& hp, ObjectType t, int s, int hs);
	Object(const Point& p, ObjectType t, int s);
	Object(const Point& p, ObjectType t, int sx, int sy, int hsx,int hsy);
	~Object();

	ObjectType GetObjectType();
	void DrawDebug(const Color& col) const;
	int Points() const;

private:
	ObjectType type;
};

