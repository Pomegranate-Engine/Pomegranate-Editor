#include "inspector_window.h"

int InspectorWindow::element_index = 0;

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
            std::unordered_multimap<const std::type_info*,Component*> components = entity->get_components();
            element_index = 0;
            std::unordered_set<const std::type_info*> keys;
            for(auto i = components.begin(); i != components.end(); i++)
            {
                Component* component = i->second;
                if(component != nullptr)
                {
                    std::string name = std::string(i->first->name());

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
                        for (auto j = component->component_data.begin(); j != component->component_data.end(); j++) {
                            std::string property_name = std::string(j->first);
                            if (j->second.second != nullptr) {
                                element_index++;
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
                                }
                                else if(j->second.first->hash_code() == typeid(TTFFont*).hash_code())
                                {
                                    auto **value = (TTFFont**)j->second.second;
                                    property_field(property_name.c_str(), value);
                                }
                                else if(j->second.first->hash_code() == typeid(Entity*).hash_code())
                                {
                                    auto **value = (Entity**)j->second.second;
                                    property_field(property_name.c_str(), value);
                                }
                                else if(j->second.first->hash_code() == typeid(std::vector<LuaComponentScript*>).hash_code())
                                {
                                    auto *value = (std::vector<LuaComponentScript*>*)j->second.second;
                                    if(ImGui::CollapsingHeader(property_name.c_str()))
                                    {
                                        for(int i = 0; i < value->size(); i++)
                                        {
                                            //Add imgui tab
                                            ImGui::Text("    ");
                                            ImGui::SameLine();
                                            LuaComponentScript** script = &value->at(i);
                                            property_field((property_name + " [" + std::to_string(i) + "]").c_str(), script);
                                            if(ImGui::IsItemDeactivatedAfterEdit())
                                            {
                                                Editor::action();
                                            }
                                            ImGui::SameLine();
                                            if(ImGui::Button("X"))
                                            {
                                                value->erase(value->begin() + i);
                                                Editor::action();
                                            }
                                        }
                                        if(ImGui::Button("Add"))
                                        {
                                            value->push_back(nullptr);
                                            Editor::action();
                                        }
                                    }
                                }
                                if(ImGui::IsItemDeactivatedAfterEdit())
                                {
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
                for (auto i = Component::component_types.begin(); i != Component::component_types.end(); i++) {
                    if (ImGui::MenuItem(scuffy_demangle(i->first.c_str()).c_str())) {
                        if(entity->add_component(i->first.c_str()) == nullptr)
                        {
                            Notify::notify({ResourceManager::load<Texture>("engine/warning.png"),EditorTheme::color_palette_red,"Component already exists!","Component " + scuffy_demangle(i->first.c_str()) + " already exists on this entity. Not added!"});
                        }
                        Editor::action();
                    }
                }
                ImGui::EndPopup();
            }
        }
        else if(Node::selected->group != nullptr)
        {
            EntityGroup *group = Node::selected->group.get();
            property_field("Name", &group->name);
            if(ImGui::IsItemDeactivatedAfterEdit())
            {
                Editor::action();
            }
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
        if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("resource"))
        {
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
        if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("resource"))
        {
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
            Entity* entity = *(Entity**)payload->Data;
            *value = entity;
            print_info("Entity Dropped");
        }
        ImGui::EndDragDropTarget();
    }
}

void InspectorWindow::property_field(const char *name, LuaComponentScript **value)
{
    //This will be a drag and drop field
    ImGui::Text("%s", name);
    //Create button to open file dialog and be drop target
    ImGui::SameLine();
    bool open = ImGui::CollapsingHeader(*value==nullptr?"None":(*value)->name.c_str());
    if(open)
    {
        //Display component properties
        LuaComponentScript* lua = *value;
        if(lua!=nullptr)
        {
            for (auto [key,value] : lua->component_data)
            {
                if(value.has_value())
                {
                    if(&value.type() == &typeid(double))
                    {
                        float v = (float)std::any_cast<double>(value);
                        property_field(key.c_str(), &v);
                        lua->component_data[key] = (double)v;
                    }
                    else if(&value.type() == &typeid(int))
                    {
                        int v = std::any_cast<int>(value);
                        property_field(key.c_str(), &v);
                        lua->component_data[key] = v;
                    }
                    else if(&value.type() == &typeid(bool))
                    {
                        bool v = std::any_cast<bool>(value);
                        property_field(key.c_str(), &v);
                        lua->component_data[key] = v;
                    }
                    else if(&value.type() == &typeid(std::string))
                    {
                        std::string v = std::any_cast<std::string>(value);
                        property_field(key.c_str(), &v);
                        lua->component_data[key] = v;
                    }
                    else if(&value.type() == &typeid(Vec2))
                    {
                        Vec2 v = std::any_cast<Vec2>(value);
                        property_field(key.c_str(), &v);
                        lua->component_data[key] = v;
                    }
                    else if(&value.type() == &typeid(Vec3))
                    {
                        Vec3 v = std::any_cast<Vec3>(value);
                        property_field(key.c_str(), &v);
                        lua->component_data[key] = v;
                    }
                    else if(&value.type() == &typeid(Color))
                    {
                        Color v = std::any_cast<Color>(value);
                        property_field(key.c_str(), &v);
                        lua->component_data[key] = v;
                    }
                    else if(&value.type() == &typeid(Texture*))
                    {
                        Texture* v = std::any_cast<Texture*>(value);
                        property_field(key.c_str(), &v);
                        lua->component_data[key] = v;
                    }
                    else if(&value.type() == &typeid(TTFFont*))
                    {
                        TTFFont* v = std::any_cast<TTFFont*>(value);
                        property_field(key.c_str(), &v);
                        lua->component_data[key] = v;
                    }
                    else if(&value.type() == &typeid(Entity*))
                    {
                        Entity* v = std::any_cast<Entity*>(value);
                        property_field(key.c_str(), &v);
                        lua->component_data[key] = v;
                    }
                }
            }
        }
    }
    //Drop target
    if(ImGui::BeginDragDropTarget())
    {
        if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("resource"))
        {
            LuaComponentScript* lua = *(LuaComponentScript**)payload->Data;
            lua->run_script();
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
