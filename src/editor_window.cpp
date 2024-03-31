#include "editor_window.h"

void EditorWindow::begin()
{
    //Get sdl2 window size
    int w, h;
    SDL_GetWindowSize(Window::current->get_sdl_window(), &w, &h);

    int f = flags;
    if(dock_slot != DOCK_SLOT_FLOATING)
    {
        f |= ImGuiWindowFlags_NoCollapse;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        if(dock_slot == DOCK_SLOT_LEFT)
        {
            ImGui::SetNextWindowPos(ImVec2(0, 24));
            ImGui::SetNextWindowSize(ImVec2((float)w/5, (float)h-32));
        }
        if(dock_slot == DOCK_SLOT_RIGHT)
        {
            ImGui::SetNextWindowPos(ImVec2((float)w/5*4, 24));
            ImGui::SetNextWindowSize(ImVec2((float)w/5, (float)h-32));
        }
        if(dock_slot == DOCK_SLOT_CENTER)
        {
            ImGui::SetNextWindowPos(ImVec2((float)w/5, 24));
            ImGui::SetNextWindowSize(ImVec2((float)w/5*3, (float)h/5*3));
        }
        if(dock_slot == DOCK_SLOT_BOTTOM)
        {
            ImGui::SetNextWindowPos(ImVec2((float)w/5, (float)h/5*3+24));
            ImGui::SetNextWindowSize(ImVec2((float)w/5*3, (float)h/5*2-32));
        }
    }
    ImGui::Begin(this->name.c_str(), &this->open, f);
    if(dock_slot != DOCK_SLOT_FLOATING)
    {
        ImGui::PopStyleVar();
    }

    // Docking
    //Check if mouse is over the window title bar
    if(ImGui::IsItemHovered())
    {
        if (!InputManager::get_mouse_button(1))
        {
            if (!mouse_released)
            {
                printf("No longer dragged\n");
                Vec2 window_pos = Vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
                //left
                if (window_pos.x < (float)w/10)
                {
                    ImGui::SetWindowPos(ImVec2(0, 24));
                    ImGui::SetWindowSize(ImVec2((float)w/5, (float)h-32));
                    dock_slot = DOCK_SLOT_LEFT;
                }
                //right
                if(window_pos.x > (float)w/10*9)
                {
                    ImGui::SetWindowPos(ImVec2((float)w/5*4, 24));
                    ImGui::SetWindowSize(ImVec2((float)w/5, (float)h-32));
                    dock_slot = DOCK_SLOT_RIGHT;
                }
                //Center middle
                if(window_pos.x > (float)w/5 && window_pos.x < (float)w/5*4 && window_pos.y < (float)h/5 && window_pos.y > 0)
                {
                    ImGui::SetWindowPos(ImVec2((float)w/5, 24));
                    ImGui::SetWindowSize(ImVec2((float)w/5*3, (float)h/5*3));
                    dock_slot = DOCK_SLOT_CENTER;
                }
                //Bottom middle
                if(window_pos.x > (float)w/5 && window_pos.y > (float)h/5*4)
                {
                    ImGui::SetWindowPos(ImVec2((float)w/5, (float)h/5*3+24));
                    ImGui::SetWindowSize(ImVec2((float)w/5*3, (float)h/5*2-32));
                    dock_slot = DOCK_SLOT_BOTTOM;
                }
            }
            mouse_released = true;
        } else {
            dock_slot = DOCK_SLOT_FLOATING;
            mouse_released = false;
        }
    }
}
void EditorWindow::end()
{
    ImGui::End();
}