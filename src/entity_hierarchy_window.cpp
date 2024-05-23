#include "entity_hierarchy_window.h"

std::vector<Node*> Window_EntityHierarchy::nodes = std::vector<Node*>();
Node* Node::selected = nullptr;
Node* Window_EntityHierarchy::selected_node = nullptr;
Node* Window_EntityHierarchy::dragging_node = nullptr;
bool create_system_popup = false;
bool Window_EntityHierarchy::searching = false;
Node* Window_EntityHierarchy::currently_linking = nullptr;
bool Window_EntityHierarchy::linking = false;
bool Window_EntityHierarchy::trying_to_link = false;
Node* Window_EntityHierarchy::to_try_link = nullptr;
float Window_EntityHierarchy::linking_distance = 0.0f;

void draw_circle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderPoint(renderer, (float)centreX + (float)x, (float)centreY - (float)y);
        SDL_RenderPoint(renderer, (float)centreX + (float)x, (float)centreY + (float)y);
        SDL_RenderPoint(renderer, (float)centreX - (float)x, (float)centreY - (float)y);
        SDL_RenderPoint(renderer, (float)centreX - (float)x, (float)centreY + (float)y);
        SDL_RenderPoint(renderer, (float)centreX + (float)y, (float)centreY - (float)x);
        SDL_RenderPoint(renderer, (float)centreX + (float)y, (float)centreY + (float)x);
        SDL_RenderPoint(renderer, (float)centreX - (float)y, (float)centreY - (float)x);
        SDL_RenderPoint(renderer, (float)centreX - (float)y, (float)centreY + (float)x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

void draw_filled_circle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius) {
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y) {
        //  Each of the following renders an octant of the circle
        SDL_RenderLine(renderer, (float)centreX - (float)x, (float)centreY - (float)y, (float)centreX + (float)x, (float)centreY - (float)y);
        SDL_RenderLine(renderer, (float)centreX - (float)x, (float)centreY + (float)y, (float)centreX + (float)x, (float)centreY + (float)y);
        SDL_RenderLine(renderer, (float)centreX - (float)y, (float)centreY - (float)x, (float)centreX + (float)y, (float)centreY - (float)x);
        SDL_RenderLine(renderer, (float)centreX - (float)y, (float)centreY + (float)x, (float)centreX + (float)y, (float)centreY + (float)x);

        if (error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

void Window_EntityHierarchy::render()
{
    HotkeyManager::add_hotkey({{SDL_SCANCODE_LCTRL, SDL_SCANCODE_H}, "Focus Hierarchy", focus});
    if(ImGui::IsWindowFocused())
    {
        HotkeyManager::add_hotkey({{SDL_SCANCODE_LSHIFT, SDL_SCANCODE_E}, "Create Entity", create_entity});
        HotkeyManager::add_hotkey({{SDL_SCANCODE_LSHIFT, SDL_SCANCODE_G}, "Create Group", create_group});
        HotkeyManager::add_hotkey({{SDL_SCANCODE_LSHIFT, SDL_SCANCODE_S}, "Create System", create_system});
        HotkeyManager::add_hotkey({{SDL_SCANCODE_LSHIFT,SDL_SCANCODE_A}, "Create AutoGroup", create_auto_group});
        HotkeyManager::add_hotkey({{SDL_SCANCODE_BACKSPACE}, "Delete Node", delete_node});
        HotkeyManager::add_hotkey({{SDL_SCANCODE_DELETE}, "Delete Node", delete_node});
        HotkeyManager::add_hotkey({{SDL_SCANCODE_D, SDL_SCANCODE_LSHIFT}, "Duplicate", duplicate});
        HotkeyManager::add_hotkey({{SDL_SCANCODE_LEFT}, "Move Left", move_left});
        HotkeyManager::add_hotkey({{SDL_SCANCODE_RIGHT}, "Move Right", move_right});
        HotkeyManager::add_hotkey({{SDL_SCANCODE_UP}, "Move Up", move_up});
        HotkeyManager::add_hotkey({{SDL_SCANCODE_DOWN}, "Move Down", move_down});
        HotkeyManager::add_hotkey({{SDL_SCANCODE_LSHIFT, SDL_SCANCODE_L}, "Begin Link", begin_link});
    }
    else
    {
        HotkeyManager::disable_hotkey("Create Entity");
        HotkeyManager::disable_hotkey("Create Group");
        HotkeyManager::disable_hotkey("Create System");
        HotkeyManager::disable_hotkey("Delete Node");
        HotkeyManager::disable_hotkey("Duplicate");
        HotkeyManager::disable_hotkey("Move Left");
        HotkeyManager::disable_hotkey("Move Right");
        HotkeyManager::disable_hotkey("Move Up");
        HotkeyManager::disable_hotkey("Move Down");
        HotkeyManager::disable_hotkey("Begin Link");
    }

    //Clean the graph
    for (int i = 0; i < nodes.size(); ++i) {
        if(nodes[i]->entity == nullptr && nodes[i]->group == nullptr && nodes[i]->system == nullptr)
        {
            nodes.erase(nodes.begin() + i);
            i--;
        }
    }
    build_graph(Editor::current_scene);
    Vec2i tex_size;

    //Search bar

    //Check if window is focused
    if(ImRect(ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + ImGui::GetWindowHeight())).Contains(ImGui::GetMousePos())){
        //Check if mouse is hovering over a node

        bool hovering_node = false;
        if(!InputManager::get_mouse_button(SDL_BUTTON_MIDDLE))
        {
            for (auto &node: nodes) {
                //Calculate screen pos
                Vec2 node_pos = node->pos;
                node_pos.x -= cam_pos.x;
                node_pos.y -= cam_pos.y;
                node_pos.x /= zoom;
                node_pos.y /= zoom;
                node_pos.x += (float) size.x / 2;
                node_pos.y += (float) size.y / 2;
                //Check if mouse is hovering over node
                Vec2 mouse = InputManager::get_mouse_position() - Vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
                if (mouse.distance_to(node_pos) < node->size) {
                    hovering_node = true;
                    if(InputManager::get_mouse_button(SDL_BUTTON_RIGHT))
                    {
                        to_try_link = node;
                        trying_to_link = true;
                        linking_distance = 0;
                    }
                    if (InputManager::get_mouse_button(SDL_BUTTON_LEFT) && dragging_node == nullptr) {
                        selected_node = node;
                        Node::selected = node;
                        dragging_node = node;
                        if(linking)
                        {
                            if(currently_linking != nullptr)
                            {
                                Node* link_to = node;
                                if(currently_linking->entity != nullptr)
                                {
                                    link_to = currently_linking;
                                    currently_linking = node;
                                }
                                else if(currently_linking->system != nullptr)
                                {
                                    link_to = currently_linking;
                                    currently_linking = node;
                                }
                                currently_linking->linked.push_back(link_to);
                                if(link_to->entity != nullptr)
                                {
                                    currently_linking->group->add_entity(link_to->entity.get());
                                }
                                else if(link_to->group != nullptr)
                                {
                                    currently_linking->group->add_group(link_to->group.get());
                                }
                                else if(link_to->system != nullptr)
                                {
                                    currently_linking->group->add_system(link_to->system.get());
                                }
                                linking = false;
                            }
                            currently_linking = node;
                        }
                    }
                }
            }
        }

        if(InputManager::get_mouse_button(SDL_BUTTON_LEFT))
        {
            linking = false;
        }

        if(InputManager::get_mouse_button(SDL_BUTTON_RIGHT) && to_try_link != nullptr)
        {
            Vec2 node_pos = to_try_link->pos;
            node_pos.x -= cam_pos.x;
            node_pos.y -= cam_pos.y;
            node_pos.x /= zoom;
            node_pos.y /= zoom;
            node_pos.x += (float) size.x / 2;
            node_pos.y += (float) size.y / 2;
            linking_distance = node_pos.distance_to(Vec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y));
            if(linking_distance > 32)
            {
                linking = true;
                trying_to_link = false;
                currently_linking = to_try_link;
                to_try_link = nullptr;
            }
        }
        else
        {
            to_try_link = nullptr;
            trying_to_link = false;
        }

        if (!hovering_node && dragging_node == nullptr) {
            if (InputManager::get_mouse_button(1)) {
                cam_pos.x -= InputManager::get_mouse_delta().x * zoom;
                cam_pos.y -= InputManager::get_mouse_delta().y * zoom;
            }
        }
        if (!InputManager::get_mouse_button(SDL_BUTTON_LEFT)) {
            dragging_node = nullptr;
        }

        if (dragging_node != nullptr) {
            Vec2 mouse = InputManager::get_mouse_position()-Vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
            mouse.x -= (float)size.x / 2;
            mouse.y -= (float)size.y / 2;
            mouse.x *= zoom;
            mouse.y *= zoom;
            mouse.x += cam_pos.x;
            mouse.y += cam_pos.y;
            dragging_node->velocity += (mouse - dragging_node->pos) * 0.1;
        }
        if (InputManager::get_mouse_scrolled()) {
            Vec2 old_mouse = InputManager::get_mouse_position()-Vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
            old_mouse.x -= (float)size.x / 2;
            old_mouse.y -= (float)size.y / 2;
            old_mouse.x *= zoom;
            old_mouse.y *= zoom;
            //Divide or multiply zoom based on scroll
            zoom += InputManager::get_mouse_scroll().y * -0.1f*zoom;
            Vec2 new_mouse = InputManager::get_mouse_position()-Vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
            new_mouse.x -= (float)size.x / 2;
            new_mouse.y -= (float)size.y / 2;
            new_mouse.x *= zoom;
            new_mouse.y *= zoom;
            cam_pos.x -= (new_mouse.x - old_mouse.x);
            cam_pos.y -= (new_mouse.y - old_mouse.y);
        }
    }



    for (auto & node : nodes) {
        simulate_node(node);
        draw_node(node);
    }

    SDL_SetRenderTarget(Window::current->get_sdl_renderer(), nullptr);
    ImGui::SetCursorPos(ImVec2(0, 0));

    if(ImGui::BeginPopupContextWindow("context_menu"))
    {
        if(linking_distance < 32 && selected_node != nullptr)
        {
            if(selected_node->group != nullptr)
            {
                if(ImGui::MenuItem("Create Entity"))
                {
                    create_entity();
                }
                if(ImGui::MenuItem("Create Group")) {
                    create_group();
                }
                if(ImGui::MenuItem("Create AutoGroup")) {
                    create_auto_group();
                }
                if(ImGui::MenuItem("Create System")) {
                    create_system();
                }
            }
            if(selected_node->entity != nullptr)
            {
                if(ImGui::MenuItem("Duplicate"))
                {
                    duplicate();
                }
            }
            if(ImGui::MenuItem("Delete Node"))
            {
                delete_node();
            }
        }
        ImGui::EndPopup();
    }

    if(create_system_popup)
        ImGui::OpenPopup("create system");
    if(ImGui::BeginPopup("create system"))
    {
        if(InputManager::get_key(SDL_SCANCODE_ESCAPE))
        {
            create_system_popup = false;
        }
        for (auto i = System::system_types.begin(); i != System::system_types.end(); i++) {
            if (ImGui::MenuItem(scuffy_demangle(i->first.c_str()).c_str())) {
                //Add the system as child to selected node
                if(selected_node != nullptr)
                {
                    if(selected_node->group != nullptr)
                    {
                        System* system = i->second();
                        selected_node->group->add_system(system);
                        Editor::action();
                        create_system_popup = false;
                        LiveShare::send_create_system(system);
                    }
                    else
                    {
                        //Find parents
                        std::vector<Group*> parents;
                        for (auto & node : nodes) {
                            if (node->group != nullptr) {
                                if (std::find(node->group->get_entities().begin(), node->group->get_entities().end(),
                                              selected_node->entity.get()) != node->group->get_entities().end()) {
                                    parents.push_back(node->group.get());
                                }
                            }
                        }
                        //Create a new system and link it to the parents
                        System* system = i->second();
                        for (auto & parent : parents) {
                            parent->add_system(system);
                        }
                        Editor::action();
                        create_system_popup = false;
                        LiveShare::send_create_system(system);
                    }
                }
            }
        }
        if(!ImGui::IsItemHovered())
        {
            if(ImGui::IsMouseClicked(0))
            {
                create_system_popup = false;
            }
        }
        ImGui::EndPopup();
    }

    ImGui::SetCursorPos(ImVec2(0, 24));
    ImGui::PushItemWidth(ImGui::GetWindowWidth());
    ImGui::InputText("##Search", &search);
    ImGui::PopItemWidth();
    if(ImGui::IsItemFocused())
    {
        searching = true;
    }
    else
    {
        searching = false;
    }
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // Gray out the hint text

    if (search.empty())
    {
        ImGui::SetCursorPos(ImVec2(0, 24));
        ImVec2 label_size = ImGui::CalcTextSize("Search nodes...");
        ImGui::SameLine(0, 0);
        ImGui::SetCursorPos(ImVec2(5, 24));
        ImGui::TextUnformatted("Search nodes...");
    }

    ImGui::PopStyleColor();
}

