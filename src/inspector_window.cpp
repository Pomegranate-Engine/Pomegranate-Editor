#include "inspector_window.h"

std::string InspectorWindow::resource_name = "";
ResourceFile* InspectorWindow::resource = nullptr;
int InspectorWindow::element_index = 0;
bool InspectorWindow::something_dropped = false;
std::string InspectorWindow::component_search_buffer = "";

InspectorHeaderTag::InspectorHeaderTag(const char *name)
{
    this->name = name;
}

InspectorWindow::InspectorWindow()
{
    this->name = "Inspector";
}

void InspectorWindow::render()
{
    if(Node::selected!= nullptr)
    {
        HotkeyManager::add_hotkey({{SDL_SCANCODE_LCTRL, SDL_SCANCODE_I}, "Focus Inspector", focus});
        if (Node::selected->entity != nullptr)
        {
            Entity *entity = Node::selected->entity.get();
            property_field("Name", &entity->name);
            if(ImGui::IsItemDeactivatedAfterEdit() || something_dropped)
            {
                LiveShare::send_change_entity_name(entity, entity->name);
                Editor::action();
            }

            std::unordered_multimap<const std::type_info*,Component*> components = entity->get_components();
            element_index = 0;
            std::unordered_set<const std::type_info*> keys;
            for(auto i = components.begin(); i != components.end(); i++)
            {
                Component* component = i->second;
                if(component != nullptr)
                {
                    if(i->first == &typeid(LuaComponent))
                    {
                        LuaComponent* lua_component = (LuaComponent*)component;
                        for(auto& s : lua_component->scripts)
                        {
                            if(s != nullptr)
                            {
                                property_field("", &s);
                            }
                        }
                        continue;
                    }
                    std::string name = std::string(i->first->name());
                    //Check if it is a lua component

                    bool component_open = ImGui::CollapsingHeader((scuffy_demangle(name.c_str())+ "##" + std::to_string(element_index)).c_str(), ImGuiTreeNodeFlags_DefaultOpen);
                    //Add a break
                    if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
                    {
                        ImGui::OpenPopup("ComponentSettingsPopup");
                        right_clicked_component = component;
                    }
                    keys.insert(i->first);
                    if(component_open) {
                        //Display the properties
                        bool header_open = true;
                        for (auto j = component->component_data.begin(); j != component->component_data.end(); j++) {
                            something_dropped = false;
                            std::string property_name = std::string(j->first);
                            if(property_name[0] == '_')
                            {
                                continue;
                            }

                            if (j->second.second != nullptr) {
                                element_index++;
                                if(header_open)
                                {
                                    if (j->second.first->hash_code() == typeid(std::string).hash_code()) {
                                        auto *value = (std::string *) j->second.second;
                                        property_field(property_name.c_str(), value);
                                    } else if (j->second.first->hash_code() == typeid(float).hash_code()) {
                                        auto *value = (float *) j->second.second;
                                        property_field(property_name.c_str(), value);
                                    } else if (j->second.first->hash_code() == typeid(int).hash_code()) {
                                        auto *value = (int *) j->second.second;
                                        property_field(property_name.c_str(), value);
                                    } else if (j->second.first->hash_code() == typeid(bool).hash_code()) {
                                        auto *value = (bool *) j->second.second;
                                        property_field(property_name.c_str(), value);
                                    } else if (j->second.first->hash_code() == typeid(Vec2).hash_code()) {
                                        auto *value = (Vec2 *) j->second.second;
                                        property_field(property_name.c_str(), value);
                                    } else if (j->second.first->hash_code() == typeid(Vec3).hash_code()) {
                                        auto *value = (Vec3 *) j->second.second;
                                        property_field(property_name.c_str(), value);
                                    } else if (j->second.first->hash_code() == typeid(Color).hash_code()) {
                                        auto *value = (Color *) j->second.second;
                                        property_field(property_name.c_str(), value);
                                    } else if (j->second.first->hash_code() == typeid(Texture *).hash_code()) {
                                        auto **value = (Texture **) j->second.second;
                                        property_field(property_name.c_str(), value);
                                    } else if (j->second.first->hash_code() == typeid(TTFFont *).hash_code()) {
                                        auto **value = (TTFFont **) j->second.second;
                                        property_field(property_name.c_str(), value);
                                    } else if (j->second.first->hash_code() == typeid(Entity *).hash_code()) {
                                        auto **value = (Entity **) j->second.second;
                                        property_field(property_name.c_str(), value);
                                    } else if (j->second.first->hash_code() == typeid(std::vector<LuaComponentData *>).hash_code()) {
                                        auto *value = (std::vector<LuaComponentData *> *) j->second.second;
                                        if (ImGui::CollapsingHeader(property_name.c_str())) {
                                            for (int i = 0; i < value->size(); i++) {
                                                //Add imgui tab
                                                ImGui::Text("    ");
                                                ImGui::SameLine();
                                                LuaComponentData **script = &value->at(i);
                                                property_field((property_name + " [" + std::to_string(i) + "]").c_str(),
                                                               script);
                                                if (ImGui::IsItemDeactivatedAfterEdit()) {
                                                    Editor::action();
                                                }
                                                if (ImGui::Button("X")) {
                                                    value->erase(value->begin() + i);
                                                    Editor::action();
                                                }
                                            }
                                            if (ImGui::Button("Add")) {
                                                value->push_back(nullptr);
                                                Editor::action();
                                            }
                                        }
                                    }
                                }



                                if(j->second.first->hash_code() == typeid(InspectorHeaderTag).hash_code())
                                {
                                    if(((InspectorHeaderTag*)j->second.second)->name.empty())
                                    {
                                        //End the header
                                        header_open = true;
                                    }

                                    header_open = ImGui::CollapsingHeader(((InspectorHeaderTag*)j->second.second)->name.c_str(),((InspectorHeaderTag*)j->second.second)->open);
                                }

                                if(ImGui::IsItemDeactivatedAfterEdit() || something_dropped)
                                {
                                    LiveShare::send_change_property(entity, name, property_name, j->second.first->hash_code(), j->second.second);
                                    Editor::action();
                                }
                            }
                        }
                    }

                }
                element_index++;
            }
            if(right_clicked_component!= nullptr)
            {
                if (ImGui::BeginPopup("ComponentSettingsPopup")) {
                    if (ImGui::MenuItem("Remove Component")) {
                        //Remove the component
                        print_info("Removing Component");
                        LiveShare::send_delete_component(entity, typeid(*right_clicked_component).name());
                        entity->remove_component(right_clicked_component);
                    }
                    ImGui::EndPopup();
                }
            }
            //Button to add components
            //It takes up the full width of the window
            ImVec2 size = ImVec2(ImGui::GetContentRegionAvail().x, 0);
            if(ImGui::Button("Add Component",size))
            {
                ImGui::OpenPopup("Add Component");
            }
            if(ImGui::BeginPopup("Add Component"))
            {
                ImGui::InputText("Search", &component_search_buffer, ImGuiInputTextFlags_None);
                for(auto i = Component::component_types.begin(); i != Component::component_types.end(); i++) {
                    std::string lower_case_name = scuffy_demangle(i->first.c_str());
                    std::transform(lower_case_name.begin(), lower_case_name.end(), lower_case_name.begin(), ::tolower);
                    std::string lower_case_search = component_search_buffer;
                    std::transform(lower_case_search.begin(), lower_case_search.end(), lower_case_search.begin(), ::tolower);
                    if (lower_case_name.find(lower_case_search) != std::string::npos) {
                        if (ImGui::MenuItem(scuffy_demangle(i->first.c_str()).c_str())) {
                            Component *component = entity->add_component(i->first.c_str());
                            if (component == nullptr) {
                                Notify::notify({ResourceManager::load<Texture>("engine/warning.png"),
                                                EditorTheme::color_palette_red, "Component already exists!",
                                                "Component " + scuffy_demangle(i->first.c_str()) +
                                                " already exists on this entity. Not added!"});
                            } else {
                                LiveShare::send_add_component(entity, i->first);
                            }
                            Editor::action();
                        }
                    }
                }

                for(auto l = LuaComponentScript::lua_component_types.begin(); l != LuaComponentScript::lua_component_types.end(); l++)
                {
                    std::string lower_case_name = l->first;
                    std::transform(lower_case_name.begin(), lower_case_name.end(), lower_case_name.begin(), ::tolower);
                    std::string lower_case_search = component_search_buffer;
                    std::transform(lower_case_search.begin(), lower_case_search.end(), lower_case_search.begin(), ::tolower);
                    if (lower_case_name.find(lower_case_search) != std::string::npos) {
                        if (ImGui::MenuItem(l->first.c_str())) {
                            LuaComponent* lc = entity->get_component<LuaComponent>();
                            if(lc == nullptr)
                            {
                                lc = entity->add_component<LuaComponent>();
                            }
                            LuaComponentData* component = lc->get_component(l->first);
                            if (component == nullptr) {
                                lc->add_component(l->first);
                            } else {
                                Notify::notify({ResourceManager::load<Texture>("engine/warning.png"),
                                                EditorTheme::color_palette_red, "Component already exists!",
                                                "Component " + l->first +
                                                " already exists on this entity. Not added!"});
                                //LiveShare::send_add_component(entity, l->first);
                            }
                            Editor::action();
                        }
                    }
                }

                ImGui::EndPopup();
            }
        }
        else if(Node::selected->group != nullptr)
        {
            if(typeid(*Node::selected->group.get()).hash_code() == typeid(Group).hash_code()) {
                Group *group = Node::selected->group.get();
                property_field("Name", &group->name);
                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    Editor::action();
                }
            }
            else if(typeid(*Node::selected->group.get()).hash_code() == typeid(AutoGroup).hash_code())
            {
                AutoGroup *group = (AutoGroup*)Node::selected->group.get();
                property_field("Name", &group->name);
                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    Editor::action();
                }
                //Display the component types
                if(ImGui::CollapsingHeader("Component Types"))
                {
                    for(auto i = group->component_types.begin(); i != group->component_types.end(); i++)
                    {
                        ImGui::Text(scuffy_demangle((*i)->name()).c_str());
                    }
                    if(ImGui::Button("Add Component"))
                    {
                        ImGui::OpenPopup("Add Component");
                    }
                }
                if(ImGui::BeginPopup("Add Component"))
                {
                    for (auto i = Component::component_types.begin(); i != Component::component_types.end(); i++) {
                        if (ImGui::MenuItem(scuffy_demangle(i->first.c_str()).c_str())) {
                            group->add_component_type(i->first);
                            Editor::action();
                        }
                    }
                    ImGui::EndPopup();
                }
            }
            else if(typeid(*Node::selected->group.get()).hash_code() == typeid(SceneGroup).hash_code()) {
                Group *group = Node::selected->group.get();
                property_field("Name", &group->name);
                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    Editor::action();
                }
            }
        }
        else if(Node::selected->system != nullptr)
        {
            //Check if it's lua system
            if(typeid(*Node::selected->system.get()).hash_code() == typeid(LuaSystem).hash_code()) {
                LuaSystem *system = (LuaSystem *) Node::selected->system.get();
                property_field("script", &system->script);
            }
        }
    }
    else if(ResourcesWindow::selected_resource_file != nullptr)
    {
        ResourceFile* resource_file = ResourcesWindow::selected_resource_file;
        HotkeyManager::add_hotkey({{SDL_SCANCODE_LCTRL, SDL_SCANCODE_I}, "Focus Inspector", focus});
        if(resource_name.empty() || resource != ResourcesWindow::selected_resource_file)
        {
            resource_name = resource_file->path;
        }
        property_field("Name", &resource_name);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
            //Rename it with the new name
            std::filesystem::rename(resource_file->path, resource_name);
            resource_file->path = resource_name;
            //Rename metadata file
            std::filesystem::rename(resource_file->path + ".meta", resource_name + ".meta");
        }
    }
    else
    {
        //Disable hotkeys
        HotkeyManager::disable_hotkey("Focus Inspector");
    }
}

