#include "resources_window.h"

ResourcesWindow::ResourcesWindow()
{
    name = "Resources Manager";
    selected_resource = nullptr;
}

void ResourcesWindow::render()
{
    if (ImGui::Button("Reload Resources"))
    {
        load_resources();
        time_since_last_reload = 0.0f;
    }
    time_since_last_reload += delta_time;
    if (time_since_last_reload > 5.0f)
    {
        time_since_last_reload = 0.0f;
        load_resources();
    }
    //Break
    ImGui::Separator();
    //List all resources
    for (auto i = ResourceManager::resources.begin(); i != ResourceManager::resources.end(); i++)
    {
        std::string name = i->first;
        Resource* resource = (Resource*)i->second;
        if (ImGui::Selectable(name.c_str(), selected_resource == resource))
         {
            selected_resource = resource;
        }
        if(ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("resource", &resource, sizeof(Resource *));
            ImGui::Text("%s", name.c_str());
            ImGui::EndDragDropSource();
        }
    }
}

void ResourcesWindow::update()
{

}

void ResourcesWindow::load_resources()
{
    std::vector<std::string> files;
    for (const auto & entry : std::filesystem::directory_iterator("res"))
    {
        files.push_back(entry.path().string());
    }
    //Figure out the types of the files
    for (auto file : files)
    {
        if (file.find(".png") != std::string::npos)
        {
            ResourceManager::load<Texture>(file);
        }
        else if (file.find(".ttf") != std::string::npos)
        {
            ResourceManager::load<TTFFont>(file);
        }
        else if (file.find(".wav") != std::string::npos)
        {
            ResourceManager::load<Audio>(file);
        }
    }
}
