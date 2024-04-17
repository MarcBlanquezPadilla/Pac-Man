#include <iostream>
#include <raylib.h>
#include "Game.h"
#include "MemoryLeaks.h"
#include "Globals.h"
#include "LogMessages.h"

int main()
{
	ReportMemoryLeaks();

	Game *game;
	AppStatus status;

	LOG("Application start");
	game = new Game();
}

