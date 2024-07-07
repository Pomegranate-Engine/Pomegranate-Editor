#include "run.h"

std::vector<ProjectDebugger*> ProjectDebugger::debuggers = std::vector<ProjectDebugger*>();

ProjectDebugger::ProjectDebugger()
{
    texture = SDL_CreateTexture(Window::current->get_sdl_renderer(),SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,720,512);
    debuggers.push_back(this);
}

void ProjectDebugger::tick()
{
    SDL_SetRenderTarget(Window::current->get_sdl_renderer(),texture);
    Camera::current_render_position = Vec2(0,0);
    Camera::current_render_zoom = 1.0f;
    SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), EditorTheme::scene_view_background.x,EditorTheme::scene_view_background.y,EditorTheme::scene_view_background.z, 255);
    SDL_RenderClear(Window::current->get_sdl_renderer());
    Editor::current_scene->tick();
    Editor::current_scene->draw(render_default_sort);
    SDL_SetRenderTarget(Window::current->get_sdl_renderer(),nullptr);

    ImGui::SetNextWindowSize(ImVec2(720,512));
    ImGui::Begin("Play");
    ImGui::SetCursorPos(ImVec2(0,0));
    ImGui::Image(texture, ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()));
    ImGui::End();
}

void ProjectDebugger::tick_debuggers()
{
    for(auto & debugger : debuggers)
    {
        debugger->tick();
    }
}


