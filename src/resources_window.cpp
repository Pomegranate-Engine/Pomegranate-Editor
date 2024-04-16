#include "resources_window.h"

std::vector<ResourceFile> ResourcesWindow::resource_files;
std::string ResourcesWindow::search;
bool ResourcesWindow::searching;
std::vector<std::string> ResourcesWindow::search_tags;

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

ResourceFile::ResourceFile(std::string path, ResourceType type, std::vector<ResourceTag> tags, Texture *icon)
{
    this->path = path;
    this->type = type;
    this->tags = tags;
    this->icon = icon;
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
    if (ImGui::Button("Reload"))
    {
        load_resources();
        time_since_last_reload = 0.0f;
    }
    ImGui::SameLine();
    ImGui::InputText("##Search", &search);
    std::transform(search.begin(), search.end(), search.begin(), ::tolower);

    for (int i = 0; i < search_tags.size(); i++)
    {
        ImGui::SetNextItemWidth(128);
        if(ImGui::BeginCombo((std::string("##tags") + std::to_string(i)).c_str(), search_tags[i].c_str()))
        {
            std::vector<std::string> tags;
            for(auto file : resource_files)
            {
                for(auto tag : file.tags)
                {
                    if(std::find(tags.begin(), tags.end(), tag.name) == tags.end())
                    {
                        tags.push_back(tag.name);
                    }
                }
            }
            for(auto tag : tags)
            {
                bool is_selected = (search_tags[i] == tag);
                if(ImGui::Selectable(tag.c_str(), is_selected))
                {
                    search_tags[i] = tag;
                }
                if(is_selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();

        }
        if(ImGui::IsItemClicked(1))
        {
            search_tags.erase(search_tags.begin() + i);
            i--;
        }
        ImGui::SameLine();
    }
    ImGui::SetNextItemWidth(128);
    if(ImGui::Button("      +      "))
    {
        search_tags.push_back("");
    }

    time_since_last_reload += delta_time;
    if (time_since_last_reload > 5.0f)
    {
        time_since_last_reload = 0.0f;
        load_resources();
    }
    //Break
    ImGui::Separator();

    //Sort files based on tag name, and then file name/path
    std::sort(resource_files.begin(), resource_files.end(), [](ResourceFile a, ResourceFile b)
    {
        if(a.tags[0].name == b.tags[0].name)
        {
            return a.path < b.path;
        }
        return a.tags[0].name < b.tags[0].name;
    });


    //List all resources
    int i = 0;
    for(auto file : resource_files)
    {

        std::string lower_case_path = file.path;
        std::transform(lower_case_path.begin(), lower_case_path.end(), lower_case_path.begin(), ::tolower);
        //Check if the search string is in the file path
        if (!search.empty() && lower_case_path.find(search) == std::string::npos)
        {
            continue;
        }


        //Filter tags
        bool has_tag = false;
        for(auto tag : file.tags)
        {
            if(search_tags.size() == 0)
            {
                has_tag = true;
                break;
            }
            for(auto search_tag : search_tags)
            {
                if(tag.name == search_tag)
                {
                    has_tag = true;
                    break;
                }
            }
        }
        if(!has_tag)
        {
            continue;
        }

        ImGui::PushID(file.path.c_str());
        //Set the color of the text
        ImGui::PushStyleColor(ImGuiCol_Text,
                              ImVec4((float) file.get_color().r / 255.0f, (float) file.get_color().g / 255.0f,
                                     (float) file.get_color().b / 255.0f, 1.0f));
        ImGui::Image((ImTextureID) file.icon->get_sdl_texture(), ImVec2(24, 24));
        ImGui::SameLine();
        ImGui::Selectable(file.path.c_str(),false, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(ImGui::GetWindowWidth()/2-64, 24));

        ImGui::PopStyleColor();

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
        {
            print_info("Double clicked");
            if(file.type == ResourceType::RESOURCE_TYPE_SCENE)
            {
                print_info("Opening scene");
                unload_all();
                Editor::current_scene = open_scene(file.path.c_str());
                Editor::current_scene_path = file.path;
            }
        }

        //Begin drag and drop source
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
            Texture *texture = ResourceManager::load<Texture>(file.path);
            ImGui::SetDragDropPayload("resource", &texture, sizeof(Texture **));
            ImGui::Text(file.path.c_str());
            ImGui::EndDragDropSource();
        }
        ImGui::PopID();
        i++;
        if(i%2!=0)
        {
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth()/2, ImGui::GetCursorPosY()));
        }
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

void create_meta(std::string path, std::string type, Vec3 color)
{
    //Create meta file
    std::ofstream meta_file(path + ".meta");
    json j = {
            {"type", "texture"},
            {"color", {1.0f, 1.0f, 1.0f, 1.0f}},
            {"tags", {{{"name", type}, {"color",{color.x/255.0f, color.y/255.0f, color.z/255.0f, 1.0f}},{"priority", 0}}}}
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
                create_meta(file.first,"texture", EditorTheme::color_palette_green);
                file.second = file.first + ".meta";
            }

            add_resource_file({
                file.first,
                ResourceType::RESOURCE_TYPE_TEXTURE,
                read_meta(file.second),
                ResourceManager::load<Texture>("engine/image.png")
            });
            //Load resource into resource manager
            ResourceManager::load<Texture>(file.first);
        }
        else if (file.first.find(".lua") != std::string::npos)
        {
            if(file.second == "")
            {
                create_meta(file.first, "script", EditorTheme::color_palette_blue);
                file.second = file.first + ".meta";
            }

            add_resource_file({
                                      file.first,
                                      ResourceType::RESOURCE_TYPE_TEXTURE,
                                      read_meta(file.second),
                                      ResourceManager::load<Texture>("engine/system.png")
                              });
            //Load resource into resource manager
            ResourceManager::load<Texture>(file.first);
        }
        else if (file.first.find(".wav") != std::string::npos)
        {
            if(file.second == "")
            {
                create_meta(file.first, "audio", EditorTheme::color_palette_yellow);
                file.second = file.first + ".meta";
            }

            add_resource_file({
                                      file.first,
                                      ResourceType::RESOURCE_TYPE_AUDIO,
                                      read_meta(file.second),
                                      ResourceManager::load<Texture>("engine/headphones.png")
                              });
            //Load resource into resource manager
            ResourceManager::load<Audio>(file.first);
        }
        else if (file.first.find(".ttf") != std::string::npos)
        {
            if(file.second == "")
            {
                create_meta(file.first, "font", EditorTheme::color_palette_purple);
                file.second = file.first + ".meta";
            }

            add_resource_file({
                                      file.first,
                                      ResourceType::RESOURCE_TYPE_FONT,
                                      read_meta(file.second),
                                      ResourceManager::load<Texture>("engine/font.png")
                              });
            //Load resource into resource manager
            ResourceManager::load<TTFFont>(file.first);
        }
        else if (file.first.find(".pscn") != std::string::npos)
        {
            if(file.second == "")
            {
                create_meta(file.first, "scene", EditorTheme::color_palette_red);
                file.second = file.first + ".meta";
            }

            add_resource_file({
                                      file.first,
                                      ResourceType::RESOURCE_TYPE_SCENE,
                                      read_meta(file.second),
                              ResourceManager::load<Texture>("engine/scene.png")});
            //Load resource into resource manager
        }
    }
}
