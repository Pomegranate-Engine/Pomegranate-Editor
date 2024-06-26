#ifndef POMEGRANATE_LIBRARY_INSPECTOR_WINDOW_H
#define POMEGRANATE_LIBRARY_INSPECTOR_WINDOW_H
#include "resources_window.h"
#include "entity_hierarchy_window.h"
#include "Imgui/misc/cpp/imgui_stdlib.h"
#include "Imgui/imgui_internal.h"
#include <unordered_set>
#include "lua.h"

struct InspectorHeaderTag
{
    std::string name;
    bool open = true;
    InspectorHeaderTag(const char* name);
};

class InspectorWindow : public EditorWindow
{
public:
    static std::string resource_name;
    static ResourceFile* resource;
    static int element_index;
    static bool something_dropped;
    static std::string component_search_buffer;
    Component* right_clicked_component = nullptr;
    static void property_field(const char* name, std::string* value);
    static void property_field(const char* name, float* value);
    static void property_field(const char* name, double* value);
    static void property_field(const char* name, int* value);
    static void property_field(const char* name, bool* value);
    static void property_field(const char* name, Vec2* value);
    static void property_field(const char* name, Vec3* value);
    static void property_field(const char* name, Color* value);
    static void property_field(const char* name, Texture** value);
    static void property_field(const char* name, TTFFont** value);
    static void property_field(const char* name, Entity** value);
    static void property_field(const char* name, LuaComponentData** value);
    static void property_field(const char* name, LuaSystemScript** value);
    static std::string get_element(std::string name);

    InspectorWindow();
    void render() override;
    void update() override;

    static void focus();
};


#endif //POMEGRANATE_LIBRARY_INSPECTOR_WINDOW_H
