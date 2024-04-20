#include "Ghost.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include "LogMessages.h"

#pragma once
class Blinky : public Ghost
{
public:
	Blinky(const Point& p, GhostState s, GhostLook view);
	AppStatus Initialise() override;
};