Vec2 find_closest_point_on_line(Vec2 start, Vec2 end, Vec2 point)
{
    // Vector from start to end
    Vec2 lineVec(end.x - start.x, end.y - start.y);

    // Vector from start to point
    Vec2 pointVec(point.x - start.x, point.y - start.y);

    // Calculate the scalar projection of pointVec onto lineVec
    float t = (pointVec.x * lineVec.x + pointVec.y * lineVec.y) / (lineVec.x * lineVec.x + lineVec.y * lineVec.y);

    // Clamp t to be within the range [0, 1] to ensure the closest point is on the line segment
    if(t < 0)
        t = 0;
    else if(t > 1)
        t = 1;
    // Calculate the closest point on the line segment
    Vec2 closestPoint(start.x + t * lineVec.x, start.y + t * lineVec.y);

    return closestPoint;
}

void Window_EntityHierarchy::create_entity()
{
    //Add the entity as child to selected node
    if(selected_node != nullptr)
    {
        if(selected_node->group != nullptr)
        {
            //Create a new entity
            auto entity = Entity::create("New Entity");
            entity->add_component<Transform>();
            selected_node->group->add_entity(entity);
            Editor::action();
            LiveShare::send_new_entity(entity);
            LiveShare::send_change_entity_name(entity, entity->name);
            LiveShare::send_add_component(entity, typeid(Transform).name());
        }
        else
        {
            //Find parents
            std::vector<Group*> parents;
            for (auto & node : nodes) {
                if (node->group != nullptr) {
                    if (std::find(node->group->get_entities().begin(), node->group->get_entities().end(),
                                  selected_node->entity.get()) != node->group->get_entities().end()) {
                        parents.push_back(node->group.get());
                    }
                }
            }
            //Create a new entity and link it to the parents
            auto entity = Entity::create("New Entity");
            entity->add_component<Transform>();
            for (auto & parent : parents) {
                parent->add_entity(entity);
            }
            Editor::action();
            LiveShare::send_new_entity(entity);
            LiveShare::send_change_entity_name(entity, entity->name);
            LiveShare::send_add_component(entity, typeid(Transform).name());
        }
    }
}

