#pragma once
#include "Ghost.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include "LogMessages.h"

class Pinky : public Ghost
{
public:
	Pinky(const Point& p, GhostState s, GhostLook view);
	AppStatus Initialise() override;
};