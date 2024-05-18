#include <SDL.h>
#include <SDL_image.h>
#include <Pomegranate/pomegranate.h>
#include"lua.h"
#include<Pomegranate/standard_ecs_physics.h>
#include<Pomegranate/standard_ecs.h>
#include<Pomegranate/standard_ecs_rendering.h>
#include<imgui.h>
#include<backends/imgui_impl_sdl3.h>
#include<backends/imgui_impl_sdlrenderer3.h>
#include<backends/imgui_impl_opengl3.h>
#include<enet/enet.h>

using namespace Pomegranate;

//Pomegranate Extensions
#include "audio.h"

//Editor
#include "editor_window.h"
#include "windows_manager.h"
#include "entity_hierarchy_window.h"
#include "scene_window.h"
#include "inspector_window.h"
#include "resources_window.h"
#include "menu_bar.h"
#include "theme.h"
#include "notifications.h"
#include "hotkey_manager.h"
#include "scene.h"
#include "extension_handler.h"

//Main window
Window main_window = Window("Pomegranate Editor", 1024, 720);
const std::string VERSION = "0.0.1";

#define EDITOR_MODE


#include "Components/components.cpp"

Mat create_transform_matrix(Vec3 position, Vec3 rotation, Vec3 scale)
{
    Mat translation = Mat::translate(position);
    Mat rotation_x = Mat::rotate(rotation.x, Vec3(1.0f, 0.0f, 0.0f));
    Mat rotation_y = Mat::rotate(rotation.y, Vec3(0.0f, 1.0f, 0.0f));
    Mat rotation_z = Mat::rotate(rotation.z, Vec3(0.0f, 0.0f, 1.0f));
    Mat scale_m = Mat::scale(scale);
    return translation * rotation_z * rotation_y * rotation_x * scale_m;
}

int main(int argc, char* argv[])
{
    print_info("Pomegranate Editor - Version: " + VERSION);
    //Print working directory
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    print_info("Working directory: " + std::string(cwd));
    //region init
    pomegranate_init(); //Init
    main_window.open(); //Open window
    main_window.make_current(); //Make window current
    main_window.set_icon("engine/pomegranate.png");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    //Set font
    io.Fonts->AddFontDefault();
    io.Fonts->AddFontFromFileTTF("engine/zed_font.ttf", 18.0f);
    io.Fonts->Build();

    EditorTheme::load("engine/theme.json");

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
    register_component(CameraFollow);
    register_component(LuaComponent);
    register_component(CubeComponent);
    register_component(Transform3D);
    register_component(ParticleEmitter);

    //Register systems
    register_system(PlayerController);
    register_system(Render);
    register_system(RigidBody);
    register_system(KinematicBody);
    register_system(CameraController);
    register_system(EditorDebug);
    register_system(LuaSystem);
    register_system(Render3D);
    register_system(ParticleSystem);


    Editor::current_scene = create_default_scene();


    //Create windows
    WindowsManager windows_manager;
    auto* entity_hierarchy = new Window_EntityHierarchy();
    windows_manager.open_window(entity_hierarchy);
    auto* scene_view = new Window_SceneView();
    windows_manager.open_window(scene_view);
    windows_manager.open_window(new InspectorWindow());
    windows_manager.open_window(new ResourcesWindow());
    float tick_time = 0.0;
    bool is_running = true;
    float time = 0;
    SDL_Event event;

    while (is_running)
    {
        Uint64 start = SDL_GetPerformanceCounter(); //For delta time
        main_window.set_title(("Pomegranate Editor - " + VERSION + std::string(" - Editing: ") + std::string(Editor::current_scene_path)).c_str()); //Set window title
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
        HotkeyManager::tick();
        windows_manager.update();
        LiveShare::update();
        //- - - - - # RENDERING # - - - - -
        //Clear SDL renderer
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 0, 0, 0, 255);
        SDL_RenderClear(Window::current->get_sdl_renderer());

        //Begin imgui
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        draw_menu_bar();
        windows_manager.render(); //Render windows
        Notify::render(delta_time);
        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(Window::current->get_sdl_renderer()); //Present
        //- - - - - # AFTER FRAME # - - - - -

        //Calculate delta time
        Uint64 end = SDL_GetPerformanceCounter();
        float secondsElapsed = (float)(end - start) / (float)SDL_GetPerformanceFrequency();
        delta_time = secondsElapsed;
        tick_time += delta_time;
        time += delta_time;
    }
    main_window.close();
    pomegranate_quit(); //Cleanup
    LiveShare::stop_server();
    return 0;
}
