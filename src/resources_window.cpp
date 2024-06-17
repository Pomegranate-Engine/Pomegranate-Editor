#include "resources_window.h"

std::string ResourcesWindow::add_tag;
ImVec4 ResourcesWindow::add_tag_color = ImVec4(255,255,255,255);
std::vector<ResourceFile> ResourcesWindow::resource_files;
ResourceFile* ResourcesWindow::selected_resource_file;
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
    if(tags.size() == 0)
    {
        return Color(255,255,255,255);
    }
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
    LiveShare::send_query_file(file.path);
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
    load_resources();
}

std::string resource_type_2_string(ResourceType type)
{
    switch (type)
    {
        case ResourceType::RESOURCE_TYPE_TEXTURE:
            return "Texture";
        case ResourceType::RESOURCE_TYPE_AUDIO:
            return "Audio";
        case ResourceType::RESOURCE_TYPE_LUA_COMPONENT_SCRIPT:
            return "Lua Component Script";
        case ResourceType::RESOURCE_TYPE_LUA_SYSTEM_SCRIPT:
            return "Lua System Script";
        case ResourceType::RESOURCE_TYPE_FONT:
            return "Font";
        case ResourceType::RESOURCE_TYPE_SHADER:
            return "Shader";
        case ResourceType::RESOURCE_TYPE_MESH:
            return "Mesh";
        case ResourceType::RESOURCE_TYPE_MATERIAL:
            return "Material";
        case ResourceType::RESOURCE_TYPE_SCENE:
            return "Scene";
        case ResourceType::RESOURCE_TYPE_PREFAB:
            return "Prefab";
        case ResourceType::RESOURCE_TYPE_SPRITE_SHEET:
            return "Sprite Sheet";
        case ResourceType::RESOURCE_TYPE_ANIMATION:
            return "Animation";
        case ResourceType::RESOURCE_TYPE_TILEMAP:
            return "Tilemap";
        case ResourceType::RESOURCE_TYPE_TILESET:
            return "Tileset";
        case ResourceType::RESOURCE_TYPE_RESOURCE_FILE:
            return "Resource File";
    }
}

