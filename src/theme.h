#ifndef POMEGRANATE_ENGINE_THEME_H
#define POMEGRANATE_ENGINE_THEME_H
#include "json.hpp"
#include<fstream>
#include "imgui.h"
#include "Pomegranate/pomegranate.h"
using namespace Pomegranate;
using json = nlohmann::json;

struct EditorTheme
{
    static float rounding;
    static Vec3 editor_text;
    static Vec3 editor_title_bar;
    static Vec3 editor_elements;
    static Vec3 editor_inputs;
    static Vec3 editor_hover;
    static Vec3 editor_popups;
    static Vec3 editor_body;
    static Vec3 scene_view_y;
    static Vec3 scene_view_x;
    static Vec3 scene_view_background;
    static Vec3 scene_hierarchy_entity;
    static Vec3 scene_hierarchy_group;
    static Vec3 scene_hierarchy_system;
    static Vec3 color_palette_red;
    static Vec3 color_palette_yellow;
    static Vec3 color_palette_green;
    static Vec3 color_palette_blue;
    static Vec3 color_palette_purple;
    static Vec3 color_palette_white;
    static void load(const char *path);
};

#endif
