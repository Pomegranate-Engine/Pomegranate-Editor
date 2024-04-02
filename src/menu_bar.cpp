#include "menu_bar.h"
bool save_key_down = false;
bool undo_key_down = false;
bool redo_key_down = false;
ImGui::FileBrowser openingDialog;
ImGui::FileBrowser savingDialog = ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename);
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
        Editor::current_scene_index = (int)Editor::history.size() - 1;
    }
}
void open()
{
    openingDialog.SetTypeFilters({".pscn"});
    openingDialog.SetTitle("Open Scene");
    openingDialog.SetPwd("res/");
    openingDialog.Open();
}
void save()
{
    if(Editor::current_scene_path.empty()) {
        //Make sure can't leave res folder
        savingDialog.SetTypeFilters({".pscn"});
        savingDialog.SetTitle("Save Scene");
        savingDialog.SetPwd("res/");
        savingDialog.Open();
    }
    else
    {
        save_scene(Editor::current_scene_path.c_str(), Editor::current_scene);
    }
}

void draw_menu_bar()
{
    if(InputManager::get_key(SDL_SCANCODE_LCTRL) && (InputManager::get_key(SDL_SCANCODE_S) && !save_key_down))
    {
        save();
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
            Editor::current_scene_path = "";
        }
        if (ImGui::MenuItem("Open"))
        {
            // Open scene
            open();
        }
        if(Editor::current_scene!= nullptr) {
            if (ImGui::MenuItem("Save","Ctrl+S")) {
                save();
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
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("Play").x - ImGui::GetStyle().WindowPadding.x - ImGui::GetStyle().WindowPadding.y);
    if (ImGui::MenuItem("Play","F5")) {
        if (!Editor::current_scene_path.empty())
        {
            //Save
            save_scene(Editor::current_scene_path.c_str(), Editor::current_scene);
            compile_project("Game", Editor::current_scene_path);
            run_project();
        }
    }
    //Play from starting scene
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("Play Start").x - ImGui::GetStyle().WindowPadding.x - ImGui::GetStyle().WindowPadding.y - ImGui::CalcTextSize("Play").x - ImGui::GetStyle().WindowPadding.x - ImGui::GetStyle().WindowPadding.y);
    if (ImGui::MenuItem("Play Start","F6"))
    {
        // Run scene from start
    }
    ImGui::EndMainMenuBar();
    savingDialog.Display();
    if (savingDialog.HasSelected())
    {
        std::string path = "res/"+savingDialog.GetSelected().filename().string();
        save_scene(path.c_str(), Editor::current_scene);
        print_info("Saved scene to: %s", path.c_str());
        Editor::current_scene_path = path;
        savingDialog.ClearSelected();
    }
    openingDialog.Display();
    if (openingDialog.HasSelected())
    {
        unload_all();
        std::string path = "res/"+openingDialog.GetSelected().filename().string();
        Editor::current_scene = open_scene(path.c_str());
        Editor::current_scene_path = path;
        openingDialog.ClearSelected();
    }
}