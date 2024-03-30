#include "menu_bar.h"
bool save_key_down = false;
void draw_menu_bar()
{
    if(InputManager::get_key(SDL_SCANCODE_LCTRL) && (InputManager::get_key(SDL_SCANCODE_S) && !save_key_down))
    {
        print_info("Saving scene...");
        save_scene("test.pscn",currently_opened_scene);
    }
    save_key_down = InputManager::get_key(SDL_SCANCODE_S);
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Scene"))
    {
        if (ImGui::MenuItem("New"))
        {
            // New scene
        }
        if (ImGui::MenuItem("Open"))
        {
            unload_all();
            currently_opened_scene = open_scene("test.pscn");
        }
        if(currently_opened_scene!= nullptr) {
            if (ImGui::MenuItem("Save","Ctrl+S")) {
                save_scene("test.pscn",currently_opened_scene);
            }
        }
        ImGui::EndMenu();
    }
if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Undo", "Ctrl+Z"))
        {
            // Undo
        }
        if (ImGui::MenuItem("Redo", "Ctrl+Y"))
        {
            // Redo
        }
        ImGui::EndMenu();
    }


    ImGui::EndMainMenuBar();
}