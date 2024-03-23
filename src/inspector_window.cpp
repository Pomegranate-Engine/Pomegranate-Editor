#include "inspector_window.h"

InspectorWindow::InspectorWindow()
{
    this->name = "Inspector";
}

void InspectorWindow::render()
{
    if(Node::selected!= nullptr)
    {
        if (Node::selected->entity != nullptr)
        {
            Entity *entity = Node::selected->entity.get();
            property_field("Name", &entity->name);
            std::unordered_multimap<const std::type_info*,Component*> components = entity->get_components();
            for(auto i = components.begin(); i != components.end(); i++)
            {
                Component* component = i->second;
                if(component != nullptr)
                {
                    std::string name = std::string(i->first->name());

                    //Add a break
                    ImGui::Separator();
#ifdef __APPLE__
                    ImGui::Text( "%s", abi::__cxa_demangle(name.c_str(), nullptr, nullptr, nullptr));
#else
                    ImGui::Text( "%s", name.c_str());
#endif
                    //Add X button to remove component
                    ImGui::SameLine();
                    //Move it to the end of the line
                    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 40);
                    //Make it square

                    if(ImGui::Button("X", ImVec2(20,20)))
                    {
                        entity->remove_component(component);
                        break;
                    }
                    ImGui::Separator();
                    //Display the properties
                    for(auto j = component->component_data.begin(); j != component->component_data.end(); j++)
                    {
                        std::string property_name = std::string(j->first);
                        if(j->second.second != nullptr)
                        {
                            if(j->second.first->hash_code() == typeid(std::string).hash_code())
                            {
                                std::string* value = (std::string*)j->second.second;
                                property_field(property_name.c_str(), value);
                            }
                            else if(j->second.first->hash_code() == typeid(float).hash_code())
                            {
                                float* value = (float*)j->second.second;
                                property_field(property_name.c_str(), value);
                            }
                            else if(j->second.first->hash_code() == typeid(int).hash_code())
                            {
                                int* value = (int*)j->second.second;
                                property_field(property_name.c_str(), value);
                            }
                            else if(j->second.first->hash_code() == typeid(bool).hash_code())
                            {
                                bool* value = (bool*)j->second.second;
                                property_field(property_name.c_str(), value);
                            }
                            else if(j->second.first->hash_code() == typeid(Vec2).hash_code())
                            {
                                Vec2* value = (Vec2*)j->second.second;
                                property_field(property_name.c_str(), value);
                            }
                            else if(j->second.first->hash_code() == typeid(Vec3).hash_code())
                            {
                                Vec3* value = (Vec3*)j->second.second;
                                property_field(property_name.c_str(), value);
                            }
                            else if(j->second.first->hash_code() == typeid(Color).hash_code())
                            {
                                Color* value = (Color*)j->second.second;
                                property_field(property_name.c_str(), value);
                            }
                            else if(j->second.first->hash_code() == typeid(Texture*).hash_code())
                            {
                                Texture** value = (Texture**)j->second.second;
                                property_field(property_name.c_str(), value);
                            }
                        }
                    }
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
#ifdef __APPLE__
                    if (ImGui::MenuItem(abi::__cxa_demangle(i->first.c_str(), nullptr, nullptr, nullptr))) {
                        entity->add_component(i->first.c_str());
                    }
#else
                    if (ImGui::MenuItem(i->first.c_str())) {
                        entity->add_component(i->first.c_str());
                    }
#endif
                }
                ImGui::EndPopup();
            }
        }
        else if(Node::selected->group != nullptr)
        {
            EntityGroup *group = Node::selected->group.get();
            property_field("Name", &group->name);
        }
    }
}

void InspectorWindow::update()
{

}

void InspectorWindow::property_field(const char *name, std::string* value)
{
    ImGui::InputText(name, value);
}

void InspectorWindow::property_field(const char *name, float* value)
{
    ImGui::InputFloat(name, value);
}

void InspectorWindow::property_field(const char *name, int* value)
{
    ImGui::InputInt(name, value);
}

void InspectorWindow::property_field(const char *name, bool* value)
{
    ImGui::Checkbox(name, value);
}
void InspectorWindow::property_field(const char *name, Vec2* value)
{
    float v[2] = {value->x, value->y};
    ImGui::InputFloat2(name, v);
    value->x = v[0];
    value->y = v[1];
}
void InspectorWindow::property_field(const char *name, Vec3* value)
{
    float v[3] = {value->x, value->y, value->z};
    ImGui::InputFloat3(name, v);
    value->x = v[0];
    value->y = v[1];
    value->z = v[2];
}
void InspectorWindow::property_field(const char *name, Color* value)
{
    float color[4] = {(float)value->r/255.0f, (float)value->g/255.0f, (float)value->b/255.0f, (float)value->a/255.0f};
    ImGui::ColorEdit4(name, color);
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
    if(ImGui::Button("Select Texture"))
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
            value = &texture;
            print_info("Texture Dropped");
        }
        ImGui::EndDragDropTarget();
    }
}