void InspectorWindow::update()
{

}

std::string InspectorWindow::get_element(std::string name)
{
    return name + "##"+ std::to_string(element_index);
}

void InspectorWindow::property_field(const char *name, std::string* value)
{
    ImGui::InputText(get_element(name).c_str(), value);
}

void InspectorWindow::property_field(const char *name, float* value)
{
    ImGui::InputFloat(get_element(name).c_str(), value);
}

void InspectorWindow::property_field(const char *name, double* value)
{
    ImGui::InputDouble(get_element(name).c_str(), value);
}

void InspectorWindow::property_field(const char *name, int* value)
{
    ImGui::InputInt(get_element(name).c_str(), value);
}

void InspectorWindow::property_field(const char *name, bool* value)
{
    ImGui::Checkbox(get_element(name).c_str(), value);
}
void InspectorWindow::property_field(const char *name, Vec2* value)
{
    float v[2] = {value->x, value->y};
    ImGui::InputFloat2(get_element(name).c_str(), v);
    value->x = v[0];
    value->y = v[1];
}
void InspectorWindow::property_field(const char *name, Vec3* value)
{
    float v[3] = {value->x, value->y, value->z};
    ImGui::InputFloat3(get_element(name).c_str(), v);
    value->x = v[0];
    value->y = v[1];
    value->z = v[2];
}
void InspectorWindow::property_field(const char *name, Color* value)
{
    float color[4] = {(float)value->r/255.0f, (float)value->g/255.0f, (float)value->b/255.0f, (float)value->a/255.0f};
    ImGui::ColorEdit4(get_element(name).c_str(), color);
    value->r = (uint8_t)(color[0]*255);
    value->g = (uint8_t)(color[1]*255);
    value->b = (uint8_t)(color[2]*255);
    value->a = (uint8_t)(color[3]*255);
}

