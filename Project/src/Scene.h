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
#include "NavMesh.h"
#include "Puntuation.h"

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SPRITES_AND_NAVMESH_ROUTES, SPRITES_AND_NAVMESH, SIZE };

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
    void UpdateGhostState();
    void PlaySounds();
    void StopSoundsInException(const Sound* s);
    void ShowPuntuation(Point position, Puntuations puntuation);
    void EatGhostPuntuation(Point position);
    void ClearLevel();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;

    //REFERENCES
    Player* player;
    Blinky* blinky;
    Pinky* pinky;
    Inky* inky;
    Clyde* clyde;
    TileMap* level;
    NavMesh* navMesh;
    Puntuation* puntuation1;
    Puntuation* puntuation2;
    std::vector<Object*> objects;

    //VARIABLE
    int peletsCollected;
    int totalPelets;
    int munch;
    bool returnMainMenu;
    bool started;
    GhostState ghostState;
    float lastStateChangeTime;
    float timer;
    int ghostEaten;

    //SOUNDS
    const Sound* startMusic;
    const Sound* retreating;
    const Sound* power_pellet;
    const Sound* siren_1;
    const Sound* siren_2;
    const Sound* siren_3;
    const Sound* siren_4;
    const Sound* siren_5;
    const Sound* munch_1;
    const Sound* munch_2;
    const Sound* eat_ghost;

    Camera2D camera;
    DebugMode debug;
};
