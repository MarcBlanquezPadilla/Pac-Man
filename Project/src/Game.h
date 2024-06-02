#pragma once
#include "Globals.h"
#include "Scene.h"

enum class GameState { MAIN_MENU, PLAYING, SETTINGS, CREDITS };

class Scene;

class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update();
    void ReturnToMainMenu();
    void GoNextLevel(int startPuntuation, int startLives, int level);
    void Render();
    void Cleanup();

private:
    AppStatus BeginPlay(int startPuntuation, int startLives, int level);
    void FinishPlay();

    AppStatus LoadResources();
    void UnloadResources();

    GameState state;
    Scene *scene;
    const Texture2D* img_menu;

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;
};
