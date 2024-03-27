#include "entity_hierarchy_window.h"

std::vector<Node*> Window_EntityHierarchy::nodes = std::vector<Node*>();
Node* Node::selected = nullptr;
Node* Window_EntityHierarchy::selected_node = nullptr;
Node* Window_EntityHierarchy::dragging_node = nullptr;

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
        SDL_RenderPoint(renderer, centreX + x, centreY - y);
        SDL_RenderPoint(renderer, centreX + x, centreY + y);
        SDL_RenderPoint(renderer, centreX - x, centreY - y);
        SDL_RenderPoint(renderer, centreX - x, centreY + y);
        SDL_RenderPoint(renderer, centreX + y, centreY - x);
        SDL_RenderPoint(renderer, centreX + y, centreY + x);
        SDL_RenderPoint(renderer, centreX - y, centreY - x);
        SDL_RenderPoint(renderer, centreX - y, centreY + x);

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
        SDL_RenderLine(renderer, centreX - x, centreY - y, centreX + x, centreY - y);
        SDL_RenderLine(renderer, centreX - x, centreY + y, centreX + x, centreY + y);
        SDL_RenderLine(renderer, centreX - y, centreY - x, centreX + y, centreY - x);
        SDL_RenderLine(renderer, centreX - y, centreY + x, centreX + y, centreY + x);

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
    //Clean the graph
    for (int i = 0; i < nodes.size(); ++i) {
        if(nodes[i]->entity == nullptr && nodes[i]->group == nullptr && nodes[i]->system == nullptr)
        {
            nodes.erase(nodes.begin() + i);
            i--;
        }
    }
    build_graph(currently_opened_scene);
    Vec2i tex_size;
    SDL_QueryTexture(graph_texture, NULL, NULL, &tex_size.x, &tex_size.y);

    if(tex_size.x != ImGui::GetWindowWidth() || tex_size.y != ImGui::GetWindowHeight())
    {
        size.x = ImGui::GetWindowWidth();
        size.y = ImGui::GetWindowHeight();
        SDL_DestroyTexture(graph_texture);
        graph_texture = SDL_CreateTexture(Window::current->get_sdl_renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
    }

    SDL_SetRenderTarget(Window::current->get_sdl_renderer(), graph_texture);
    SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 34, 39, 44, 255);
    SDL_RenderClear(Window::current->get_sdl_renderer());

    //Check if window is focused
    if(ImGui::IsWindowFocused()) {
        //Check if mouse is hovering over a node
        bool hovering_node = false;
        for (int i = 0; i < nodes.size(); ++i) {
            //Calculate screen pos
            Vec2 node_pos = nodes[i]->pos;
            node_pos.x -= cam_pos.x;
            node_pos.y -= cam_pos.y;
            node_pos.x /= zoom;
            node_pos.y /= zoom;
            node_pos.x += size.x / 2;
            node_pos.y += size.y / 2;
            //Check if mouse is hovering over node
            Vec2 mouse = InputManager::get_mouse_position()-Vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
            if (mouse.distance_to(node_pos) < nodes[i]->size) {
                hovering_node = true;
                if (InputManager::get_mouse_button(SDL_BUTTON_LEFT) && dragging_node == nullptr) {
                    selected_node = nodes[i];
                    Node::selected = nodes[i];
                    dragging_node = nodes[i];
                }
            }
        }
        if (!hovering_node && dragging_node == nullptr) {
            if (InputManager::get_mouse_button(SDL_BUTTON_LEFT)) {
                cam_pos.x -= InputManager::get_mouse_delta().x * zoom;
                cam_pos.y -= InputManager::get_mouse_delta().y * zoom;
            }
        }
        if (!InputManager::get_mouse_button(SDL_BUTTON_LEFT)) {
            dragging_node = nullptr;
        }

        if (dragging_node != nullptr) {
            Vec2 mouse = InputManager::get_mouse_position()-Vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
            mouse.x -= size.x / 2;
            mouse.y -= size.y / 2;
            mouse.x *= zoom;
            mouse.y *= zoom;
            mouse.x += cam_pos.x;
            mouse.y += cam_pos.y;
            dragging_node->velocity += (mouse - dragging_node->pos) * 0.1;
        }
        if (InputManager::get_mouse_scrolled()) {
            Vec2 old_mouse = InputManager::get_mouse_position()-Vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
            old_mouse.x -= size.x / 2;
            old_mouse.y -= size.y / 2;
            old_mouse.x *= zoom;
            old_mouse.y *= zoom;
            //Divide or multiply zoom based on scroll
            zoom += InputManager::get_mouse_scroll().y * -0.1*zoom;
            Vec2 new_mouse = InputManager::get_mouse_position()-Vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
            new_mouse.x -= size.x / 2;
            new_mouse.y -= size.y / 2;
            new_mouse.x *= zoom;
            new_mouse.y *= zoom;
            cam_pos.x -= (new_mouse.x - old_mouse.x);
            cam_pos.y -= (new_mouse.y - old_mouse.y);
        }
    }



    for (int i = 0; i < nodes.size(); ++i) {
        simulate_node(nodes[i]);
        draw_node(nodes[i]);
    }

    SDL_SetRenderTarget(Window::current->get_sdl_renderer(), NULL);
    ImGui::SetCursorPos(ImVec2(0, 0));
    ImGui::Image((void*)graph_texture, ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()));

    //open Context menu
    if(ImGui::BeginPopupContextWindow())
    {
        if(ImGui::MenuItem("Create Entity")) {
            //Add the entity as child to selected node
            if(selected_node != nullptr)
            {
                if(selected_node->group != nullptr)
                {
                    //Create a new entity
                    Entity *entity = new Entity();
                    entity->name = "New Entity";
                    selected_node->group->add_entity(entity);
                }
            }
        }
        if(ImGui::MenuItem("Create Group")) {
            //Add the group as child to selected node
            if(selected_node != nullptr)
            {
                if(selected_node->group != nullptr)
                {
                    //Create a new group
                    EntityGroup *group = new EntityGroup("New Group");
                    selected_node->group->add_group(group);
                }
            }
        }
        if(ImGui::BeginMenu("Create System")) {
            for (auto i = System::system_types.begin(); i != System::system_types.end(); i++) {
                if (ImGui::MenuItem(scuffy_demangle(i->first.c_str()).c_str())) {
                    //Add the system as child to selected node
                    if(selected_node != nullptr)
                    {
                        if(selected_node->group != nullptr)
                        {
                            selected_node->group->add_system(i->second());
                        }
                    }
                }
            }
            ImGui::EndMenu();
        }
        if(ImGui::MenuItem("Delete Node"))
        {
            if(selected_node != nullptr)
            {
                //Delete the entity/group/system from the scene remove the node and all linked nodes
                if(selected_node->entity != nullptr)
                {
                    selected_node->entity->force_destroy();
                }
                if(selected_node->group != nullptr)
                {
                    //Get the nodes parent EntityGroup* and remove the group from it
                    EntityGroup* parent = selected_node->group->get_parent();
                    if(parent != nullptr)
                    {
                        EntityGroup* group = selected_node->group.get();
                        parent->remove_group(group);
                    }
                }
                if(selected_node->system != nullptr)
                {
                    //Get the node parent EntityGroup* and remove the system from it

                }

                for (int i = 0; i < nodes.size(); ++i) {
                    for (int j = 0; j < nodes[i]->linked.size(); ++j) {
                        if(nodes[i]->linked[j] == selected_node)
                        {
                            nodes[i]->linked.erase(nodes[i]->linked.begin() + j);
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
                selected_node = nullptr;
                Node::selected = nullptr;
            }
        }
        ImGui::EndPopup();
    }
}

void Window_EntityHierarchy::update()
{

}

Window_EntityHierarchy::Window_EntityHierarchy()
{
    this->flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
    this->name = "Graph Hierarchy";
    this->open = true;
    this->docked = false;
    this->position = Vec2i(0, 0);
    this->cam_pos = Vec2(0, 0);
    this->zoom = 1.0;
    this->size = Vec2i(512, Window::current->get_height());
    graph_texture = SDL_CreateTexture(Window::current->get_sdl_renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.x, size.y);
    selected_node = nullptr;
    dragging_node = nullptr;
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

    node_pos.x += size.x / 2;
    node_pos.y += size.y / 2;
    //Calculate the size of the node based on camera zoom
    float node_size = s;

    //Draw the node
    SDL_SetTextureColorMod(n->texture->get_sdl_texture(), color.r, color.g, color.b);
    SDL_RenderTexture(Window::current->get_sdl_renderer(), n->texture->get_sdl_texture(), nullptr, new SDL_FRect{node_pos.x-node_size, node_pos.y-node_size, node_size*2, node_size*2});
    if(selected_node == n)
    {
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 255, 255, 255, 255);
        draw_circle(Window::current->get_sdl_renderer(), node_pos.x, node_pos.y, node_size+4);
    }
    //Draw name
    std::string name = "";
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
    SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 0, 0, 0, 255);
    TTFFont* font = ResourceManager::load<TTFFont>("engine_res/zed_font.ttf");
    SDL_Surface* surface = TTF_RenderText_Solid(font->get_ttf_font(), name.c_str(), {255, 255, 255, 255});
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Window::current->get_sdl_renderer(), surface);
    Vec2i tex_size;
    SDL_QueryTexture(texture, NULL, NULL, &tex_size.x, &tex_size.y);
    SDL_RenderTexture(Window::current->get_sdl_renderer(), texture, nullptr, new SDL_FRect{node_pos.x-tex_size.x/8, node_pos.y+node_size, (float)tex_size.x/4, (float)tex_size.y/4});
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);


    SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 127, 127, 127, 80);
    SDL_SetRenderDrawBlendMode(Window::current->get_sdl_renderer(), SDL_BLENDMODE_BLEND);
    for (int i = 0; i < n->linked.size(); ++i) {
        Vec2 linked_pos = n->linked[i]->pos;
        linked_pos.x -= cam_pos.x;
        linked_pos.y -= cam_pos.y;
        linked_pos.x /= zoom;
        linked_pos.y /= zoom;
        linked_pos.x += size.x / 2;
        linked_pos.y += size.y / 2;
        SDL_RenderLine(Window::current->get_sdl_renderer(), node_pos.x, node_pos.y, linked_pos.x, linked_pos.y);
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
    for (int i = 0; i < nodes.size(); ++i)
    {
        if(nodes[i]!=node)
        {
            //Attempt to maintain a distance by 32 pixels
            Vec2 dir = nodes[i]->pos - node->pos;
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
    Node* group_node = nullptr;
    for (int i = 0; i < nodes.size(); ++i) {
        if(nodes[i]->group.get() == group)
        {
            group_exists = true;
            group_node = nodes[i];
            break;
        }
    }
    if(!group_exists)
    {
        group_node = new Node(group);
        print_info("Group node created");
        nodes.push_back(group_node);
    }
    //Link the parent
    if(parent != nullptr)
    {
        bool linked = false;
        for (int i = 0; i < parent->linked.size(); ++i) {
            if(parent->linked[i] == group_node)
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
    for (int i = 0; i < entities.size(); ++i) {
        //Check nodes to see if it already exists
        bool exists = false;
        Node* node = nullptr;
        for (int j = 0; j < nodes.size(); ++j) {
            if(nodes[j]->entity.get() == entities[i])
            {
                exists = true;
                node = nodes[j];
                break;
            }
        }

        if(!exists)
        {
            node = new Node(entities[i]);
            nodes.push_back(node);
            print_info("Entity node created");
        }
        //Check if it's already linked
        bool linked = false;
        for (int j = 0; j < group_node->linked.size(); ++j) {
            if(group_node->linked[j] == node)
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
    for (int i = 0; i < systems.size(); ++i) {
        //Check nodes to see if it already exists
        bool exists = false;
        Node* node = nullptr;
        for (int j = 0; j < nodes.size(); ++j) {
            if(nodes[j]->system.get() == systems[i])
            {
                exists = true;
                node = nodes[j];
                break;
            }
        }
        if(!exists)
        {
            node = new Node(systems[i]);
            nodes.push_back(node);
            print_info("System node created");
        }
        //Check if it's already linked
        bool linked = false;
        for (int j = 0; j < group_node->linked.size(); ++j) {
            if(group_node->linked[j] == node)
            {
                linked = true;
                break;
            }
        }
        if(!linked) {
            group_node->linked.push_back(node);
        }
    }
    for (int i = 0; i < groups.size(); ++i) {
        build_graph(groups[i], group_node);
    }
}

Color colors[] = {
        {214,92,92,255},
        {92,214,92,255},
        {92,92,214,255},
        {214,173,92,255},
};

Color rand_color()
{
    return colors[rand()%4];
}

Node::Node(Entity *entity)
{
    this->pos = Vec2(Window_EntityHierarchy::nodes.size(),0);
    this->velocity = Vec2(0, 0);
    this->size = 8;
    this->color = colors[2];
    this->texture = ResourceManager::load<Texture>("engine_res/entity.png");
    this->entity = std::unique_ptr<Entity>(entity);
    this->system = nullptr;
    this->group = nullptr;
    this->open = true;
}
Node::Node(Pomegranate::System *system)
{
    this->pos = Vec2(Window_EntityHierarchy::nodes.size(),0);
    this->velocity = Vec2(0, 0);
    this->size = 8;
    this->color = colors[0];
    this->texture = ResourceManager::load<Texture>("engine_res/system.png");
    this->entity = nullptr;
    this->system = std::unique_ptr<System>(system);
    this->group = nullptr;
    this->open = true;
}
Node::Node(Pomegranate::EntityGroup *group)
{
    this->pos = Vec2(Window_EntityHierarchy::nodes.size(),0);
    this->velocity = Vec2(0, 0);
    this->size = 8;
    this->color = colors[3];
    this->texture = ResourceManager::load<Texture>("engine_res/group.png");
    this->entity = nullptr;
    this->system = nullptr;
    this->group = std::unique_ptr<EntityGroup>(group);
    this->open = true;
}
