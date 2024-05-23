#ifndef POMEGRANATE_LIBRARY_RESOURCES_WINDOW_H
#define POMEGRANATE_LIBRARY_RESOURCES_WINDOW_H
#include "editor_window.h"
#include "Pomegranate/resource_manager.h"
#include "Pomegranate/ttf_font.h"
#include "Pomegranate/texture.h"
#include "Pomegranate/audio.h"
#include "lua.h"
#include "theme.h"
#include "live_share.h"

//Include file/directory stuff
#include <filesystem>
#include <iostream>
#include <string>
#include<vector>

#include "Imgui/misc/cpp/imgui_stdlib.h"

#include "json.hpp"

#include "scene.h"

using json = nlohmann::json;

using namespace Pomegranate;

enum class ResourceType
{
    RESOURCE_TYPE_TEXTURE,
    RESOURCE_TYPE_AUDIO,
    RESOURCE_TYPE_LUA_SCRIPT,
    RESOURCE_TYPE_FONT,
    RESOURCE_TYPE_SHADER,
    RESOURCE_TYPE_MESH,
    RESOURCE_TYPE_MATERIAL,
    RESOURCE_TYPE_SCENE,
    RESOURCE_TYPE_PREFAB,
    RESOURCE_TYPE_SPRITE_SHEET,
    RESOURCE_TYPE_ANIMATION,
    RESOURCE_TYPE_TILEMAP,
    RESOURCE_TYPE_TILESET,
    RESOURCE_TYPE_RESOURCE_FILE
};

struct ResourceTag
{
public:
    std::string name;
    Color color;
    uint32_t priority;
    std::vector<ResourceTag> child_tags;
    ResourceTag(std::string name, Color color, int priority);
    void add_child_tag(ResourceTag tag);
};

class ResourceFile
{
public:
    Texture* icon;
    std::string path;
    ResourceType type;
    Color get_color();
    std::vector<ResourceTag> tags;
    ResourceFile(std::string path, ResourceType type, std::vector<ResourceTag> tags, Texture* icon);
};

class ResourcesWindow : public EditorWindow
{
public:
    static std::string search;
    static bool searching;
    static std::vector<std::string> search_tags;
    static std::vector<ResourceFile> resource_files;
    static void add_resource_file(ResourceFile file);
    float time_since_last_reload;
    Resource* selected_resource;
    ResourcesWindow();
    void load_resources();
    void render() override;
    void update() override;
};


#endif //POMEGRANATE_LIBRARY_RESOURCES_WINDOW_H
