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

//Main window
Window main_window = Window("Pomegranate Editor", 1024, 720);
const std::string VERSION = "0.0.1";

#define EDITOR_MODE

#ifdef USE_OPENGL
#include<glew/glew.h>
#include<GL/GL.h>

#include"gl/shader.h"
#include"gl/mesh.h"
#endif

#include "components.cpp"

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

#ifdef USE_OPENGL
    //OpenGL
    SDL_GLContext gl_context = SDL_GL_CreateContext(main_window.get_sdl_window());
    //Set version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //Load default shader
    auto* shader = ResourceManager::load<Shader>("res/default.glsl");

    //Generate perspective and lookat matrices
    Mat projection = Mat::perspective(90.0f, (float)main_window.get_width() / (float)main_window.get_height(), 0.1f, 100.0f);


    //Generate cube mesh
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Vec3> face_normals;

    //vertices
    vertices.push_back({Vec3(-1.0f, -1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec2(0.0f, 0.0f), Vec4(1.0f, 0.0f, 0.0f, 1.0f)});
    vertices.push_back({Vec3(1.0f, -1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec2(1.0f, 0.0f), Vec4(0.0f, 1.0f, 0.0f, 1.0f)});
    vertices.push_back({Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec2(1.0f, 1.0f), Vec4(0.0f, 0.0f, 1.0f, 1.0f)});
    vertices.push_back({Vec3(-1.0f, 1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec2(0.0f, 1.0f), Vec4(1.0f, 1.0f, 0.0f, 1.0f)});
    vertices.push_back({Vec3(-1.0f, -1.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec2(0.0f, 0.0f), Vec4(1.0f, 0.0f, 0.0f, 1.0f)});
    vertices.push_back({Vec3(1.0f, -1.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec2(1.0f, 0.0f), Vec4(0.0f, 1.0f, 0.0f, 1.0f)});
    vertices.push_back({Vec3(1.0f, 1.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec2(1.0f, 1.0f), Vec4(0.0f, 0.0f, 1.0f, 1.0f)});
    vertices.push_back({Vec3(-1.0f, 1.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec2(0.0f, 1.0f), Vec4(1.0f, 1.0f, 0.0f, 1.0f)});

    //indices
    indices = {
            0, 1, 2,
            2, 3, 0,

            1, 5, 6,
            6, 2, 1,

            7, 6, 5,
            5, 4, 7,

            4, 0, 3,
            3, 7, 4,

            4, 5, 1,
            1, 0, 4,

            3, 2, 6,
            6, 7, 3
    };

    //face normals
    face_normals = {
            Vec3(0.0f, 0.0f, 1.0f),
            Vec3(0.0f, 0.0f, 1.0f),
            Vec3(0.0f, 0.0f, 1.0f),
            Vec3(0.0f, 0.0f, 1.0f),
            Vec3(0.0f, 0.0f, 1.0f),
            Vec3(0.0f, 0.0f, 1.0f),
            Vec3(0.0f, 0.0f, 1.0f),
            Vec3(0.0f, 0.0f, 1.0f)
    };

    Mesh mesh = Mesh(vertices,indices,face_normals);
#endif

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
        //- - - - - # RENDERING # - - - - -

#ifdef USE_OPENGL
        glViewport(0,0,main_window.get_width(),main_window.get_height());
        glClearColor(0.0,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        //draw a triangle
        //shader->set<Vec4>("color", Vec4(0.0f, 1.0f, 1.0f, 1.0f));
        //shader->set<Texture*>("tex", ResourceManager::load<Texture>("engine/pomegranate.png"));
        Mat view = Mat::look_at(Vec3(0.0f,0.0f,1.0f),Vec3(0.0f,0.0f,-10.0f),Vec3(0.0f,1.0f,0.0f));
        shader->use();
        shader->set<Mat>("projection", projection);
        shader->set<Mat>("view", view);
        shader->set<Mat>("model", create_transform_matrix(Vec3(cos(time)*2,sin(time)*2,-5.0f),Vec3(0.0f,-time,time),Vec3(1.0f,1.0f,1.0f)));
        //Draw triangle with uvs
        mesh.draw();
        SDL_GL_SwapWindow(main_window.get_sdl_window());
#endif

#ifndef USE_OPENGL
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
#endif

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
    return 0;
}