void Window_EntityHierarchy::create_group()
{
    //Add the group as child to selected node
    if(selected_node != nullptr)
    {
        if(selected_node->group != nullptr)
        {
            //Create a new group
            auto *group = new Group("New Group");
            selected_node->group->add_group(group);
            LiveShare::send_create_group(group);
            Editor::action();
        }
        else
        {
            //Find parents
            std::vector<Group*> parents;
            for (auto & node : nodes) {
                if (node->group != nullptr) {
                    if (std::find(node->group->get_entities().begin(), node->group->get_entities().end(),
                                  selected_node->entity.get()) != node->group->get_entities().end()) {
                        parents.push_back(node->group.get());
                    }
                }
            }
            //Create a new group and link it to the parents
            auto *group = new Group("New Group");
            for (auto & parent : parents) {
                parent->add_group(group);
            }
            LiveShare::send_create_group(group);
            Editor::action();
        }
    }
}

void Window_EntityHierarchy::create_auto_group()
{
    //Add the group as child to selected node
    if(selected_node != nullptr)
    {
        if(selected_node->group != nullptr)
        {
            //Create a new group
            auto *group = new AutoGroup("New AutoGroup");
            selected_node->group->add_group(group);
            Editor::action();
        }
        else
        {
            //Find parents
            std::vector<Group*> parents;
            for (auto & node : nodes) {
                if (node->group != nullptr) {
                    if (std::find(node->group->get_entities().begin(), node->group->get_entities().end(),
                                  selected_node->entity.get()) != node->group->get_entities().end()) {
                        parents.push_back(node->group.get());
                    }
                }
            }
            //Create a new group and link it to the parents
            auto *group = new AutoGroup("New AutoGroup");
            for (auto & parent : parents) {
                parent->add_group(group);
            }
            Editor::action();
        }
    }
}

