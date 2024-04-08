#include "SDL.h"
#include "Pomegranate/pomegranate.h"
#include"lua.h"
#include"Pomegranate/standard_ecs_physics.h"
#include"Pomegranate/standard_ecs.h"
#include"Pomegranate/standard_ecs_rendering.h"
#include"Imgui/imgui.h"
#include"Imgui/backends/imgui_impl_sdl3.h"
#include"Imgui/backends/imgui_impl_sdlrenderer3.h"
using namespace Pomegranate;

#include "scene.h"

//[/COMPONENTS_SYSTEMS_INCLUDE]

class PlayerComponent : public Component
{
public:
    float speed;
    float deceleration;
    Vec2 velocity;
    void init(Pomegranate::Entity * e) override
    {
        speed = 1.0;
        deceleration = 0.1;
        velocity = Vec2();
        e->require_component<Transform>();
        e->require_component<Sprite>();
        register_component(PlayerComponent);
        push_data<float>("speed", &speed);
        push_data<float>("deceleration", &deceleration);
        push_data<Vec2>("velocity", &velocity);
    }
};
class PlayerController : public System
{
    void tick(Pomegranate::Entity *e) override
    {
        if(e->has_component<LuaComponent>())
        {
            auto* player = e->get_component<LuaComponent>()->get_component("PlayerComponent");
            auto* transform = e->get_component<Transform>();
            Vec2 v = player->get<Vec2>("velocity");
            if (InputManager::get_key(SDL_SCANCODE_W)) {
                v.y -= 1*player->get<double>("speed");
            }
            if (InputManager::get_key(SDL_SCANCODE_S)) {
                v.y += 1*player->get<double>("speed");
            }
            if (InputManager::get_key(SDL_SCANCODE_A)) {
                v.x -= 1*player->get<double>("speed");
            }
            if (InputManager::get_key(SDL_SCANCODE_D)) {
                v.x += 1*player->get<double>("speed");
            }
            float deceleration = player->get<double>("deceleration");
            v -= v * deceleration*delta_time;
            transform->pos += v;
            player->set("velocity",v);
        }
    }
};
class CameraFollow : public Component
{
public:
    float speed;
    Entity* target;
    void init(Entity* e) override
    {
        speed = 1.0;
        target = nullptr;
        e->require_component<Transform>();
        e->require_component<Camera>();
        register_component(CameraFollow);
        push_data<float>("speed", &speed);
        push_data<Entity*>("target", &target);
    }
};
class CameraController : public System
{
public:
    void tick(Entity* e) override
    {
        if(e->has_component<CameraFollow>())
        {
            auto* camera_follow = e->get_component<CameraFollow>();
            auto* transform = e->get_component<Transform>();
            if(camera_follow->target)
            {
                if(camera_follow->target->has_component<Transform>())
                {
                    auto* target_transform = camera_follow->target->get_component<Transform>();
                    transform->pos = transform->pos + (target_transform->pos - transform->pos) * camera_follow->speed * delta_time;
                }
            }
        }
    }
};

class EditorDebug : public System
{
public:
    void draw(Pomegranate::Entity *) override
    {
#ifdef EDITOR_MODE

#endif
    }
};

//Main window
const char* game_name = "[/GAME_NAME]";
const char* scene_path = "[/SCENE_PATH]";
Window main_window = Window(game_name, 1024, 720);



int main(int argc, char* argv[])
{
    //region init
    pomegranate_init(); //Init
    ImGui::CreateContext(); //Create imgui context
    main_window.open(); //Open window
    main_window.make_current(); //Make window current
    main_window.set_icon(game_name);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    //Set default font to engine/zed_font.ttf
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
    register_component(LuaComponent);


    //Register systems
    register_system(Render);
    register_system(RigidBody);
    register_system(KinematicBody);
    register_system(EditorDebug);

    //[/COMPONENTS_SYSTEMS_REGISTER]
    register_component(PlayerComponent);
    register_system(PlayerController);
    register_system(CameraController);
    register_component(CameraFollow);

    Editor::current_scene = open_scene(scene_path);

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
        print_info("Ticking");
        if (tick_time > 0.016)
        {
            tick_time = 0.0;
            //Update
            Editor::current_scene->tick();
        }
        print_info("Ticked");
        //- - - - - # RENDERING # - - - - -
        //Clear SDL renderer
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 0, 0, 0, 255);
        SDL_RenderClear(Window::current->get_sdl_renderer());
        //Begin imgui
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        print_info("Drawing");
        Editor::current_scene->draw(nullptr); //Render scene
        print_info("Drew");
        //Draw imgui
        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());

        SDL_RenderPresent(Window::current->get_sdl_renderer()); //Present

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