void InspectorWindow::property_field(const char *name, Texture **value)
{
    //This will be a drag and drop field
    ImGui::Text("%s", name);
    //Create button to open file dialog and be drop target
    ImGui::SameLine();
    if(ImGui::Button(*value==nullptr?"None":(*value)->path.c_str()))
    {
        //Open file dialog
        //Set the texture to the selected texture
    }
    //Drop target
    if(ImGui::BeginDragDropTarget())
    {
        if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("resource_texture"))
        {
            InspectorWindow::something_dropped = true;
            Texture* texture = *(Texture**)payload->Data;
            *value = texture;
            print_info("Texture Dropped");
        }
        ImGui::EndDragDropTarget();
    }
}

void InspectorWindow::property_field(const char *name, TTFFont **value)
{
    //This will be a drag and drop field
    ImGui::Text("%s", name);
    //Create button to open file dialog and be drop target
    ImGui::SameLine();
    if(ImGui::Button(*value==nullptr?"None":(*value)->path.c_str()))
    {
        //Open file dialog
        //Set the texture to the selected texture
    }
    //Drop target
    if(ImGui::BeginDragDropTarget())
    {
        if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("resource_texture"))
        {
            InspectorWindow::something_dropped = true;
            TTFFont* ttf = *(TTFFont**)payload->Data;
            *value = ttf;
            print_info("Texture Dropped");
        }
        ImGui::EndDragDropTarget();
    }
}

