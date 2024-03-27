#ifndef POMEGRANATE_LIBRARY_SCENE_WINDOW_H
#define POMEGRANATE_LIBRARY_SCENE_WINDOW_H
#include "editor_window.h"
#include "Imgui/imgui.h"
#include "Pomegranate/pomegranate.h"
#include "Pomegranate/ecs.h"
#include "Pomegranate/resource_manager.h"
#include "Pomegranate/standard_ecs_rendering.h"
#include"scene.h"
using namespace Pomegranate;

class Window_SceneView : public EditorWindow
{
public:
    Vec2 position;

    Vec2 selected_entity_arrow_hor_pos = Vec2(0, 0);
    Vec2 selected_entity_arrow_hor_half = Vec2(0, 0);
    Vec2 selected_entity_arrow_vert_pos = Vec2(0, 0);
    Vec2 selected_entity_arrow_vert_half = Vec2(0, 0);
    Entity* entity_selected = nullptr;
    bool dragging_entity = false;
    bool dragging_entity_horizontal = false;
    bool dragging_entity_vertical = false;
    SDL_Texture* render_texture;
    Window_SceneView();
    void update() override;
    void render() override;
};


#endif //POMEGRANATE_LIBRARY_SCENE_WINDOW_H
