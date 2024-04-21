#pragma once
#include <raylib.h>
#include "Player.h"
#include "TileMap.h"
#include "Object.h"
#include "Globals.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Inky.h"
#include "Clyde.h"

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };

class Scene
{
public:
    Scene();
    ~Scene();

    AppStatus Init();
    void Update();
    void Render();
    void Release();
    bool GetReturnMainMenu();

private:
    AppStatus LoadLevel(int stage);

    void CheckCollisions();
    void ClearLevel();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;

    Player* player;
    Blinky* blinky;
    Pinky* pinky;
    Inky* inky;
    Clyde* clyde;
    TileMap* level;
    std::vector<Object*> objects;
    int peletsCollected;
    int munch;
    bool returnMainMenu;
    Sound startMusic;

    Camera2D camera;
    DebugMode debug;
};
