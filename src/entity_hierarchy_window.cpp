#include "entity_hierarchy_window.h"

std::vector<Node*> Window_EntityHierarchy::nodes = std::vector<Node*>();
Node* Node::selected = nullptr;
Node* Window_EntityHierarchy::selected_node = nullptr;
Node* Window_EntityHierarchy::dragging_node = nullptr;
bool create_system_popup = false;
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
    HotkeyManager::add_hotkey({{SDL_SCANCODE_LCTRL, SDL_SCANCODE_H}, "Focus hierarchy", focus});
    if(ImGui::IsWindowFocused())
    {
        HotkeyManager::add_hotkey({{SDL_SCANCODE_LSHIFT, SDL_SCANCODE_E}, "Create Entity", create_entity});
        HotkeyManager::add_hotkey({{SDL_SCANCODE_LSHIFT, SDL_SCANCODE_G}, "Create Group", create_group});
        HotkeyManager::add_hotkey({{SDL_SCANCODE_LSHIFT, SDL_SCANCODE_S}, "Create System", create_system});
        HotkeyManager::add_hotkey({{SDL_SCANCODE_BACKSPACE}, "Delete Node", delete_node});
        HotkeyManager::add_hotkey({{SDL_SCANCODE_DELETE}, "Delete Node", delete_node});
        HotkeyManager::add_hotkey({{SDL_SCANCODE_D, SDL_SCANCODE_LSHIFT}, "Duplicate", duplicate});
    }
    else
    {
        HotkeyManager::disable_hotkey("Create Entity");
        HotkeyManager::disable_hotkey("Create Group");
        HotkeyManager::disable_hotkey("Create System");
        HotkeyManager::disable_hotkey("Delete Node");
        HotkeyManager::disable_hotkey("Duplicate");
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

    //Check if window is focused
    if(ImRect(ImGui::GetWindowPos(),ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(),ImGui::GetWindowPos().y + ImGui::GetWindowHeight())).Contains(ImVec2(InputManager::get_mouse_position().x,InputManager::get_mouse_position().y))){
        //Check if mouse is hovering over a node

        bool hovering_node = false;
        if(!InputManager::get_key(SDL_SCANCODE_LSHIFT))
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
                Vec2 mouse =
                        InputManager::get_mouse_position() - Vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
                if (mouse.distance_to(node_pos) < node->size) {
                    hovering_node = true;
                    if (InputManager::get_mouse_button(SDL_BUTTON_LEFT) && dragging_node == nullptr) {
                        selected_node = node;
                        Node::selected = node;
                        dragging_node = node;
                    }
                }
            }
        }
        if (!hovering_node && dragging_node == nullptr) {
            if (InputManager::get_mouse_button(1) && !InputManager::get_key(SDL_SCANCODE_LSHIFT)) {
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
    //open Context menu
    if(ImGui::BeginPopupContextWindow())
    {
        if(selected_node != nullptr)
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
        for (auto i = System::system_types.begin(); i != System::system_types.end(); i++) {
            if (ImGui::MenuItem(scuffy_demangle(i->first.c_str()).c_str())) {
                //Add the system as child to selected node
                if(selected_node != nullptr)
                {
                    if(selected_node->group != nullptr)
                    {
                        selected_node->group->add_system(i->second());
                        Editor::action();
                        create_system_popup = false;
                    }
                    else
                    {
                        //Find parents
                        std::vector<EntityGroup*> parents;
                        for (auto & node : nodes) {
                            if (node->group != nullptr) {
                                if (std::find(node->group->get_entities()->begin(), node->group->get_entities()->end(),
                                              selected_node->entity.get()) != node->group->get_entities()->end()) {
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
}

void Window_EntityHierarchy::create_entity()
{
    //Add the entity as child to selected node
    if(selected_node != nullptr)
    {
        if(selected_node->group != nullptr)
        {
            //Create a new entity
            auto *entity = new Entity();
            entity->name = "New Entity";
            selected_node->group->add_entity(entity);
            Editor::action();
        }
        else
        {
            //Find parents
            std::vector<EntityGroup*> parents;
            for (auto & node : nodes) {
                if (node->group != nullptr) {
                    if (std::find(node->group->get_entities()->begin(), node->group->get_entities()->end(),
                                  selected_node->entity.get()) != node->group->get_entities()->end()) {
                        parents.push_back(node->group.get());
                    }
                }
            }
            //Create a new entity and link it to the parents
            auto *entity = new Entity();
            entity->name = "New Entity";
            for (auto & parent : parents) {
                parent->add_entity(entity);
            }
            Editor::action();
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
            auto *group = new EntityGroup("New Group");
            selected_node->group->add_group(group);
            Editor::action();
        }
        else
        {
            //Find parents
            std::vector<EntityGroup*> parents;
            for (auto & node : nodes) {
                if (node->group != nullptr) {
                    if (std::find(node->group->get_entities()->begin(), node->group->get_entities()->end(),
                                  selected_node->entity.get()) != node->group->get_entities()->end()) {
                        parents.push_back(node->group.get());
                    }
                }
            }
            //Create a new group and link it to the parents
            auto *group = new EntityGroup("New Group");
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
    this->flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
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
void Window_EntityHierarchy::draw_node(Node* n)
{
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

    ImGui::SetCursorPos(ImVec2(node_pos.x-node_size, node_pos.y-node_size));
    ImageRotated((void*)n->texture->get_sdl_texture(),ImVec2(node_pos.x, node_pos.y+node_size*3) ,ImVec2(node_size*2, node_size*2), ImVec4(color.r/255.0f, color.g/255.0f, color.b/255.0f, 1.0f),n->velocity.x*0.05f);
    //Make it DragDrop for the inspector
    ImGui::SetItemAllowOverlap();
    ImGui::SetCursorPos(ImVec2(node_pos.x-node_size, node_pos.y-node_size));
    if(InputManager::get_key(SDL_SCANCODE_LSHIFT))
    {
        ImGui::InvisibleButton((std::string("##") + (n->entity != nullptr ? n->entity->name : n->group != nullptr ? n->group->name : n->system != nullptr ? scuffy_demangle(typeid(*n->system).name()) : "")).c_str(), ImVec2(node_size * 2, node_size * 2));
        if (ImGui::BeginDragDropSource()) {
            if(n->entity != nullptr)
                ImGui::SetDragDropPayload("Entity", &n->entity, sizeof(Entity *));
            if(n->group != nullptr)
                ImGui::SetDragDropPayload("Group", &n->group, sizeof(EntityGroup *));
            if(n->system != nullptr)
                ImGui::SetDragDropPayload("System", &n->system, sizeof(System *));
            ImGui::Image((void *) n->texture->get_sdl_texture(), ImVec2(node_size * 2, node_size * 2), ImVec2(0, 0),
                         ImVec2(1, 1), ImVec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f));
            ImGui::EndDragDropSource();
        }
    }
    //Draw name
    std::string name;
    if(n->group != nullptr)
    {
        name = n->group->name;
    }
    if(n->entity != nullptr)
    {
        name = n->entity->name;
    }
    if(n->system != nullptr)
    {
        name = scuffy_demangle(typeid(*n->system).name());
    }

    ImGui::SetCursorPos(ImVec2(node_pos.x-(float)name.size()*4, node_pos.y+node_size));
    ImGui::Text(name.c_str());

    for (auto & i : n->linked) {
        Vec2 linked_pos = i->pos;
        linked_pos.x -= cam_pos.x;
        linked_pos.y -= cam_pos.y;
        linked_pos.x /= zoom;
        linked_pos.y /= zoom;
        linked_pos.x += (float)size.x / 2;
        linked_pos.y += (float)size.y / 2;
        ImGui::GetCurrentWindow()->DrawList->AddLine(ImVec2(node_pos.x, node_pos.y + 24), ImVec2(linked_pos.x, linked_pos.y + 24), IM_COL32(127, 127, 127, 80), 2);
    }
}

void Window_EntityHierarchy::simulate_node(Node *node)
{
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

void Window_EntityHierarchy::build_graph(EntityGroup *group, Node* parent)
{
    std::vector<Entity*> entities = *group->get_entities();
    std::vector<EntityGroup*> groups = *group->get_child_groups();
    std::vector<System*> systems = *group->get_systems();
    bool group_exists = false;
    Node* group_node;
    for (auto & node : nodes) {
        if(node->group.get() == group)
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
    else
    {
        group_node->open = false;
    }
    for (auto & entitie : entities) {
        //Check nodes to see if it already exists
        bool exists = false;
        Node* node = nullptr;
        for (auto & j : nodes) {
            if(j->entity.get() == entitie)
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
            if(j->system.get() == system)
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
    for (auto & g : groups) {
        build_graph(g, group_node);
    }
}

void Window_EntityHierarchy::delete_node()
{
    if(selected_node != nullptr)
    {
        //Delete the entity/group/system from the scene remove the node and all linked nodes
        if(selected_node->entity != nullptr)
        {
            selected_node->entity->force_destroy();
            Editor::action();
        }
        if(selected_node->group != nullptr)
        {
            //Get the nodes parent EntityGroup* and remove the group from it
            EntityGroup* parent = selected_node->group->get_parent();
            if(parent != nullptr)
            {
                EntityGroup* group = selected_node->group.get();
                parent->remove_group(group);
                Editor::action();
            }
        }
        if(selected_node->system != nullptr)
        {
            //Get the node parent EntityGroup* and remove the system from it
            //Sort through nodes to find the parent group
            EntityGroup* parent;
            for (auto & node : nodes) {
                if(node->group != nullptr)
                {
                    if(node->group->has_system(selected_node->system.get()))
                    {
                        parent = node->group.get();
                        //Remove the system from the parent group
                        parent->remove_system(selected_node->system.get());
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
            std::vector<EntityGroup*> groups = std::vector<EntityGroup*>();
            for (auto & node : nodes) {
                if(node->group != nullptr)
                {
                    if(std::find(node->group->get_entities()->begin(),
                                 node->group->get_entities()->end(),selected_node->entity.get()) != node->group->get_entities()->end())
                    {
                        groups.push_back(node->group.get());
                    }
                }
            }
            Entity* entity = selected_node->entity->duplicate();
            //Link it
            for (auto & group : groups) {
                group->add_entity(entity);
            }
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


Node::Node(Entity *entity)
{
    this->pos = Vec2((float)Window_EntityHierarchy::nodes.size(),0);
    this->velocity = Vec2(0, 0);
    this->size = 8;
    this->color = Color((int)EditorTheme::scene_hierarchy_entity.x,(int)EditorTheme::scene_hierarchy_entity.y,(int)EditorTheme::scene_hierarchy_entity.z,255);
    this->texture = ResourceManager::load<Texture>("engine/entity.png");
    this->entity = std::unique_ptr<Entity>(entity);
    this->system = nullptr;
    this->group = nullptr;
    this->open = true;
}
Node::Node(Pomegranate::System *system)
{
    this->pos = Vec2((float)Window_EntityHierarchy::nodes.size(),0);
    this->velocity = Vec2(0, 0);
    this->size = 8;
    this->color = Color((int)EditorTheme::scene_hierarchy_system.x,(int)EditorTheme::scene_hierarchy_system.y,(int)EditorTheme::scene_hierarchy_system.z,255);
    this->texture = ResourceManager::load<Texture>("engine/system.png");
    this->entity = nullptr;
    this->system = std::unique_ptr<System>(system);
    this->group = nullptr;
    this->open = true;
}
Node::Node(Pomegranate::EntityGroup *group)
{
    this->pos = Vec2((float)Window_EntityHierarchy::nodes.size(),0);
    this->velocity = Vec2(0, 0);
    this->size = 8;
    this->color = Color((int)EditorTheme::scene_hierarchy_group.x,(int)EditorTheme::scene_hierarchy_group.y,(int)EditorTheme::scene_hierarchy_group.z,255);
    this->texture = ResourceManager::load<Texture>("engine/group.png");
    this->entity = nullptr;
    this->system = nullptr;
    this->group = std::unique_ptr<EntityGroup>(group);
    this->open = true;
}
