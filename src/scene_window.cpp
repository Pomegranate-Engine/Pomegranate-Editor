#include "scene_window.h"

#include "entity_hierarchy_window.h"

Window_SceneView::Window_SceneView()
{
    render_texture = SDL_CreateTexture(Window::current->get_sdl_renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
    this->name = "Scene View";
    this->flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
}

void draw_bezier(Vec2 a, Vec2 b, Vec2 c, Vec2 position, Color color)
{
    //Adjust for camera position
    a -= position;
    b -= position;
    c -= position;

    //Draw bezier curve
    SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), color.r, color.g, color.b, 255);
    std::vector<SDL_FPoint> points;
    for(float t = 0; t < 1; t+=0.01)
    {
        Vec2 p = a*pow(1-t,2) + b*2*(1-t)*t + c*pow(t,2);
        points.push_back({p.x, p.y});
    }
    SDL_RenderLines(Window::current->get_sdl_renderer(), points.data(), points.size());
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
        render_texture = SDL_CreateTexture(Window::current->get_sdl_renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
    }
    if(currently_opened_scene != nullptr)
    {
        SDL_SetRenderTarget(Window::current->get_sdl_renderer(), render_texture);
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 0, 0, 0, 255);
        SDL_RenderClear(Window::current->get_sdl_renderer());
        //Create camera entity
        Entity* camera = new Entity();
        camera->add_component<Camera,Transform>();
        Camera::make_current(camera);

        camera->get_component<Transform>()->pos = this->position;

        //Axis
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 0, 255, 0, 255);
        SDL_SetRenderDrawBlendMode(Window::current->get_sdl_renderer(), SDL_BLENDMODE_BLEND);
        SDL_RenderLine(Window::current->get_sdl_renderer(), 0-this->position.x, 0, 0-this->position.x, ImGui::GetWindowHeight());
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 255, 0, 0, 255);
        SDL_RenderLine(Window::current->get_sdl_renderer(), 0, 0-this->position.y, ImGui::GetWindowWidth(), 0-this->position.y);



        //Render scene
        currently_opened_scene->draw(nullptr);

        Vec2 mouse_world_pos = InputManager::get_mouse_position()-Vec2(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y) + this->position;
        //Draw movement arrows around selected entity
        if (Node::selected != nullptr)
        {
            if(Node::selected->entity != nullptr)
            {
                if(Node::selected->entity->get_component<Transform>())
                {
                    if(entity_selected != Node::selected->entity.get())
                    {
                        selected_entity_arrow_hor_pos = Node::selected->entity->get_component<Transform>()->pos;
                        selected_entity_arrow_hor_half = Node::selected->entity->get_component<Transform>()->pos;
                        selected_entity_arrow_vert_pos = Node::selected->entity->get_component<Transform>()->pos;
                        selected_entity_arrow_vert_half = Node::selected->entity->get_component<Transform>()->pos;
                    }
                    entity_selected = Node::selected->entity.get();
                    Entity* entity = Node::selected->entity.get();
                    auto* transform = entity->get_component<Transform>();
                    SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 255, 0, 0, 255);
                    //Draw arrow
                    if(!dragging_entity_horizontal)
                        selected_entity_arrow_hor_pos = selected_entity_arrow_hor_pos.lerp(transform->pos + Vec2(64, 0),20*delta_time);
                    selected_entity_arrow_hor_half = selected_entity_arrow_hor_half.lerp(transform->pos + Vec2(32, 0),30*delta_time);
                    draw_bezier(transform->pos,selected_entity_arrow_hor_half,selected_entity_arrow_hor_pos, this->position, Color(255,0,0));
                    //Draw arrow tip
                    SDL_RenderLine(Window::current->get_sdl_renderer(), selected_entity_arrow_hor_pos.x-this->position.x, selected_entity_arrow_hor_pos.y-this->position.y, selected_entity_arrow_hor_pos.x-8-this->position.x, selected_entity_arrow_hor_pos.y-8-this->position.y);
                    SDL_RenderLine(Window::current->get_sdl_renderer(), selected_entity_arrow_hor_pos.x-this->position.x, selected_entity_arrow_hor_pos.y-this->position.y, selected_entity_arrow_hor_pos.x-8-this->position.x, selected_entity_arrow_hor_pos.y+8-this->position.y);

                    SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 0, 255, 0, 255);
                    //Draw arrow
                    if(!dragging_entity_vertical)
                        selected_entity_arrow_vert_pos = selected_entity_arrow_vert_pos.lerp(transform->pos + Vec2(0, -64),20*delta_time);
                    selected_entity_arrow_vert_half = selected_entity_arrow_vert_half.lerp(transform->pos + Vec2(0, -32),30*delta_time);
                    draw_bezier(transform->pos,selected_entity_arrow_vert_half,selected_entity_arrow_vert_pos, this->position, Color(0,255,0));
                    //Draw arrow tip
                    SDL_RenderLine(Window::current->get_sdl_renderer(), selected_entity_arrow_vert_pos.x-this->position.x, selected_entity_arrow_vert_pos.y-this->position.y, selected_entity_arrow_vert_pos.x-8-this->position.x, selected_entity_arrow_vert_pos.y+8-this->position.y);
                    SDL_RenderLine(Window::current->get_sdl_renderer(), selected_entity_arrow_vert_pos.x-this->position.x, selected_entity_arrow_vert_pos.y-this->position.y, selected_entity_arrow_vert_pos.x+8-this->position.x, selected_entity_arrow_vert_pos.y+8-this->position.y);

                    //Move entity
                    if(ImGui::IsWindowFocused() && InputManager::get_mouse_button(1) && !dragging_entity && !dragging_entity_horizontal && !dragging_entity_vertical)
                    {
                        if(transform->pos.distance_to(mouse_world_pos) < 16)
                        {
                            dragging_entity = true;
                        }
                        if(selected_entity_arrow_hor_pos.distance_to(mouse_world_pos) < 16)
                        {
                            dragging_entity_horizontal = true;
                        }
                        if(selected_entity_arrow_vert_pos.distance_to(mouse_world_pos) < 16)
                        {
                            dragging_entity_vertical = true;
                        }
                    }
                    if(dragging_entity)
                    {
                        transform->pos = mouse_world_pos;
                    }
                    if(dragging_entity_horizontal)
                    {
                        transform->pos.x = mouse_world_pos.x - 64;
                        selected_entity_arrow_hor_pos = mouse_world_pos;
                    }
                    if(dragging_entity_vertical)
                    {
                        transform->pos.y = mouse_world_pos.y + 64;
                        selected_entity_arrow_vert_pos = mouse_world_pos;
                    }
                }
                else
                {
                    entity_selected = nullptr;
                }
            }
            else
            {
                entity_selected = nullptr;
            }
        }
        else
        {
            entity_selected = nullptr;
        }
        if(!ImGui::IsWindowFocused() || !InputManager::get_mouse_button(1))
        {
            dragging_entity = false;
            dragging_entity_horizontal = false;
            dragging_entity_vertical = false;
        }
        //Destroy camera entity
        camera->force_destroy();
        Entity::entity_count--;
        SDL_SetRenderTarget(Window::current->get_sdl_renderer(), nullptr);
    }

    //Move camera
    if(ImGui::IsWindowFocused() && InputManager::get_mouse_button(1) && !dragging_entity && !dragging_entity_horizontal && !dragging_entity_vertical)
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
