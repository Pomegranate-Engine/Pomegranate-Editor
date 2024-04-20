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
using namespace Pomegranate;

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
#include <vulkan/vulkan.hpp>

//Main window
Window main_window = Window("Pomegranate Editor", 1024, 720);
const std::string VERSION = "0.0.1";

#define EDITOR_MODE

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
    float speed = 1.0;
    Entity* target = nullptr;
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
    void draw(Pomegranate::Entity *e) override
    {
#ifdef EDITOR_MODE
        if(e->has_component<Sprite>() && e->has_component<Transform>())
        {
            //Draw bounding box
            auto* sprite = e->get_component<Sprite>();
            auto* transform = e->get_component<Transform>();
            SDL_FRect rect = {transform->pos.x, transform->pos.y, (float)sprite->texture->get_size().x*transform->scale.x,(float)sprite->texture->get_size().y*transform->scale.y};
            //Adjust for camera zoom position and stuff
            rect.x -= Camera::current->get_component<Transform>()->pos.x;
            rect.y -= Camera::current->get_component<Transform>()->pos.y;
            rect.x *= Camera::current->get_component<Camera>()->zoom;
            rect.y *= Camera::current->get_component<Camera>()->zoom;
            rect.w *= Camera::current->get_component<Camera>()->zoom;
            rect.h *= Camera::current->get_component<Camera>()->zoom;
            rect.x -= rect.w/2;
            rect.y -= rect.h/2;
            //Offset to sdl_renderer center
            int screen_w = 0;
            int screen_h = 0;
            float render_scale_x = 1.0;
            float render_scale_y = 1.0;
            SDL_GetRenderScale(Window::current->get_sdl_renderer(), &render_scale_x, &render_scale_y);
            SDL_GetCurrentRenderOutputSize(Window::current->get_sdl_renderer(), &screen_w, &screen_h);
            screen_w /= render_scale_x;
            screen_h /= render_scale_y;
            rect.x += screen_w/2;
            rect.y += screen_h/2;
            SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 255, 0, 0, 255);
            SDL_RenderRect(Window::current->get_sdl_renderer(), &rect);
        }
#endif
    }
};
class Transform3D : public Component
{
public:
    Vec3 pos;
    Vec3 scale;
    Vec3 rotation;
    void init(Entity* e) override
    {
        pos = Vec3();
        scale = Vec3(1.0,1.0,1.0);
        rotation = Vec3();
        register_component(Transform3D);
        push_data<Vec3>("pos", &pos);
        push_data<Vec3>("scale", &scale);
        push_data<Vec3>("rotation", &rotation);
    }
};

class CubeComponent : public Component
{
public:
    float size;
    void init(Entity* e) override
    {
        size = 1.0;
        e->require_component<Transform3D>();
        register_component(CubeComponent);
        push_data<float>("size", &size);
    }
};

class Render3D : public System
{
public:
    Mat projection;
    Mat view;
    Render3D()
    {
        projection = Mat::perspective(1.57, (float)Window::current->get_width()/(float)Window::current->get_height(), 0.01, 1000.0);
        view = Mat::look_at(Vec3(0,0,0),Vec3(0,0,1),Vec3(0,1,0));
    }
    void draw(Entity* e) override
    {
        if(e->has_component<Transform3D>() && e->has_component<CubeComponent>())
        {
            print_info("Drawing cube");
            auto* transform = e->get_component<Transform3D>();
            auto* cube = e->get_component<CubeComponent>();
            //Calculate cube vertices
            Vec3 vertices[8] = {
                    Vec3(-cube->size/2,-cube->size/2,-cube->size/2),
                    Vec3(cube->size/2,-cube->size/2,-cube->size/2),
                    Vec3(cube->size/2,cube->size/2,-cube->size/2),
                    Vec3(-cube->size/2,cube->size/2,-cube->size/2),
                    Vec3(-cube->size/2,-cube->size/2,cube->size/2),
                    Vec3(cube->size/2,-cube->size/2,cube->size/2),
                    Vec3(cube->size/2,cube->size/2,cube->size/2),
                    Vec3(-cube->size/2,cube->size/2,cube->size/2)
            };
            //Adjust based on transform
            for(int i = 0; i < 8; i++)
            {
                vertices[i] = vertices[i] * transform->scale;
                //Calculate angle (float) and axis for rotation
                Vec3 axis = Vec3(1,0,0);
                float angle = transform->rotation.x;
                vertices[i] = vertices[i].rotate(angle, axis);
                axis = Vec3(0,1,0);
                angle = transform->rotation.y;
                vertices[i] = vertices[i].rotate(angle, axis);
                axis = Vec3(0,0,1);
                angle = transform->rotation.z;
                vertices[i] = vertices[i].rotate(angle, axis);
                vertices[i] = vertices[i] + transform->pos;
                Vec4 v = Vec4(vertices[i].x, vertices[i].y, vertices[i].z, 1.0);
                //Adjust for matricies
                v = (projection*view)*v;
                vertices[i] = Vec3(v.x/v.w, v.y/v.w, v.z/v.w);
                //Adjust for screen

                int renderer_width = 0;
                int renderer_height = 0;
                SDL_GetCurrentRenderOutputSize(Window::current->get_sdl_renderer(), &renderer_width, &renderer_height);

                vertices[i].x *= (float)renderer_width/2;
                vertices[i].y *= (float)renderer_height/2;
                vertices[i].x += (float)renderer_width/2;
                vertices[i].y += (float)renderer_height/2;
            }
            //Draw lines
            SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 255, 255, 255, 255);
            for(int i = 0; i < 8; i++)
            {
                //Draw point
                SDL_RenderPoint(Window::current->get_sdl_renderer(), vertices[i].x, vertices[i].y);
            }
            //Draw lines
            for(int i = 0; i < 4; i++)
            {
                SDL_RenderLine(Window::current->get_sdl_renderer(), vertices[i].x, vertices[i].y, vertices[(i+1)%4].x, vertices[(i+1)%4].y);
                SDL_RenderLine(Window::current->get_sdl_renderer(), vertices[i+4].x, vertices[i+4].y, vertices[((i+1)%4)+4].x, vertices[((i+1)%4)+4].y);
                SDL_RenderLine(Window::current->get_sdl_renderer(), vertices[i].x, vertices[i].y, vertices[i+4].x, vertices[i+4].y);
            }
        }
    }
};

int main(int argc, char* argv[])
{
    //init vulkan
    VkInstance instance;
    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Pomegranate";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "Pomegranate";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;
    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;
    if(vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS)
    {
        print_error("Failed to create vulkan instance");
        return -1;
    }
    //region init
    pomegranate_init(); //Init
    ImGui::CreateContext(); //Create imgui context
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
    io.Fonts->AddFontFromFileTTF("engine/zed_font.ttf", 18.0f);

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

    //Register systems
    register_system(PlayerController);
    register_system(Render);
    register_system(RigidBody);
    register_system(KinematicBody);
    register_system(CameraController);
    register_system(EditorDebug);
    register_system(LuaSystem);
    register_system(Render3D);

    Editor::current_scene = new EntityGroup("root");

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
        //Draw imgui
        ImGui::Render();

        // Main rendering function

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
