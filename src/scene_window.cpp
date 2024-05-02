#include "scene_window.h"

Window_SceneView::Window_SceneView()
{
    render_texture = SDL_CreateTexture(Window::current->get_sdl_renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
    this->name = "Scene View";
    this->flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
}

void draw_bezier(Vec2 a, Vec2 b, Vec2 c, Vec2 position, float zoom, Color color)
{
    int screen_w = 0;
    int screen_h = 0;
    SDL_GetCurrentRenderOutputSize(Window::current->get_sdl_renderer(), &screen_w, &screen_h);
    //Adjust for camera position
    a -= position;
    b -= position;
    c -= position;

    //Adjust for zoom
    a *= zoom;
    b *= zoom;
    c *= zoom;

    //Adjust for screen position
    a.x += screen_w/2;
    a.y += screen_h/2;
    b.x += screen_w/2;
    b.y += screen_h/2;
    c.x += screen_w/2;
    c.y += screen_h/2;

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
void draw_circle(Vec2 pos, float radius, Vec2 position, float zoom, Color color)
{
    int screen_w = 0;
    int screen_h = 0;
    SDL_GetCurrentRenderOutputSize(Window::current->get_sdl_renderer(), &screen_w, &screen_h);
    //Adjust for camera position
    pos -= position;

    //Adjust for zoom
    pos *= zoom;

    //Adjust for screen position
    pos.x += screen_w/2;
    pos.y += screen_h/2;

    //Draw circle
    SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), color.r, color.g, color.b, 255);
    for(float t = 0; t < 2.0f*3.14159f; t+=0.01f)
    {
        Vec2 p = pos + Vec2(cos(t), sin(t))*radius*zoom;
        SDL_RenderPoint(Window::current->get_sdl_renderer(), p.x, p.y);
    }
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
    if(Editor::current_scene != nullptr)
    {
        SDL_SetRenderTarget(Window::current->get_sdl_renderer(), render_texture);
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), EditorTheme::scene_view_background.x,EditorTheme::scene_view_background.y,EditorTheme::scene_view_background.z, 255);
        SDL_RenderClear(Window::current->get_sdl_renderer());
        //Create camera entity
        Entity* camera = new Entity();
        camera->add_components<Camera,Transform>();
        Camera::make_current(camera);

        //Lerp zoom to zoom target
        zoom = (zoom_target - zoom) * 0.1f + zoom;

        camera->get_component<Transform>()->pos = this->position;
        camera->get_component<Camera>()->zoom = zoom;

        //Axis
        int screen_w = 0;
        int screen_h = 0;
        SDL_GetCurrentRenderOutputSize(Window::current->get_sdl_renderer(), &screen_w, &screen_h);

        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), EditorTheme::scene_view_y.x,EditorTheme::scene_view_y.y,EditorTheme::scene_view_y.z, 255);
        SDL_SetRenderDrawBlendMode(Window::current->get_sdl_renderer(), SDL_BLENDMODE_BLEND);
        SDL_RenderLine(Window::current->get_sdl_renderer(), -this->position.x*zoom+screen_w/2, 0, -this->position.x*zoom+screen_w/2, ImGui::GetWindowHeight());
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), EditorTheme::scene_view_x.x,EditorTheme::scene_view_x.y,EditorTheme::scene_view_x.z, 255);
        SDL_RenderLine(Window::current->get_sdl_renderer(), 0, -this->position.y*zoom+screen_h/2, ImGui::GetWindowWidth(), -this->position.y*zoom+screen_h/2);



        //Render scene
        Editor::current_scene->draw(nullptr);

        Vec2 mouse_world_pos = ((InputManager::get_mouse_position()-Vec2(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y))-Vec2(screen_w/2,screen_h/2))/zoom+this->position;
        //Draw movement arrows around selected entity
        if (Node::selected != nullptr)
        {
            if(previous_selected_node != Node::selected)
            {
                entities_selected.clear();
            }
            if(Node::selected->entity != nullptr)
            {
                if(Node::selected->entity->get_component<Transform>())
                {
                    if(std::find(entities_selected.begin(), entities_selected.end(),Node::selected->entity.get()) == entities_selected.end())
                    {
                        selected_entity_arrow_hor_pos = Node::selected->entity->get_component<Transform>()->pos;
                        selected_entity_arrow_hor_half = Node::selected->entity->get_component<Transform>()->pos;
                        selected_entity_arrow_vert_pos = Node::selected->entity->get_component<Transform>()->pos;
                        selected_entity_arrow_vert_half = Node::selected->entity->get_component<Transform>()->pos;
                        entities_selected.push_back(Node::selected->entity.get());
                    }
                }
                else
                {
                    entities_selected.clear();
                }
            }
            else if(Node::selected->group != nullptr)
            {
                std::vector<Entity*>* entities = Node::selected->group->get_all_entities();
                if(entities->size() > 0)
                {
                    if (std::find(entities_selected.begin(), entities_selected.end(), entities->at(0)) ==
                        entities_selected.end()) {
                        if(entities->at(0)->has_component<Transform>())
                        {
                            selected_entity_arrow_hor_pos = entities->at(0)->get_component<Transform>()->pos;
                            selected_entity_arrow_hor_half = entities->at(0)->get_component<Transform>()->pos;
                            selected_entity_arrow_vert_pos = entities->at(0)->get_component<Transform>()->pos;
                            selected_entity_arrow_vert_half = entities->at(0)->get_component<Transform>()->pos;
                        }
                        for (Entity *entity: *entities) {
                            entities_selected.push_back(entity);
                        }
                    }
                }
            }
            else
            {
                entities_selected.clear();
            }
            previous_selected_node = Node::selected;
        }
        else
        {
            entities_selected.clear();
            previous_selected_node = nullptr;
        }

        //Transform

        //Get all the transforms
        std::vector<Transform*> transforms;
        for(Entity* entity : entities_selected)
        {
            if(entity->has_component<Transform>())
                transforms.push_back(entity->get_component<Transform>());
        }

        Vec2 pos = Vec2();
        //Get average position
        for(Transform* transform : transforms)
        {
            pos += transform->pos;
        }
        pos /= transforms.size();
        Vec2 initial_pos = pos;

        float rotation = 0;
        //Get average rotation
        for(Transform* transform : transforms)
        {
            rotation += transform->rot;
        }
        rotation /= (float)transforms.size();
        float inital_rot = rotation;

        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), EditorTheme::scene_view_x.x,
                               EditorTheme::scene_view_x.y, EditorTheme::scene_view_x.z, 255);
        //Draw arrow
        if (!dragging_entity_horizontal)
            selected_entity_arrow_hor_pos = selected_entity_arrow_hor_pos.lerp(
                    pos + Vec2(64, 0), 15 * delta_time);
        selected_entity_arrow_hor_half = selected_entity_arrow_hor_half.lerp(
                pos + Vec2(32, 0), 30 * delta_time);
        draw_bezier(pos, selected_entity_arrow_hor_half, selected_entity_arrow_hor_pos,
                    this->position, zoom, Color(255, 0, 0));
        //Draw arrow tip
        SDL_RenderLine(Window::current->get_sdl_renderer(),
                       (selected_entity_arrow_hor_pos.x - this->position.x) * zoom + screen_w / 2,
                       (selected_entity_arrow_hor_pos.y - this->position.y) * zoom + screen_h / 2,
                       (selected_entity_arrow_hor_pos.x - 8 - this->position.x) * zoom + screen_w / 2,
                       (selected_entity_arrow_hor_pos.y - 8 - this->position.y) * zoom + screen_h / 2);
        SDL_RenderLine(Window::current->get_sdl_renderer(),
                       (selected_entity_arrow_hor_pos.x - this->position.x) * zoom + screen_w / 2,
                       (selected_entity_arrow_hor_pos.y - this->position.y) * zoom + screen_h / 2,
                       (selected_entity_arrow_hor_pos.x - 8 - this->position.x) * zoom + screen_w / 2,
                       (selected_entity_arrow_hor_pos.y + 8 - this->position.y) * zoom + screen_h / 2);
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), EditorTheme::scene_view_y.x,
                               EditorTheme::scene_view_y.y, EditorTheme::scene_view_y.z, 255);
        //Draw arrow
        if (!dragging_entity_vertical)
            selected_entity_arrow_vert_pos = selected_entity_arrow_vert_pos.lerp(
                    pos + Vec2(0, -64), 15 * delta_time);
        selected_entity_arrow_vert_half = selected_entity_arrow_vert_half.lerp(
                pos + Vec2(0, -32), 30 * delta_time);
        draw_bezier(pos, selected_entity_arrow_vert_half, selected_entity_arrow_vert_pos,
                    this->position, zoom, Color(0, 255, 0));
        //Draw arrow tip
        SDL_RenderLine(Window::current->get_sdl_renderer(),
                       (selected_entity_arrow_vert_pos.x - this->position.x) * zoom + screen_w / 2,
                       (selected_entity_arrow_vert_pos.y - this->position.y) * zoom + screen_h / 2,
                       (selected_entity_arrow_vert_pos.x - 8 - this->position.x) * zoom + screen_w / 2,
                       (selected_entity_arrow_vert_pos.y + 8 - this->position.y) * zoom + screen_h / 2);
        SDL_RenderLine(Window::current->get_sdl_renderer(),
                       (selected_entity_arrow_vert_pos.x - this->position.x) * zoom + screen_w / 2,
                       (selected_entity_arrow_vert_pos.y - this->position.y) * zoom + screen_h / 2,
                       (selected_entity_arrow_vert_pos.x + 8 - this->position.x) * zoom + screen_w / 2,
                       (selected_entity_arrow_vert_pos.y + 8 - this->position.y) * zoom + screen_h / 2);
        //Draw circle around entity transform position
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), EditorTheme::color_palette_white.x,
                               EditorTheme::color_palette_white.y, EditorTheme::color_palette_white.z,
                               255);
        //With bezier curve
        draw_bezier(pos + Vec2(16, 0), pos + Vec2(16, 16),
                    pos + Vec2(0, 16), this->position, zoom, Color(255, 255, 255));
        draw_bezier(pos + Vec2(-16, 0), pos + Vec2(-16, -16),
                    pos + Vec2(0, -16), this->position, zoom, Color(255, 255, 255));
        draw_bezier(pos + Vec2(16, 0), pos + Vec2(16, -16),
                    pos + Vec2(0, -16), this->position, zoom, Color(255, 255, 255));
        draw_bezier(pos + Vec2(-16, 0), pos + Vec2(-16, 16),
                    pos + Vec2(0, 16), this->position, zoom, Color(255, 255, 255));

        //Draw larger circle for rotation
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), EditorTheme::color_palette_blue.x,
                               EditorTheme::color_palette_blue.y, EditorTheme::color_palette_blue.z,
                               255);
        draw_circle(pos, 48, this->position, zoom, Color(255, 255, 255));

        //Rotation
        if (ImGui::IsWindowFocused() && InputManager::get_mouse_button(1) && !dragging_entity &&
            !dragging_entity_horizontal && !dragging_entity_vertical) {
            if (pos.distance_to(mouse_world_pos) < 56 && pos.distance_to(mouse_world_pos) > 40) {
                if(!dragging_entity_rotation)
                {
                    dragging_start_angle = Vec2(0,0).angle_to((mouse_world_pos - pos)) - Vec2(0,0).angle_to((pos - Vec2(0, 0)));
                }
                dragging_entity_rotation = true;
            }
        }

        //Move entity
        if (ImGui::IsWindowFocused() && InputManager::get_mouse_button(1) && !dragging_entity &&
            !dragging_entity_horizontal && !dragging_entity_vertical && !dragging_entity_rotation) {
            if (pos.distance_to(mouse_world_pos) < 16) {
                dragging_entity = true;
            }
            if (selected_entity_arrow_hor_pos.distance_to(mouse_world_pos) < 16) {
                dragging_entity_horizontal = true;
            }
            if (selected_entity_arrow_vert_pos.distance_to(mouse_world_pos) < 16) {
                dragging_entity_vertical = true;
            }
        }
        if (dragging_entity) {
            pos = mouse_world_pos;
        }
        if (dragging_entity_horizontal) {
            pos.x = mouse_world_pos.x - 64;
            selected_entity_arrow_hor_pos = mouse_world_pos;
        }
        if (dragging_entity_vertical) {
            pos.y = mouse_world_pos.y + 64;
            selected_entity_arrow_vert_pos = mouse_world_pos;
        }
        if (dragging_entity_rotation) {
            rotation = Vec2(0,0).angle_to((mouse_world_pos - pos)) - Vec2(0,0).angle_to((pos - Vec2(0, 0)));
        }
        //Adjust all the transforms
        Vec2 delta = pos - initial_pos;
        for (Transform *transform : transforms) {
            transform->pos += delta;
            if(dragging_entity_rotation)
            {
                float rotation_delta = rotation - dragging_start_angle;
                transform->rot += rotation_delta;
                //Rotate transform position
                Vec2 new_pos = pos + (transform->pos - pos).rotate(rotation_delta);
                transform->pos = new_pos;
            }
        }
        dragging_start_angle = rotation;

        if(!InputManager::get_mouse_button(1))
        {
            if(dragging_entity || dragging_entity_horizontal || dragging_entity_vertical || dragging_entity_rotation)
            {
                Editor::action();
            }
            dragging_entity = false;
            dragging_entity_horizontal = false;
            dragging_entity_vertical = false;
            dragging_entity_rotation = false;
            dragging_start_angle = 0;
        }
        else
        {
            print_info("Not entity");
        }
        //Destroy camera entity
        camera->force_destroy();
        Entity::entity_count--;
        SDL_SetRenderTarget(Window::current->get_sdl_renderer(), nullptr);
    }

    //Move camera
    if(ImGui::IsWindowFocused()) {
        if (InputManager::get_mouse_button(1) && !dragging_entity &&
            !dragging_entity_horizontal && !dragging_entity_vertical && !dragging_entity_rotation) {
            this->position -= InputManager::mouse_delta / zoom;
        }
        if (InputManager::get_mouse_scrolled()) {
            this->zoom_target += (InputManager::mouse_scroll.y * 0.1) * zoom_target;
        }
    }
    if(zoom_target < 0.01)
    {
        zoom_target = 0.01;
    }
    ImGui::SetCursorPos(ImVec2(0,0));
    ImGui::Image(render_texture, ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()));
    if(ImGui::BeginDragDropTarget())
    {
        if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("resource_texture"))
        {
            Texture* tex = *(Texture**) payload->Data;
            std::string entity_name = tex->path;
            Entity *entity = new Entity();
            entity->add_component<Transform>();
            entity->add_component<Sprite>();
            entity->get_component<Sprite>()->texture = tex;
            entity->get_component<Transform>()->pos = ((InputManager::get_mouse_position()-Vec2(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y))-Vec2(size.x/2,size.y/2))/zoom+this->position;

            Editor::current_scene->add_entity(entity);
        }
        else if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("resource_scene"))
        {
            const char* scene_path = (const char*)payload->Data;
            SceneGroup* group = new SceneGroup("Scene", scene_path);
            group->instantiate();
            group->name = group->get_child_groups()[0][0]->name;
            Editor::current_scene->add_group(group);
        }
    }
    //Draw debug info
    //Set cursor position to top left with margin
    ImGui::SetCursorPos(ImVec2(5,25));
    //Draw text
    ImGui::Text("Camera pos: %f %f", this->position.x, this->position.y);
    ImGui::Text("Camera zoom: %f", this->zoom);
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