void InspectorWindow::property_field(const char *name, Entity **value)
{
    //This will be a drag and drop field
    ImGui::Text("%s", name);
    //Create button to open file dialog and be drop target
    ImGui::SameLine();
    if(*value != nullptr) {
        if (ImGui::Button((*value)->name.c_str())) {
            //Open file dialog
            //Set the texture to the selected texture
        }
    }
    else
    {
        if(ImGui::Button("None"))
        {
            //Open file dialog
            //Set the texture to the selected texture
        }
    }
    //Drop target
    if(ImGui::BeginDragDropTarget())
    {
        if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity"))
        {
            InspectorWindow::something_dropped = true;
            Entity* entity = *(Entity**)payload->Data;
            *value = entity;
            print_info("Entity Dropped");
        }
        ImGui::EndDragDropTarget();
    }
}

void InspectorWindow::property_field(const char *name, LuaComponentData **value)
{
    if(!std::string(name).empty())
    {
        //This will be a drag and drop field
        ImGui::Text("%s", name);
        //Create button to open file dialog and be drop target
        ImGui::SameLine();
    }
    bool open = ImGui::CollapsingHeader(*value==nullptr?"None":(*value)->name.c_str());
    //Drop target
    if(ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* pay = ImGui::GetDragDropPayload();
        print_info(pay->DataType);
        if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("resource_lua_component_script"))
        {
            InspectorWindow::something_dropped = true;
            LuaComponentScript* lua = *(LuaComponentScript**)payload->Data;
            lua->run_script();
            *value = new LuaComponentData(lua);
            print_info("Texture Dropped");
        }
        ImGui::EndDragDropTarget();
    }
    if(open)
    {
        //Display component properties
        LuaComponentData* lua = *value;
        if(lua!=nullptr)
        {
            for (auto [key,value] : lua->component_data)
            {
                if(key[0] == '_')
                {
                    continue;
                }
                if(value.second!=nullptr)
                {
                    if(value.first == &typeid(double))
                    {
                        property_field(key.c_str(), (double*)value.second);
                    }
                    if(value.first == &typeid(float))
                    {
                        property_field(key.c_str(), (float*)value.second);
                    }
                    else if(value.first == &typeid(int))
                    {
                        property_field(key.c_str(), (int*)value.second);
                    }
                    else if(value.first == &typeid(bool))
                    {
                        property_field(key.c_str(), (bool*)value.second);
                    }
                    else if(value.first == &typeid(std::string))
                    {
                        property_field(key.c_str(), (std::string*)value.second);
                    }
                    else if(value.first == &typeid(Vec2))
                    {
                        property_field(key.c_str(), (Vec2*)value.second);
                    }
                    else if(value.first == &typeid(Vec3))
                    {
                        property_field(key.c_str(), (Vec3*)value.second);
                    }
                    else if(value.first == &typeid(Color))
                    {
                        property_field(key.c_str(), (Color*)value.second);
                    }
                    else if(value.first == &typeid(Texture*))
                    {
                        property_field(key.c_str(), (Texture**)value.second);
                    }
                    else if(value.first == &typeid(TTFFont*))
                    {
                        property_field(key.c_str(), (TTFFont**)value.second);
                    }
                    else if(value.first == &typeid(Entity*))
                    {
                        property_field(key.c_str(), (Entity**)value.second);
                    }
                }
            }
        }
    }
}

void InspectorWindow::property_field(const char *name, LuaSystemScript **value)
{
    //This will be a drag and drop field
    ImGui::Text("%s", name);
    //Create button to open file dialog and be drop target
    ImGui::SameLine();
    if(ImGui::Button(*value==nullptr?"None":(*value)->path.c_str()))
    {
        //Open file dialog
        //Set the texture to the selected texture
    }
    //Drop target
    if(ImGui::BeginDragDropTarget())
    {
        if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("resource_lua_system_script"))
        {
            InspectorWindow::something_dropped = true;
            LuaSystemScript* lua = *(LuaSystemScript**)payload->Data;
            *value = lua;
            print_info("Texture Dropped");
        }
        ImGui::EndDragDropTarget();
    }
}

void InspectorWindow::focus()
{
    ImGui::SetWindowFocus("Inspector");
}
