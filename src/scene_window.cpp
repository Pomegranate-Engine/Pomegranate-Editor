#include "scene_window.h"

Window_SceneView::Window_SceneView()
{
    scene_root = nullptr;
    render_texture = SDL_CreateTexture(Pomegranate::Window::current->get_sdl_renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
    this->name = "Scene View";
    this->flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
}

void Window_SceneView::render() {
    //Check if render texture is the correct size
    Vec2i tex_size;
    SDL_QueryTexture(render_texture, NULL, NULL, &tex_size.x, &tex_size.y);
    if(tex_size.x != ImGui::GetWindowWidth() || tex_size.y != ImGui::GetWindowHeight())
    {
        size.x = ImGui::GetWindowWidth();
        size.y = ImGui::GetWindowHeight();
        SDL_DestroyTexture(render_texture);
        render_texture = SDL_CreateTexture(Pomegranate::Window::current->get_sdl_renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
    }
    if(scene_root != nullptr)
    {
        SDL_SetRenderTarget(Pomegranate::Window::current->get_sdl_renderer(), render_texture);
        SDL_SetRenderDrawColor(Pomegranate::Window::current->get_sdl_renderer(), 0, 0, 0, 255);
        SDL_RenderClear(Pomegranate::Window::current->get_sdl_renderer());
        //Create camera entity
        Entity* camera = new Entity();
        camera->add_component<Camera,Transform>();
        Camera::make_current(camera);

        camera->get_component<Transform>()->pos = this->position;

        //Render scene
        scene_root->draw(nullptr);
        //Destroy camera entity
        camera->force_destroy();
        SDL_SetRenderTarget(Pomegranate::Window::current->get_sdl_renderer(), nullptr);
    }

    //Move camera
    if(ImGui::IsWindowFocused() && InputManager::get_mouse_button(1))
    {
        this->position -= InputManager::mouse_delta;
    }
    ImGui::SetCursorPos(ImVec2(0,0));
    ImGui::Image(render_texture, ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()));
    //Open context menu
    if(ImGui::BeginPopupContextWindow())
    {
        if(ImGui::MenuItem("Reset Camera"))
        {
            this->position = Vec2();
        }
        ImGui::EndPopup();
    }
}

void Window_SceneView::update() {

}
