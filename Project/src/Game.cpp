#include <stdio.h>
#include <raylib.h>
#include "Globals.h"
#include "Game.h"
#include "ResourceManager.h"
#include "LogMessages.h"
#include "Scene.h"

Game::Game()
{
    state = GameState::MAIN_MENU;
    scene = nullptr;
    img_menu = nullptr;

    target = {};
    src = {};
    dst = {};
}
Game::~Game()
{
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }
}
AppStatus Game::Initialise(float scale)
{
    float w, h;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;

    //Initialise window
    InitWindow((int)w, (int)h, "Pac-Man");
    InitAudioDevice();

    //Render texture initialisation, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (target.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::ERROR;
    }
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { 0, 0, w, h };

    //Load textures
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }

    //Set the target frame rate for the application
    SetTargetFPS(60);
    //Disable the escape key to quit functionality
    SetExitKey(0);

    return AppStatus::OK;
}
AppStatus Game::BeginPlay(int startPuntuation = 0, int startLives = 3, int level = 0)
{
    scene = new Scene(this, startPuntuation, startLives, level);
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
AppStatus Game::Update()
{
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if (WindowShouldClose()) return AppStatus::QUIT;

    switch (state)
    {
    case GameState::MAIN_MENU:
        if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
        if (IsKeyPressed(KEY_SPACE))
        {
            if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
            state = GameState::PLAYING;
        }
        break;

    case GameState::PLAYING:
        if (IsKeyPressed(KEY_ESCAPE)) //PROVISIONAL RETURN MAIN MENU
        {
            ReturnToMainMenu();
        }
        else if (IsKeyPressed(KEY_F2)) //PROVISIONAL RETURN MAIN MENU
        {
            scene->GoNextLevel();
        }
        else
        {
            //Game logic
            scene->Update();
        }
        break;
    }
    return AppStatus::OK;
}
void Game::Render()
{
    BeginTextureMode(target);
    ClearBackground(BLACK);

    switch (state)
    {
    case GameState::MAIN_MENU:
        DrawTexture(*img_menu, 0, 0, WHITE);
        break;

    case GameState::PLAYING:
        scene->Render();
        break;
    }

    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    EndDrawing();
 }
void Game::ReturnToMainMenu()
{
    state = GameState::MAIN_MENU;
    FinishPlay();
}
void Game::GoNextLevel(int startPuntuation, int startLives, int level)
{
    FinishPlay();
    level++;
    BeginPlay(startPuntuation, startLives, level);
}
void Game::Cleanup()
{
    UnloadResources();
    CloseWindow();
}
AppStatus Game::LoadResources()
{
    ResourceManager& data = ResourceManager::Instance();

    if (data.LoadTexture(ResourceType::IMG_MENU, "resources/sprites/MainMenu.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu = data.GetTexture(ResourceType::IMG_MENU);

    return AppStatus::OK;
}
void Game::UnloadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(ResourceType::IMG_MENU);

    UnloadRenderTexture(target);
}
void Game::FinishPlay()
{
    scene->Release();
    delete scene;
    scene = nullptr;
}