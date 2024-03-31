#include "menu_bar.h"
bool save_key_down = false;
bool undo_key_down = false;
bool redo_key_down = false;
void undo()
{
    if(Editor::current_scene_index == 0)
    {
        return;
    }
    // Undo
    unload_all();
    Editor::current_scene = open_scene_from_json(Editor::history[Editor::current_scene_index-1]);
    Editor::current_scene_index--;
    if(Editor::current_scene_index < 0)
    {
        Editor::current_scene_index = 0;
    }
}
void redo()
{
    if(Editor::current_scene_index == Editor::history.size()-1)
    {
        return;
    }
    // Redo
    unload_all();
    Editor::current_scene = open_scene_from_json(Editor::history[Editor::current_scene_index + 1]);
    Editor::current_scene_index++;
    if (Editor::current_scene_index > Editor::history.size() - 1) {
        Editor::current_scene_index = Editor::history.size() - 1;
    }
}
void draw_menu_bar()
{
    if(InputManager::get_key(SDL_SCANCODE_LCTRL) && (InputManager::get_key(SDL_SCANCODE_S) && !save_key_down))
    {
        print_info("Saving scene...");
        save_scene("test.pscn",Editor::current_scene);
    }
    save_key_down = InputManager::get_key(SDL_SCANCODE_S);

    if(InputManager::get_key(SDL_SCANCODE_LCTRL) && (InputManager::get_key(SDL_SCANCODE_Z) && !undo_key_down))
    {
        undo();
    }
    undo_key_down = InputManager::get_key(SDL_SCANCODE_Z);

    if(InputManager::get_key(SDL_SCANCODE_LCTRL) && (InputManager::get_key(SDL_SCANCODE_Y) && !redo_key_down))
    {
        redo();
    }
    redo_key_down = InputManager::get_key(SDL_SCANCODE_Y);

    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Scene"))
    {
        if (ImGui::MenuItem("New"))
        {
            // New scene
            unload_all();
            Editor::current_scene = new EntityGroup("root");
        }
        if (ImGui::MenuItem("Open"))
        {
            unload_all();
            Editor::current_scene = open_scene("test.pscn");
        }
        if(Editor::current_scene!= nullptr) {
            if (ImGui::MenuItem("Save","Ctrl+S")) {
                save_scene("test.pscn",Editor::current_scene);
            }
        }
        ImGui::EndMenu();
    }
if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Undo", "Ctrl+Z"))
        {
            // Undo
            undo();
        }
        if (ImGui::MenuItem("Redo", "Ctrl+Y"))
        {
            // Redo
            redo();
        }
        ImGui::EndMenu();
    }


    ImGui::EndMainMenuBar();
}