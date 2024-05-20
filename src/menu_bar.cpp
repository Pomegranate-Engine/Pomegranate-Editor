#include "menu_bar.h"
ImGui::FileBrowser openingDialog;
ImGui::FileBrowser savingDialog = ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename);
std::function<void()> save_current_callback;
bool save_scene_modal = false;
bool join_server_modal = false;
void undo();
void redo();
void save();
void open();
void run_scene();
void new_scene();
void force_new_scene();
void force_open();

void undo()
{
    if(Editor::current_scene_index == 0)
    {
        Notify::notify({ResourceManager::load<Texture>("engine/warning.png"),EditorTheme::color_palette_yellow,"Error","No more undo steps"});
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
    Notify::notify({ResourceManager::load<Texture>("engine/check.png"),EditorTheme::color_palette_green,"Undo","Undone"});
}
void redo()
{
    if(Editor::current_scene_index == Editor::history.size()-1)
    {
        Notify::notify({ResourceManager::load<Texture>("engine/warning.png"),EditorTheme::color_palette_yellow,"Error","No more redo steps"});
        return;
    }
    // Redo
    unload_all();
    Editor::current_scene = open_scene_from_json(Editor::history[Editor::current_scene_index + 1]);
    Editor::current_scene_index++;
    if (Editor::current_scene_index > Editor::history.size() - 1) {
        Editor::current_scene_index = (int)Editor::history.size() - 1;
    }
    Notify::notify({ResourceManager::load<Texture>("engine/check.png"),EditorTheme::color_palette_green,"Redo","Redone"});
}
void force_open()
{
    openingDialog.SetTypeFilters({".pscn"});
    openingDialog.SetTitle("Open Scene");
    openingDialog.SetPwd("res/");
    openingDialog.Open();
}
void open()
{
    if(!Editor::current_scene_path.empty())
    {
        save();
        force_open();
    }
    else
    {
        save_scene_modal = true;
        save_current_callback = force_open;
    }
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
        Notify::notify({ResourceManager::load<Texture>("engine/check.png"), EditorTheme::color_palette_green,"Saved","Saved scene to: "+Editor::current_scene_path});
    }
}
void run_scene()
{
    if (!Editor::current_scene_path.empty())
    {
        //Save
        Notify::notify({ResourceManager::load<Texture>("engine/check.png"),EditorTheme::color_palette_green,"Saving","Saving scene"});
        save_scene(Editor::current_scene_path.c_str(), Editor::current_scene);
        //Compile
        Notify::notify({ResourceManager::load<Texture>("engine/check.png"), EditorTheme::color_palette_green,"Compiling","Compiling project"});
        compile_project("Game", Editor::current_scene_path);
        //Run
        Notify::notify({ResourceManager::load<Texture>("engine/check.png"), EditorTheme::color_palette_green,"Running","Starting"});
        run_project();
    }
    else
    {
        Notify::notify({ResourceManager::load<Texture>("engine/error.png"),EditorTheme::color_palette_red,"Error","Please save the scene or open an existing scene"});
    }
}
void force_new_scene()
{
    Notify::notify({ResourceManager::load<Texture>("engine/check.png"),EditorTheme::color_palette_green,"New Scene","Creating new scene"});
    unload_all();
    Editor::current_scene = create_default_scene();
    Editor::current_scene_path = "";
}
void new_scene()
{
    // New scene
    if(!Editor::current_scene_path.empty())
    {
        save();
        force_new_scene();
    }
    else
    {
        save_scene_modal = true;
        save_current_callback = force_new_scene;
    }
}
void draw_menu_bar()
{
    HotkeyManager::add_hotkey({{SDL_SCANCODE_F5},"Run Scene",run_scene});
    //HotkeyManager::add_hotkey({{SDL_SCANCODE_F6},"Run",run}})
    HotkeyManager::add_hotkey({{SDL_SCANCODE_LCTRL,SDL_SCANCODE_S},"Save",save});
    HotkeyManager::add_hotkey({{SDL_SCANCODE_LCTRL,SDL_SCANCODE_O},"Open",open});
    HotkeyManager::add_hotkey({{SDL_SCANCODE_LCTRL,SDL_SCANCODE_Z},"Undo",undo});
    HotkeyManager::add_hotkey({{SDL_SCANCODE_LCTRL,SDL_SCANCODE_Y},"Redo",redo});
    HotkeyManager::add_hotkey({{SDL_SCANCODE_LCTRL,SDL_SCANCODE_N},"New",new_scene});

    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Scene"))
    {
        if (ImGui::MenuItem("New"))
        {
            // New scene
            new_scene();
        }
        if (ImGui::MenuItem("Open","Ctrl+O"))
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
    if(ImGui::BeginMenu("Share"))
    {
        if(ImGui::MenuItem("Begin Sharing"))
        {
            LiveShare::start_server();
        }
        if(ImGui::MenuItem("Stop Sharing"))
        {
            LiveShare::stop_server();
        }
        if(ImGui::MenuItem("Join LiveShare"))
        {
            //Open full screen dialog
            LiveShare::stop_server();
            LiveShare::join_address = "";

            join_server_modal = true;
        }
        if(ImGui::MenuItem("Leave LiveShare"))
        {
            LiveShare::leave_server();
        }
        ImGui::EndMenu();
    }
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("Play").x - ImGui::GetStyle().WindowPadding.x - ImGui::GetStyle().WindowPadding.y);
    if (ImGui::MenuItem("Play","F5")) {
        // Run scene
        run_scene();
    }
    //Play from starting scene
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("Play Start").x - ImGui::GetStyle().WindowPadding.x - ImGui::GetStyle().WindowPadding.y - ImGui::CalcTextSize("Play").x - ImGui::GetStyle().WindowPadding.x - ImGui::GetStyle().WindowPadding.y);
    if (ImGui::MenuItem("Play Start","F6"))
    {
        // Run scene from start
        Notify::notify({ResourceManager::load<Texture>("engine/error.png"),EditorTheme::color_palette_red,"Not implemented","This feature is not implemented"});
    }
    ImGui::EndMainMenuBar();
    savingDialog.Display();
    if (savingDialog.HasSelected())
    {
        std::string path = "res/"+savingDialog.GetSelected().filename().string();
        save_scene(path.c_str(), Editor::current_scene);
        Editor::current_scene_path = path;
        savingDialog.ClearSelected();
        Notify::notify({ResourceManager::load<Texture>("engine/check.png"),EditorTheme::color_palette_green,"Saved","Saved scene to: " + Editor::current_scene_path});
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

    if(save_scene_modal)
    {
        ImGui::OpenPopup("Save Current");
    }

    if(ImGui::BeginPopupModal("Save Current",&save_scene_modal))
    {
        ImGui::Text("Save current scene?");
        if(ImGui::Button("Yes"))
        {
            save();
            ImGui::CloseCurrentPopup();
            save_scene_modal = false;
        }
        ImGui::SameLine();
        if(ImGui::Button("No"))
        {
            save_current_callback();
            ImGui::CloseCurrentPopup();
            save_scene_modal = false;
        }
        ImGui::EndPopup();
    }

    if(join_server_modal)
    {
        ImGui::OpenPopup("Join");
    }

    if(ImGui::BeginPopupModal("Join",&join_server_modal))
    {
        ImGui::Text("Enter address to join");
        static char address[256];
        ImGui::InputText("Address",address,256);
        static char password[256];
        ImGui::InputText("Password",password,256);
        if(ImGui::Button("Join"))
        {
            LiveShare::join_address = address;
            LiveShare::join_password = password;
            LiveShare::join_server();
            ImGui::CloseCurrentPopup();
            join_server_modal = false;
        }
        ImGui::EndPopup();
    }
}