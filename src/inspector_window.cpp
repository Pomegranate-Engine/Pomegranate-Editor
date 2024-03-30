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
            Component* right_clicked_component = nullptr;
            for(auto i = components.begin(); i != components.end(); i++)
            {
                Component* component = i->second;
                if(component != nullptr)
                {
                    std::string name = std::string(i->first->name());

                    bool component_open = ImGui::CollapsingHeader(scuffy_demangle(name.c_str()).c_str(), ImGuiTreeNodeFlags_DefaultOpen);
                    //Add a break
                    if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
                    {
                        ImGui::OpenPopup("ComponentSettingsPopup");
                        right_clicked_component = component;
                    }
                    if(component_open) {
                        //Display the properties
                        for (auto j = component->component_data.begin(); j != component->component_data.end(); j++) {
                            std::string property_name = std::string(j->first);
                            if (j->second.second != nullptr) {
                                if (j->second.first->hash_code() == typeid(std::string).hash_code()) {
                                    std::string *value = (std::string *) j->second.second;
                                    property_field(property_name.c_str(), value);
                                } else if (j->second.first->hash_code() == typeid(float).hash_code()) {
                                    float *value = (float *) j->second.second;
                                    property_field(property_name.c_str(), value);
                                } else if (j->second.first->hash_code() == typeid(int).hash_code()) {
                                    int *value = (int *) j->second.second;
                                    property_field(property_name.c_str(), value);
                                } else if (j->second.first->hash_code() == typeid(bool).hash_code()) {
                                    bool *value = (bool *) j->second.second;
                                    property_field(property_name.c_str(), value);
                                } else if (j->second.first->hash_code() == typeid(Vec2).hash_code()) {
                                    Vec2 *value = (Vec2 *) j->second.second;
                                    property_field(property_name.c_str(), value);
                                } else if (j->second.first->hash_code() == typeid(Vec3).hash_code()) {
                                    Vec3 *value = (Vec3 *) j->second.second;
                                    property_field(property_name.c_str(), value);
                                } else if (j->second.first->hash_code() == typeid(Color).hash_code()) {
                                    Color *value = (Color *) j->second.second;
                                    property_field(property_name.c_str(), value);
                                } else if (j->second.first->hash_code() == typeid(Texture *).hash_code()) {
                                    Texture **value = (Texture **) j->second.second;
                                    property_field(property_name.c_str(), value);
                                }
                            }
                        }
                    }

                }
            }
            if(ImGui::BeginPopup("ComponentSettingsPopup"))
            {
                if(ImGui::MenuItem("Remove Component"))
                {
                    //Remove the component
                    entity->remove_component(right_clicked_component);
                }
                ImGui::EndPopup();
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
                        entity->add_component(i->first.c_str());
                    }
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
    if(ImGui::Button((*value)->path.c_str()))
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
