#pragma once
#include "Ghost.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include "LogMessages.h"

class Clyde : public Ghost
{
public:
	Clyde(const Point& p, GhostState s, GhostLook view);
	AppStatus Initialise() override;
};