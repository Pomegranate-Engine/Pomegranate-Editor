#include <SDL.h>
#include <SDL_image.h>
#include <Pomegranate/pomegranate.h>
#include<Pomegranate/lua_wrapper.h>
#include<Pomegranate/standard_ecs_physics.h>
#include<Pomegranate/standard_ecs.h>
#include<Pomegranate/standard_ecs_rendering.h>
#include<imgui.h>
#include<backends/imgui_impl_sdl3.h>
#include<backends/imgui_impl_sdlrenderer3.h>
using namespace Pomegranate;

//Editor
#include "editor_window.h"
#include "windows_manager.h"
#include "entity_hierarchy_window.h"
#include "scene_window.h"
#include "inspector_window.h"
#include "resources_window.h"

//Main window
Window main_window = Window("Window", 1024, 720);

class PlayerComponent : public Component
{
public:
    float speed;
    void init(Pomegranate::Entity * e) override
    {
        speed = 1.0;
        e->require_component<Transform>();
        e->require_component<Sprite>();
        register_component(PlayerComponent);
    }
};
class PlayerController : public System
{
    void tick(Pomegranate::Entity *e) override
    {
        if(e->has_component<PlayerComponent>())
        {
            auto* player = e->get_component<PlayerComponent>();
            if (InputManager::get_key(SDL_SCANCODE_W)) {
                e->get_component<Transform>()->pos.y -= 1*player->speed;
            }
            if (InputManager::get_key(SDL_SCANCODE_S)) {
                e->get_component<Transform>()->pos.y += 1*player->speed;
            }
            if (InputManager::get_key(SDL_SCANCODE_A)) {
                e->get_component<Transform>()->pos.x -= 1*player->speed;
            }
            if (InputManager::get_key(SDL_SCANCODE_D)) {
                e->get_component<Transform>()->pos.x += 1*player->speed;
            }
        }
    }
};

int main(int argc, char* argv[])
{
    //region init
    pomegranate_init(); //Init
    ImGui::CreateContext(); //Create imgui context
    main_window.open(); //Open window
    main_window.make_current(); //Make window current
    main_window.set_icon("engine_res/pomegranate.png");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    //Set font
    io.Fonts->AddFontFromFileTTF("engine_res/zed_font.ttf", 16.0f);

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(main_window.get_sdl_window(),main_window.get_sdl_renderer());
    ImGui_ImplSDLRenderer3_Init(main_window.get_sdl_renderer());
//endregion

    //Register all pomegranate base components
    register_component(Transform);
    register_component(Sprite);
    register_component(AnimatedSprite);
    register_component(Camera);
    register_component(Tilemap);
    register_component(PhysicsObject);
    register_component(CollisionShape);
    register_component(PositionLink);
    register_component(ScaleLink);
    register_component(RotationLink);
    register_component(PlayerComponent);

    EntityGroup* scene_root = new EntityGroup("root");
    scene_root->add_system(new Render());

    //Create windows
    WindowsManager windows_manager;
    auto* entity_hierarchy = new Window_EntityHierarchy();
    entity_hierarchy->scene_root = scene_root;
    windows_manager.open_window(entity_hierarchy);
    auto* scene_view = new Window_SceneView();
    scene_view->scene_root = scene_root;
    windows_manager.open_window(scene_view);
    windows_manager.open_window(new InspectorWindow());
    windows_manager.open_window(new ResourcesWindow());
    float tick_time = 0.0;
    bool is_running = true;
    SDL_Event event;

    while (is_running)
    {
        Uint64 start = SDL_GetPerformanceCounter(); //For delta time

        //- - - - - # SDL2 Stuff # - - - - -
        InputManager::mouse_delta = Vec2();
        InputManager::mouse_scroll = Vec2();
        InputManager::mouse_moved = false;
        InputManager::mouse_scrolled = false;
        InputManager::press = -1;
        //Poll events
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            InputManager::process_event(event);
            if (event.type == SDL_EVENT_QUIT) {
                is_running = false;
            }
        }

        //- - - - - # UPDATE # - - - - -

        if (tick_time > 0.016)
        {
            tick_time = 0.0;
            //Update

        }
        windows_manager.update();
        //- - - - - # RENDERING # - - - - -
        //Clear SDL renderer
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 0, 0, 0, 255);
        SDL_RenderClear(Window::current->get_sdl_renderer());
        //Begin imgui
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();



        windows_manager.render(); //Render windows

        //Draw imgui
        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());

        SDL_RenderPresent(Window::current->get_sdl_renderer()); //Present

        //- - - - - # AFTER FRAME # - - - - -

        //Calculate delta time
        Uint64 end = SDL_GetPerformanceCounter();
        float secondsElapsed = (float)(end - start) / (float)SDL_GetPerformanceFrequency();
        delta_time = secondsElapsed;
        tick_time += delta_time;
    }
    main_window.close();
    pomegranate_quit(); //Cleanup
    return 0;
}
