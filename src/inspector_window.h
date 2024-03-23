#ifndef POMEGRANATE_LIBRARY_INSPECTOR_WINDOW_H
#define POMEGRANATE_LIBRARY_INSPECTOR_WINDOW_H
#include "entity_hierarchy_window.h"
#include "Imgui/misc/cpp/imgui_stdlib.h"

class InspectorWindow : public EditorWindow
{
public:
    static void property_field(const char* name, std::string* value);
    static void property_field(const char* name, float* value);
    static void property_field(const char* name, int* value);
    static void property_field(const char* name, bool* value);
    static void property_field(const char* name, Vec2* value);
    static void property_field(const char* name, Vec3* value);
    static void property_field(const char* name, Color* value);
    static void property_field(const char* name, Texture** value);
    InspectorWindow();
    void render() override;
    void update() override;
};


#endif //POMEGRANATE_LIBRARY_INSPECTOR_WINDOW_H
