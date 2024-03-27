#include "menu_bar.h"

void draw_menu_bar()
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
            currently_opened_scene = open_scene("test.pscn");
        }
        if(currently_opened_scene!= nullptr) {
            if (ImGui::MenuItem("Save")) {
                save_scene("test.pscn",currently_opened_scene);
            }
        }
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
}