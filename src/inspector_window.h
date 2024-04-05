#ifndef POMEGRANATE_LIBRARY_INSPECTOR_WINDOW_H
#define POMEGRANATE_LIBRARY_INSPECTOR_WINDOW_H
#include "entity_hierarchy_window.h"
#include "Imgui/misc/cpp/imgui_stdlib.h"
#include "Imgui/imgui_internal.h"
#include <unordered_set>

class InspectorWindow : public EditorWindow
{
public:
    static int element_index;
    Component* right_clicked_component = nullptr;
    static void property_field(const char* name, std::string* value);
    static void property_field(const char* name, float* value);
    static void property_field(const char* name, int* value);
    static void property_field(const char* name, bool* value);
    static void property_field(const char* name, Vec2* value);
    static void property_field(const char* name, Vec3* value);
    static void property_field(const char* name, Color* value);
    static void property_field(const char* name, Texture** value);
    static void property_field(const char* name, TTFFont** value);
    static void property_field(const char* name, Entity** value);
    static std::string get_element(std::string name);

    InspectorWindow();
    void render() override;
    void update() override;

    static void focus();
};


#endif //POMEGRANATE_LIBRARY_INSPECTOR_WINDOW_H
