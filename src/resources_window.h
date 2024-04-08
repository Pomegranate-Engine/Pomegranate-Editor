#ifndef POMEGRANATE_LIBRARY_RESOURCES_WINDOW_H
#define POMEGRANATE_LIBRARY_RESOURCES_WINDOW_H
#include "editor_window.h"
#include "Pomegranate/resource_manager.h"
#include "Pomegranate/ttf_font.h"
#include "Pomegranate/texture.h"
#include "Pomegranate/audio.h"
#include "lua.h"

//Include file/directory stuff
#include <filesystem>
#include <iostream>
#include <string>
#include<vector>

using namespace Pomegranate;
class ResourcesWindow : public EditorWindow
{
public:
    float time_since_last_reload;
    Resource* selected_resource;
    ResourcesWindow();
    void load_resources();
    void render() override;
    void update() override;
};


#endif //POMEGRANATE_LIBRARY_RESOURCES_WINDOW_H
