#ifndef POMEGRANATE_LIBRARY_SCENE_WINDOW_H
#define POMEGRANATE_LIBRARY_SCENE_WINDOW_H
#include "editor_window.h"
#include "Imgui/imgui.h"
#include "Pomegranate/pomegranate.h"
#include "Pomegranate/ecs.h"
#include "Pomegranate/resource_manager.h"
#include "Pomegranate/standard_ecs_rendering.h"
using namespace Pomegranate;

class Window_SceneView : public EditorWindow
{
public:
    Vec2 position;
    EntityGroup* scene_root;
    SDL_Texture* render_texture;
    Window_SceneView();
    void update() override;
    void render() override;
};


#endif //POMEGRANATE_LIBRARY_SCENE_WINDOW_H