void create_meta(ResourceFile* file)
{
    //Create meta file
    std::ofstream meta_file(file->path + ".meta");
    meta_file.clear();
    json j = {
            {"type", resource_type_2_string(file->type)},
            {"color", {file->get_color().r/255.0f,file->get_color().g/255.0f,file->get_color().b/255.0f, 1.0f}},
            {"tags", {{{"name", resource_type_2_string(file->type)}, {"color",{file->get_color().r/255.0f,file->get_color().g/255.0f,file->get_color().b/255.0f, 1.0f}},{"priority", 0}}}}
    };

    for(auto & tag : file->tags)
    {
        j["tags"].push_back({{"name", tag.name}, {"color",{tag.color.r/255.0f,tag.color.g/255.0f,tag.color.b/255.0f, 1.0f}},{"priority", tag.priority}});
    }

    meta_file << j.dump(4);
    meta_file.close();
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
    for (int i = 0; i < search_tags.size(); i++)
    {
        ImGui::SetNextItemWidth(128);
        if(ImGui::BeginCombo((std::string("##tags") + std::to_string(i)).c_str(), search_tags[i].c_str()))
        {
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
        if(a.tags.size() == 0 || b.tags.size() == 0)
        {
            return a.path < b.path;
        }
        if(a.tags[0].name == b.tags[0].name)
        {
            return a.path < b.path;
        }
        return a.tags[0].name < b.tags[0].name;
    });


    //List all resources
    int i = 0;
    for(auto& file : resource_files)
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
        ImGui::Selectable(file.path.c_str(),&file == selected_resource_file, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(ImGui::GetWindowWidth()/2-64, 24));

        ImGui::PopStyleColor();

        if(ImGui::BeginPopupContextItem("##context"))
        {
            //Make button right click

            if(ImGui::BeginMenu("Tags"))
            {
                bool add = ImGui::InputText("Add Tag:##tag_name", &add_tag, ImGuiInputTextFlags_EnterReturnsTrue);
                if (add) {
                    ResourceTag tag(add_tag, Color(add_tag_color.x,add_tag_color.y,add_tag_color.z,1.0f), 1);
                    file.tags.push_back(tag);
                    add_tag = "";
                    create_meta(&file);
                }

                bool is_input_text_active = ImGui::IsItemActive();
                bool is_input_text_activated = ImGui::IsItemActivated();

                if (is_input_text_activated)
                    ImGui::OpenPopup("##auto_complete");

                {
                    ImGui::SetNextWindowPos(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y));
                    //ImGui::SetNextWindowSize({ ImGui::GetItemRectSize().x, 0 });
                    if (ImGui::BeginPopup("##auto_complete", ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ChildWindow))
                    {
                        for (int i = 0; i < tags.size(); i++)
                        {
                            //if (strstr(autocomplete[i], input) == NULL)
                            //    continue;
                            if(tags[i].contains(add_tag))
                            {
                                if (ImGui::Selectable(tags[i].c_str()))
                                {
                                    ImGui::ClearActiveID();
                                    add_tag = tags[i];
                                }
                            }
                        }

                        if (add || (!is_input_text_active && !ImGui::IsWindowFocused()))
                            ImGui::CloseCurrentPopup();

                        ImGui::EndPopup();
                    }
                }
                float col[3] = {add_tag_color.x,add_tag_color.y,add_tag_color.z};
                ImGui::ColorPicker3("##tag_color", col);
                add_tag_color.x = col[0];
                add_tag_color.y = col[1];
                add_tag_color.z = col[2];

                for (auto tag : file.tags)
                {
                    if (ImGui::MenuItem(tag.name.c_str()))
                    {
                        for(auto i = file.tags.begin(); i != file.tags.end(); i++)
                        {
                            if(i->name == tag.name)
                            {
                                file.tags.erase(i);
                                break;
                            }
                        }
                        create_meta(&file);
                        //Reopen the popup
                        ImGui::OpenPopup("##context");
                    }
                }
                ImGui::EndMenu();
            }
            if(ImGui::MenuItem("Delete"))
            {
                std::filesystem::remove(file.path);
                std::filesystem::remove(file.path + ".meta");
            }
            ImGui::EndPopup();
        }

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

        if(ImGui::IsItemHovered() && !ImGui::IsMouseDragging(0) && ImGui::IsMouseReleased(0))
        {
            selected_resource_file = &file;
            Window_EntityHierarchy::selected_node = nullptr;
            Node::selected = nullptr;
        }

        //Begin drag and drop source
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
            if(file.type == ResourceType::RESOURCE_TYPE_TEXTURE)
            {
                Texture *texture = ResourceManager::load<Texture>(file.path);
                ImGui::SetDragDropPayload("resource_texture", &texture, sizeof(Texture **));
                ImGui::Text(file.path.c_str());
                ImGui::SameLine();
                ImGui::Image((ImTextureID) file.icon->get_sdl_texture(), ImVec2(24, 24));
                ImGui::EndDragDropSource();
            }
            else if(file.type == ResourceType::RESOURCE_TYPE_AUDIO)
            {
                Audio *audio = ResourceManager::load<Audio>(file.path);
                ImGui::SetDragDropPayload("resource_audio", &audio, sizeof(Audio **));
                ImGui::Text(file.path.c_str());
                ImGui::SameLine();
                ImGui::Image((ImTextureID) file.icon->get_sdl_texture(), ImVec2(24, 24));
                ImGui::EndDragDropSource();
            }
            else if(file.type == ResourceType::RESOURCE_TYPE_FONT)
            {
                TTFFont *font = ResourceManager::load<TTFFont>(file.path);
                ImGui::SetDragDropPayload("resource_font", &font, sizeof(TTFFont **));
                ImGui::Text(file.path.c_str());
                ImGui::SameLine();
                ImGui::Image((ImTextureID) file.icon->get_sdl_texture(), ImVec2(24, 24));
                ImGui::EndDragDropSource();
            }
            else if(file.type == ResourceType::RESOURCE_TYPE_SCENE)
            {
                ImGui::SetDragDropPayload("resource_scene", file.path.c_str(), file.path.size());
                ImGui::Text(file.path.c_str());
                ImGui::SameLine();
                ImGui::Image((ImTextureID) file.icon->get_sdl_texture(), ImVec2(24, 24));
                ImGui::EndDragDropSource();
            }
            else if(file.type == ResourceType::RESOURCE_TYPE_LUA_SYSTEM_SCRIPT)
            {
                LuaSystemScript *script = ResourceManager::load<LuaSystemScript>(file.path);
                ImGui::SetDragDropPayload("resource_lua_system_script", &script, sizeof(LuaSystemScript **) );
                ImGui::Text(file.path.c_str());
                ImGui::SameLine();
                ImGui::Image((ImTextureID) file.icon->get_sdl_texture(), ImVec2(24, 24));
                ImGui::EndDragDropSource();
            }
            else if(file.type == ResourceType::RESOURCE_TYPE_LUA_COMPONENT_SCRIPT)
            {
                LuaSystemScript *script = ResourceManager::load<LuaSystemScript>(file.path);
                ImGui::SetDragDropPayload("resource_lua_component_script", &script, sizeof(LuaSystemScript **) );
                ImGui::Text(file.path.c_str());
                ImGui::SameLine();
                ImGui::Image((ImTextureID) file.icon->get_sdl_texture(), ImVec2(24, 24));
                ImGui::EndDragDropSource();
            }
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

