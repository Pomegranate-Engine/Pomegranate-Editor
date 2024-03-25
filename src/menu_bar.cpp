#include "menu_bar.h"

std::string save_branch(EntityGroup* group, int depth = 0)
{
    return "";
}

std::string save_scene(EntityGroup* group)
{
    // Save scene
    print_info("Saving scene: %s", group->name.c_str());
    std::string content = save_branch(group);

    return content;
}

void draw_menu_bar(EntityGroup* group)
{
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Scene"))
    {
        if (ImGui::MenuItem("New"))
        {
            // New scene
        }
        if (ImGui::MenuItem("Open"))
        {
            //group = open_scene("test.pscn");
        }
        if(group!= nullptr) {
            if (ImGui::MenuItem("Save")) {
                // Save scene
                std::string content = save_scene(group);
                print_pass(content);
                //Save it to a file called test.pscn
                std::ofstream file("test.pscn");
                file << content;
                file.close();
            }
        }
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
}