#pragma once
#include "Ghost.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include "LogMessages.h"

class Inky : public Ghost
{
public:
	Inky(const Point& p, GhostState s, GhostLook view);
	AppStatus Initialise() override;
};