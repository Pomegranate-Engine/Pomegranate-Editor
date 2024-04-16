#include "resources_window.h"

std::vector<ResourceFile> ResourcesWindow::resource_files;

ResourceTag::ResourceTag(std::string name, Color color, int priority)
{
    this->name = name;
    this->color = color;
    this->priority = priority;
}

void ResourceTag::add_child_tag(ResourceTag tag)
{
    //First ensure that the tag does not already exist
    for (auto i = child_tags.begin(); i != child_tags.end(); i++)
    {
        if (i->name == tag.name)
        {
            return;
        }
    }
    child_tags.push_back(tag);
}

ResourceFile::ResourceFile(std::string path, ResourceType type, std::vector<ResourceTag> tags)
{
    this->path = path;
    this->type = type;
    this->tags = tags;
}

Color ResourceFile::get_color()
{
    //Find tag with highest priority
    ResourceTag highest_priority_tag = tags[0];
    for (auto tag : tags)
    {
        if (tag.priority > highest_priority_tag.priority)
        {
            highest_priority_tag = tag;
        }
    }
    return highest_priority_tag.color;
}

void ResourcesWindow::add_resource_file(ResourceFile file)
{
    //First ensure that the file does not already exist
    for (auto i = resource_files.begin(); i != resource_files.end(); i++)
    {
        if (i->path == file.path)
        {
            return;
        }
    }
    resource_files.push_back(file);
}

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
    for(auto file : resource_files)
    {
        ImGui::PushID(file.path.c_str());
        //Set the color of the text
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4((float)file.get_color().r/255.0f, (float)file.get_color().g/255.0f, (float)file.get_color().b/255.0f, 1.0f));
        ImGui::Selectable(file.path.c_str());
        ImGui::PopStyleColor();

        ImGui::PopID();
    }
}

void ResourcesWindow::update()
{

}

bool ends_with(const std::string& str, const std::string& ending) {
    if (str.length() >= ending.length()) {
        return (str.substr(str.length() - ending.length()) == ending);
    } else {
        return false;
    }
}

void create_meta(std::string path, Vec3 color)
{
    //Create meta file
    std::ofstream meta_file(path + ".meta");
    json j = {
            {"type", "texture"},
            {"color", {1.0f, 1.0f, 1.0f, 1.0f}},
            {"tags", {{{"name", "Texture"}, {"color",{color.x/255.0f, color.y/255.0f, color.z/255.0f, 1.0f}},{"priority", 0}}}}
    };
    meta_file << j.dump(4);
    meta_file.close();
}

std::vector<ResourceTag> read_meta(std::string path)
{
    json j;
    std::ifstream meta_file(path);
    meta_file >> j;
    meta_file.close();
    std::vector<ResourceTag> tags;
    for (auto tag : j["tags"])
    {
        tags.emplace_back(tag["name"], Color(tag["color"][0].get<float>(), tag["color"][1].get<float>(), tag["color"][2].get<float>(), tag["color"][3].get<float>()), tag["priority"].get<int>());
    }
    return tags;
}

void ResourcesWindow::load_resources()
{
    std::vector<std::pair<std::string,std::string>> files;
    for (const auto & entry : std::filesystem::directory_iterator("res"))
    {
        if(ends_with(entry.path().string(),".meta"))
        {
            continue;
        }
        else
        {
            //Check if the file has a meta file
            if (std::filesystem::exists(entry.path().string() + ".meta"))
            {
                files.emplace_back(entry.path().string(), entry.path().string() + ".meta");
            }
            else
            {
                files.emplace_back(entry.path().string(), "");
            }
        }
    }
    //Figure out the types of the files
    for (auto file : files)
    {
        if (file.first.find(".png") != std::string::npos)
        {
            if(file.second == "")
            {
                create_meta(file.first, EditorTheme::color_palette_green);
                file.second = file.first + ".meta";
            }

            add_resource_file({
                file.first,
                ResourceType::RESOURCE_TYPE_TEXTURE,
                read_meta(file.second)
            });
            //Load resource into resource manager
            ResourceManager::load<Texture>(file.first);
        }
        else if (file.first.find(".lua") != std::string::npos)
        {
            if(file.second == "")
            {
                create_meta(file.first, EditorTheme::color_palette_blue);
                file.second = file.first + ".meta";
            }

            add_resource_file({
                                      file.first,
                                      ResourceType::RESOURCE_TYPE_TEXTURE,
                                      read_meta(file.second)
                              });
            //Load resource into resource manager
            ResourceManager::load<Texture>(file.first);
        }
        else if (file.first.find(".wav") != std::string::npos)
        {
            if(file.second == "")
            {
                create_meta(file.first, EditorTheme::color_palette_yellow);
                file.second = file.first + ".meta";
            }

            add_resource_file({
                                      file.first,
                                      ResourceType::RESOURCE_TYPE_AUDIO,
                                      read_meta(file.second)
                              });
            //Load resource into resource manager
            ResourceManager::load<Audio>(file.first);
        }
        else if (file.first.find(".ttf") != std::string::npos)
        {
            if(file.second == "")
            {
                create_meta(file.first, EditorTheme::color_palette_purple);
                file.second = file.first + ".meta";
            }

            add_resource_file({
                                      file.first,
                                      ResourceType::RESOURCE_TYPE_FONT,
                                      read_meta(file.second)
                              });
            //Load resource into resource manager
            ResourceManager::load<TTFFont>(file.first);
        }
        else if (file.first.find(".pscn") != std::string::npos)
        {
            if(file.second == "")
            {
                create_meta(file.first, EditorTheme::color_palette_red);
                file.second = file.first + ".meta";
            }

            add_resource_file({
                                      file.first,
                                      ResourceType::RESOURCE_TYPE_RESOURCE_FILE,
                                      read_meta(file.second)
                              });
            //Load resource into resource manager
        }
    }
}
