#pragma once
#include "raylib.h"
#include "Globals.h"
#include <unordered_map>
#include <string>

enum class ResourceType {
    IMG_PLAYER,
    IMG_MAP,
    IMG_ITEMS,
    IMG_PUNTUATIONS,
    IMG_LETTERS,
    IMG_GHOST
};

class ResourceManager {
public:
    //Singleton instance retrieval
    static ResourceManager& Instance()
    {
        static ResourceManager instance; //Guaranteed to be initialized only once
        return instance;
    }

    //Load and unload texture
    AppStatus LoadTexture(ResourceType id, const std::string& file_path);
    void ReleaseTexture(ResourceType id);

    //Get texture by key
    const Texture2D* GetTexture(ResourceType id) const;

    //Release resources
    void Release();

    //Ensure Singleton cannot be copied or assigned
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

private:
    //Private constructor to prevent instantiation
    ResourceManager();
    //Destructor
    ~ResourceManager();

    //Dictionary to store loaded textures
    std::unordered_map<ResourceType, Texture2D> textures;
};