void Window_EntityHierarchy::create_system()
{
    create_system_popup = true;
}

void Window_EntityHierarchy::update()
{

}

Window_EntityHierarchy::Window_EntityHierarchy()
{
    this->flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoNav;
    this->name = "Graph Hierarchy";
    this->open = true;
    this->position = Vec2i(0, 0);
    this->cam_pos = Vec2(0, 0);
    this->zoom = 1.0;
    this->size = Vec2i(512, Window::current->get_height());
    selected_node = nullptr;
    dragging_node = nullptr;
}
static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
{
    return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}
void ImageRotated(ImTextureID tex_id, ImVec2 center, ImVec2 size, ImVec4 color, float angle)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float cos_a = cosf(angle);
    float sin_a = sinf(angle);
    ImVec2 pos[4] =
            {
                    center + ImRotate(ImVec2(-size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
                    center + ImRotate(ImVec2(+size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
                    center + ImRotate(ImVec2(+size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a),
                    center + ImRotate(ImVec2(-size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a)
            };
    ImVec2 uvs[4] =
            {
                    ImVec2(0.0f, 0.0f),
                    ImVec2(1.0f, 0.0f),
                    ImVec2(1.0f, 1.0f),
                    ImVec2(0.0f, 1.0f)
            };

    ImU32 col = IM_COL32((int)(color.x * 255), (int)(color.y * 255), (int)(color.z * 255), (int)(color.w * 255));
    draw_list->AddImageQuad(tex_id, pos[0], pos[1], pos[2], pos[3], uvs[0], uvs[1], uvs[2], uvs[3], col);
}
std::vector<Node*> Window_EntityHierarchy::find_all_parents(Node* node)
{
    //Find parents and parents of parents based on linkages use recursion and do not add itself
    std::vector<Node*> parents;
    for (auto & n : nodes)
    {
        if (n != node && n->group != nullptr)
        {
            if (std::find(n->linked.begin(), n->linked.end(), node) != n->linked.end())
            {
                parents.push_back(n);
            }
        }
    }
    return parents;
}
bool Window_EntityHierarchy::is_node_visible(Node *node)
{
    std::vector<Node*> parents = find_all_parents(node);
    if(parents.size() > 0)
    {
        //Check if its open
        bool any_open = false;
        for (auto & parent : parents) {
            if (parent->open && is_node_visible(parent)) {
                any_open = true;
                break;
            }
        }
        if(!any_open)
        {
            return false;
        }
    }
    return true;
}
void Window_EntityHierarchy::draw_node(Node* n)
{
    if(!is_node_visible(n))
    {
        return;
    }

    Vec2 pos = n->pos;
    float s = n->size;
    Color color = n->color;
    //Calculate the position of the node based on camera position/zoom
    Vec2 node_pos = pos;
    //Center on screen
    node_pos.x -= cam_pos.x;
    node_pos.y -= cam_pos.y;

    node_pos.x /= zoom;
    node_pos.y /= zoom;

    node_pos.x += (float)size.x / 2;
    node_pos.y += (float)size.y / 2;
    //Calculate the size of the node based on camera zoom
    float node_size = s;

    if(selected_node == n)
    {
        //Draw selection circle imgui drawlist
        ImGui::GetCurrentWindow()->DrawList->AddCircle(ImVec2(node_pos.x, node_pos.y + node_size*3), node_size + 4, IM_COL32(255, 255, 255, 255), 32, 2);
    }

    //Draw name
    std::string name;
    if(n->group != nullptr)
    {
        name = n->group->name;
        if(typeid(*n->group.get()) == typeid(AutoGroup))
        {
            n->texture = ResourceManager::load<Texture>("engine/auto_group.png");
            n->color = Color((int)EditorTheme::color_palette_purple.x,(int)EditorTheme::color_palette_purple.y,(int)EditorTheme::color_palette_purple.z,255);
        }
    }
    if(n->entity != nullptr)
    {
        name = n->entity->name;
    }
    if(n->system != nullptr)
    {
        name = scuffy_demangle(typeid(*n->system.get()).name());
    }

    std::string name1 = name;
    std::transform(name1.begin(), name1.end(), name1.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    std::string search1 = search;
    std::transform(search1.begin(), search1.end(), search1.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    if((name1.find(search)) == std::string::npos)
    {
        color = color * 0.5f;
    }
    if(!n->open)
    {
        color = color * 0.75f;
    }
    if(linking && currently_linking != n)
    {
        float d = Vec2(ImGui::GetMousePos().x,ImGui::GetMousePos().y).distance_to(Vec2(node_pos.x,node_pos.y+node_size*3))/64.0f;
        if(d>0.5f)
        {
            d = 0.5f;
        }
        d = 0.5f - d;
        color = color * ((d)+0.5f);
    }
    else
    {
        //Draw line to mouse from node
        if(linking)
        {
            ImGui::GetCurrentWindow()->DrawList->AddLine(ImVec2(node_pos.x, node_pos.y + node_size*3), ImVec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y), IM_COL32(255, 255, 255, 255), 2);
        }
    }

    ImGui::SetCursorPos(ImVec2(node_pos.x-node_size, node_pos.y-node_size));
    ImageRotated((void*)n->texture->get_sdl_texture(),ImVec2(node_pos.x, node_pos.y+node_size*3) ,ImVec2(node_size*2, node_size*2), ImVec4(color.r/255.0f, color.g/255.0f, color.b/255.0f, 1.0f),n->velocity.x*0.05f);
    //Make it DragDrop for the inspector
    ImGui::SetItemAllowOverlap();
    ImGui::SetCursorPos(ImVec2(node_pos.x-node_size, node_pos.y-node_size));
    ImGui::InvisibleButton(("invisible_button_" + name).c_str(), ImVec2(node_size * 2, node_size * 2));

    if(InputManager::get_mouse_button(SDL_BUTTON_MIDDLE))
    {
        print_info("Middle clicked!");
        std::string name = std::string("##") + (n->entity != nullptr ? n->entity->name : n->group != nullptr ? n->group->name : n->system != nullptr ? scuffy_demangle(typeid(*n->system.get()).name()) : "");
        ImGui::InvisibleButton(name.c_str(), ImVec2(node_size * 2, node_size * 2));
        ImGui::ButtonBehavior(ImRect(ImGui::GetItemRectMin(),ImGui::GetItemRectMax()), ImGui::GetItemID(), NULL, NULL, ImGuiButtonFlags_MouseButtonMiddle);

        if (ImGui::BeginDragDropSource()) {
            if(n->entity != nullptr)
                ImGui::SetDragDropPayload("Entity", &n->entity, sizeof(Entity *));
            if(n->group != nullptr)
                ImGui::SetDragDropPayload("Group", &n->group, sizeof(Group *));
            if(n->system != nullptr)
                ImGui::SetDragDropPayload("System", &n->system, sizeof(System *));
            ImGui::Image((void *) n->texture->get_sdl_texture(), ImVec2(node_size * 2, node_size * 2), ImVec2(0, 0),
                         ImVec2(1, 1), ImVec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f));
            ImGui::EndDragDropSource();
        }
    }
    else
    {
        if(ImGui::IsItemHovered() && ImGui::GetMouseClickedCount(0) == 2)
        {
            if(n->group != nullptr)
            {
                print_info("Toggled open");
                n->open = !n->open;
            }
        }
    }

    ImGui::SetCursorPos(ImVec2(node_pos.x-(float)name.size()*4, node_pos.y+node_size));
    ImGui::Text(name.c_str());
    for (auto &i: n->linked)
    {
        if(is_node_visible(i) && !linking) {
            Vec2 linked_pos = i->pos;
            linked_pos.x -= cam_pos.x;
            linked_pos.y -= cam_pos.y;
            linked_pos.x /= zoom;
            linked_pos.y /= zoom;
            linked_pos.x += (float) size.x / 2;
            linked_pos.y += (float) size.y / 2;
            Vec2 mouse = Vec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
            Vec2 point = find_closest_point_on_line(node_pos + Vec2(0, 24), linked_pos + Vec2(0, 24), mouse);

            //Draw point on line
            ImGui::GetCurrentWindow()->DrawList->AddCircleFilled(ImVec2(point.x, point.y), 4, IM_COL32(255, 255, 255, 255));

            if (ImGui::IsMouseDown(1)) {
                if ((node_pos + Vec2(0, 24)).distance_to(mouse) > 32 &&
                    (linked_pos + Vec2(0, 24)).distance_to(mouse) > 32) {
                    if (point.distance_to(Vec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y)) < 4) {
                        //Remove the link and remove enitty from group
                        i->linked.erase(std::remove(i->linked.begin(), i->linked.end(), n), i->linked.end());
                        n->linked.erase(std::remove(n->linked.begin(), n->linked.end(), i), n->linked.end());
                        if (i->entity != nullptr) {
                            n->group->remove_entity(i->entity.get());
                        } else if (i->group != nullptr) {
                            n->group->remove_group(i->group.get());
                        } else if (i->system != nullptr) {
                            n->group->remove_system(i->system.get());
                        }
                    }
                }
            }
            //Draw point on line
            //ImGui::GetCurrentWindow()->DrawList->AddCircleFilled(ImVec2(point.x, point.y), 4, IM_COL32(255, 255, 255, 255));
            ImGui::GetCurrentWindow()->DrawList->AddLine(ImVec2(node_pos.x, node_pos.y + 24),
                                                         ImVec2(linked_pos.x, linked_pos.y + 24),
                                                         IM_COL32(127, 127, 127, 80), 2);
        }
    }
}

void Window_EntityHierarchy::simulate_node(Node *node)
{
    //Make sure nodes pointer is not null
    if(node->system == nullptr && node->entity == nullptr && node->group == nullptr)
    {
        //Remove the node
        nodes.erase(std::remove(nodes.begin(), nodes.end(), node), nodes.end());
        delete node;
        return;
    }

    node->pos += node->velocity;
    //Eliminate NaN
    if(node->pos.x != node->pos.x)
    {
        node->pos.x = 0;
    }
    if(node->pos.y != node->pos.y)
    {
        node->pos.y = 0;
    }
    if(node->velocity.x != node->velocity.x || node->velocity.y != node->velocity.y)
    {
        node->velocity = Vec2(0, 0);
    }
    node->velocity*=0.8;
    float repulsion = 0.001f;
    float attraction = 0.001f;
    float distance = 128;
    for (auto & i : nodes)
    {
        if(i!=node)
        {
            //Attempt to maintain a distance by 32 pixels
            Vec2 dir = i->pos - node->pos;
            float dist = dir.length();
            if (dist < distance) {
                dir = dir.normalized();
                dir *= (distance - dist) * repulsion;
                node->velocity -= dir;
            }
            if(dist>0)
            {
                dir = dir.normalized();
                dir *= (dist - distance) * attraction;
                node->velocity += dir;
            }
        }
    }
}

void Window_EntityHierarchy::build_graph(GroupRef group, Node* parent)
{
    std::vector<EntityRef> entities = group->get_entities();
    std::vector<GroupRef> groups = group->get_child_groups();
    std::vector<SystemRef> systems = group->get_systems();
    bool group_exists = false;
    Node* group_node;
    for (auto & node : nodes) {
        if(node->group.get() == group.get())
        {
            group_exists = true;
            group_node = node;
            break;
        }
    }
    if(!group_exists)
    {
        group_node = new Node(group);
        print_info("Group node created");
        selected_node = group_node;
        Node::selected = group_node;
        nodes.push_back(group_node);
    }
    //Link the parent
    if(parent != nullptr)
    {
        bool linked = false;
        for (auto & i : parent->linked) {
            if(i == group_node)
            {
                linked = true;
                break;
            }
        }
        if(!linked)
        {
            parent->linked.push_back(group_node);
        }
    }
    std::vector<Node*> linked = group_node->linked;

    //Remove all linked nodes that are not in the group
    for (auto & i : linked) {
        if(i->entity != nullptr)
        {
            if(std::find(entities.begin(), entities.end(), i->entity.get()) == entities.end())
            {
                group_node->linked.erase(std::remove(group_node->linked.begin(), group_node->linked.end(), i), group_node->linked.end());
            }
        }
        else if(i->group != nullptr)
        {
            if(std::find(groups.begin(), groups.end(), i->group.get()) == groups.end())
            {
                group_node->linked.erase(std::remove(group_node->linked.begin(), group_node->linked.end(), i), group_node->linked.end());
            }
        }
        else if(i->system != nullptr)
        {
            if(std::find(systems.begin(), systems.end(), i->system.get()) == systems.end())
            {
                group_node->linked.erase(std::remove(group_node->linked.begin(), group_node->linked.end(), i), group_node->linked.end());
            }
        }
    }

    for (auto & entitie : entities) {
        //Check nodes to see if it already exists
        bool exists = false;
        Node* node = nullptr;
        for (auto & j : nodes) {
            if(j->entity == entitie)
            {
                exists = true;
                node = j;
                break;
            }
        }

        if(!exists)
        {
            node = new Node(entitie);
            nodes.push_back(node);
            selected_node = node;
            Node::selected = node;
            print_info("Entity node created");
        }
        //Check if it's already linked
        bool linked = false;
        for (auto & j : group_node->linked) {
            if(j == node)
            {
                linked = true;
                break;
            }
        }
        if(!linked)
        {
            group_node->linked.push_back(node);
        }
    }
    for (auto & system : systems) {
        //Check nodes to see if it already exists
        bool exists = false;
        Node* node = nullptr;
        for (auto & j : nodes) {
            if(j->system == system)
            {
                exists = true;
                node = j;
                break;
            }
        }
        if(!exists)
        {
            node = new Node(system);
            nodes.push_back(node);
            selected_node = node;
            Node::selected = node;
            print_info("System node created");
        }
        //Check if it's already linked
        bool linked = false;
        for (auto & j : group_node->linked) {
            if(j == node)
            {
                linked = true;
                break;
            }
        }
        if(!linked) {
            group_node->linked.push_back(node);
        }
    }
    for (auto & g : groups)
    {
        if(typeid(*g.get()) == typeid(SceneGroup))
        {
            //Skip the first group as it's the root of the scene group build the graph
            build_graph(g->get_child_groups()[0], group_node);
        }
        else
        {
            build_graph(g, group_node);
        }
    }
}

void Window_EntityHierarchy::delete_node()
{
    if(searching)
        return;
    if(selected_node != nullptr)
    {
        //Delete the entity/group/system from the scene remove the node and all linked nodes
        if(selected_node->entity != nullptr)
        {
            LiveShare::send_delete_entity(selected_node->entity);
            selected_node->entity->force_destroy();
            Editor::action();
        }
        if(selected_node->group != nullptr)
        {
            //Get the nodes parent Group* and remove the group from it
            GroupRef parent = selected_node->group->get_parent();
            if(parent != nullptr)
            {
                Group* group = selected_node->group.get();
                LiveShare::send_delete_group(group);
                parent->remove_group(group);
                group->destroy();
                Editor::action();
            }
        }
        if(selected_node->system != nullptr)
        {
            //Get the node parent Group* and remove the system from it
            //Sort through nodes to find the parent group
            Group* parent;
            for (auto & node : nodes) {
                if(node->group != nullptr)
                {
                    if(node->group->has_system(selected_node->system.get()))
                    {
                        parent = node->group.get();
                        //Remove the system from the parent group
                        parent->remove_system(selected_node->system.get());
                        LiveShare::send_delete_system(selected_node->system);
                        selected_node->system->force_destroy();
                        break;
                    }
                }
            }
            Editor::action();
        }

        //Find parent and set new selected node
        Node* new_selected = nullptr;
        for (auto & node : nodes) {
            for (auto & linked : node->linked) {
                if(linked == selected_node)
                {
                    new_selected = node;
                    break;
                }
            }
        }

        for (auto & node : nodes) {
            for (int j = 0; j < node->linked.size(); ++j) {
                if(node->linked[j] == selected_node)
                {
                    node->linked.erase(node->linked.begin() + j);
                    j--;
                }
            }
        }

        for (int i = 0; i < nodes.size(); ++i) {
            if(nodes[i] == selected_node)
            {
                nodes.erase(nodes.begin() + i);
                i--;
            }
        }
        selected_node = new_selected;
        Node::selected = new_selected;
    }
}

void Window_EntityHierarchy::duplicate()
{
    //Duplicate the entity
    if(selected_node != nullptr)
    {
        if(selected_node->entity != nullptr)
        {
            std::vector<Group*> groups = std::vector<Group*>();
            for (auto & node : nodes) {
                if(node->group != nullptr)
                {
                    bool has = false;
                    for(auto & ent : node->group->get_entities())
                    {
                        if(ent == selected_node->entity)
                        {
                            has = true;
                            break;
                        }
                    }
                    if(has)
                    {
                        groups.push_back(node->group.get());
                    }
                }
            }
            EntityRef entity = selected_node->entity->duplicate();
            //Link it
            for (auto & group : groups) {
                group->add_entity(entity);
            }

            LiveShare::send_entity_full(entity);
            Editor::action();
        }
    }
}

void Window_EntityHierarchy::focus()
{
    ImGui::SetWindowFocus("Graph Hierarchy");
    //Find the root and make it the selected node
    for (auto & node : nodes) {
        if(node->group != nullptr)
        {
            if(node->group->get_parent() == nullptr)
            {
                selected_node = node;
                Node::selected = node;
                break;
            }
        }
    }
}

void Window_EntityHierarchy::move(Vec2 dir)
{
    if (selected_node != nullptr)
    {
        // Calculate the distance from selected_node to each node in the specified direction
        std::vector<Node*> nodes_in_direction;
        for (Node* node : nodes) {
            Vec2 diff = {node->pos.x - selected_node->pos.x, node->pos.y - selected_node->pos.y};
            float dot_product = diff.x * dir.x + diff.y * dir.y;
            if (dot_product > 0) { // Check if the node is in the specified direction
                nodes_in_direction.push_back(node);
            }
        }

        // Find the node with the minimum distance
        Node* closest_node = nullptr;
        float min_distance = INFINITY;
        for (Node* node : nodes_in_direction) {
            Vec2 diff = {node->pos.x - selected_node->pos.x, node->pos.y - selected_node->pos.y};
            float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            if (distance < min_distance) {
                min_distance = distance;
                closest_node = node;
            }
        }

        // Move to the closest node (you need to implement this part)
        if (closest_node != nullptr) {
            // Implement moving to the closest node
            selected_node = closest_node;
            Node::selected = closest_node;
        }
    }
}

void Window_EntityHierarchy::move_right()
{
    move(Vec2(1,0));
}
void Window_EntityHierarchy::move_left()
{
    move(Vec2(-1,0));
}
void Window_EntityHierarchy::move_up()
{
    move(Vec2(0,-1));
}
void Window_EntityHierarchy::move_down()
{
    move(Vec2(0,1));
}

void Window_EntityHierarchy::begin_link()
{
    if(selected_node != nullptr)
    {
        linking = true;
        currently_linking = selected_node;
    }
}


Node::Node(EntityRef entity)
{
    this->pos = Vec2((float)Window_EntityHierarchy::nodes.size(),0);
    this->velocity = Vec2(0, 0);
    this->size = 8;
    this->color = Color((int)EditorTheme::scene_hierarchy_entity.x,(int)EditorTheme::scene_hierarchy_entity.y,(int)EditorTheme::scene_hierarchy_entity.z,255);
    this->texture = ResourceManager::load<Texture>("engine/entity.png");
    this->entity = EntityRef(entity);
    this->system = nullptr;
    this->group = nullptr;
    this->open = true;
}
Node::Node(Pomegranate::SystemRef system)
{
    this->pos = Vec2((float)Window_EntityHierarchy::nodes.size(),0);
    this->velocity = Vec2(0, 0);
    this->size = 8;
    this->color = Color((int)EditorTheme::scene_hierarchy_system.x,(int)EditorTheme::scene_hierarchy_system.y,(int)EditorTheme::scene_hierarchy_system.z,255);
    this->texture = ResourceManager::load<Texture>("engine/system.png");
    this->entity = nullptr;
    this->system = system;
    this->group = nullptr;
    this->open = true;
}
Node::Node(Pomegranate::GroupRef group)
{
    this->pos = Vec2((float)Window_EntityHierarchy::nodes.size(),0);
    this->velocity = Vec2(0, 0);
    this->size = 8;
    this->color = Color((int)EditorTheme::scene_hierarchy_group.x,(int)EditorTheme::scene_hierarchy_group.y,(int)EditorTheme::scene_hierarchy_group.z,255);
    this->texture = ResourceManager::load<Texture>("engine/group.png");
    this->entity = nullptr;
    this->system = nullptr;
    this->group = GroupRef(group);
    this->open = true;
}