std::vector<ResourceTag> read_meta(std::string path)
{
    if(path.empty())
        return {};
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
            std::vector<ResourceTag> tags = read_meta(file.second);
            if(tags.size() == 0)
            {
                ResourceTag tag("Texture", Color(EditorTheme::color_palette_green.x/255.0f,EditorTheme::color_palette_green.y/255.0f,EditorTheme::color_palette_green.z/255.0f,1.0f), 0);
                tags.push_back(tag);
            }
            ResourceFile f = {
                    file.first,
                    ResourceType::RESOURCE_TYPE_TEXTURE,
                    tags,
                    ResourceManager::load<Texture>("engine/image.png")
            };
            if(file.second == "")
            {
                create_meta(&f);
                file.second = file.first + ".meta";
            }

            add_resource_file(f);

            //Load resource into resource manager
            ResourceManager::load<Texture>(file.first);
        }
        else if (file.first.find(".lua") != std::string::npos)
        {
            std::vector<ResourceTag> tags = read_meta(file.second);
            if(tags.size() == 0)
            {
                ResourceTag tag("Script", Color(EditorTheme::color_palette_blue.x/255.0f,EditorTheme::color_palette_blue.y/255.0f,EditorTheme::color_palette_blue.z/255.0f,1.0f), 0);
                tags.push_back(tag);
            }
            //Read the file
            std::string first_line = "";
            std::ifstream lua_file(file.first);
            if(lua_file.is_open())
            {
                std::getline(lua_file, first_line);
                lua_file.close();
            }

            ResourceType type = ResourceType::RESOURCE_TYPE_LUA_COMPONENT_SCRIPT;

            if(first_line.starts_with("--system"))
            {
                type = ResourceType::RESOURCE_TYPE_LUA_SYSTEM_SCRIPT;
            }
            else if(first_line.starts_with("--component"))
            {
                type = ResourceType::RESOURCE_TYPE_LUA_COMPONENT_SCRIPT;
            }

            ResourceFile f = {
                    file.first,
                    type,
                    tags,
                    ResourceManager::load<Texture>("engine/system.png")
            };
            if(file.second == "")
            {
                create_meta(&f);
                file.second = file.first + ".meta";
            }

            add_resource_file(f);
            //Load resource into resource manager
            if(type == ResourceType::RESOURCE_TYPE_LUA_SYSTEM_SCRIPT)
            {
                ResourceManager::load<LuaSystemScript>(file.first);
            }
            else if(type == ResourceType::RESOURCE_TYPE_LUA_COMPONENT_SCRIPT)
            {
                ResourceManager::load<LuaComponentScript>(file.first);
            }
        }
        else if (file.first.find(".wav") != std::string::npos)
        {
            std::vector<ResourceTag> tags = read_meta(file.second);
            if(tags.size() == 0)
            {
                ResourceTag tag("Audio", Color(EditorTheme::color_palette_red.x/255.0f,EditorTheme::color_palette_red.y/255.0f,EditorTheme::color_palette_red.z/255.0f,1.0f), 0);
                tags.push_back(tag);
            }
            ResourceFile f = {
                    file.first,
                    ResourceType::RESOURCE_TYPE_AUDIO,
                    tags,
                    ResourceManager::load<Texture>("engine/headphones.png")
            };
            if(file.second == "")
            {
                create_meta(&f);
                file.second = file.first + ".meta";
            }

            add_resource_file(f);
            //Load resource into resource manager
            ResourceManager::load<Audio>(file.first);
        }
        else if (file.first.find(".ttf") != std::string::npos)
        {
            std::vector<ResourceTag> tags = read_meta(file.second);
            if(tags.size() == 0)
            {
                ResourceTag tag("Font", Color(EditorTheme::color_palette_green.x/255.0f,EditorTheme::color_palette_green.y/255.0f,EditorTheme::color_palette_green.z/255.0f,1.0f), 0);
                tags.push_back(tag);
            }
            ResourceFile f = {
                    file.first,
                    ResourceType::RESOURCE_TYPE_FONT,
                    tags,
                    ResourceManager::load<Texture>("engine/font.png")
            };

            if(file.second == "")
            {
                create_meta(&f);
                file.second = file.first + ".meta";
            }

            add_resource_file(f);
            //Load resource into resource manager
            ResourceManager::load<TTFFont>(file.first);
        }
        else if (file.first.find(".pscn") != std::string::npos)
        {
            std::vector<ResourceTag> tags = read_meta(file.second);
            if(tags.size() == 0)
            {
                ResourceTag tag("Scene", Color(EditorTheme::color_palette_white.x/255.0f,EditorTheme::color_palette_white.y/255.0f,EditorTheme::color_palette_white.z/255.0f,1.0f), 0);
                tags.push_back(tag);
            }
            ResourceFile f = {
                    file.first,
                    ResourceType::RESOURCE_TYPE_SCENE,
                    tags,
                    ResourceManager::load<Texture>("engine/scene.png")};
            if(file.second == "")
            {
                create_meta(&f);
                file.second = file.first + ".meta";
            }

            add_resource_file(f);
            //Load resource into resource manager
        }
    }
}
