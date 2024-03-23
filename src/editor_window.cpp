#include "editor_window.h"

void EditorWindow::begin()
{
    ImGui::Begin(this->name.c_str(), &this->open, flags);
    if(docked)
    {
        ImGui::SetWindowPos(ImVec2(position.x, position.y));
        ImGui::SetWindowSize(ImVec2(size.x, size.y));
    }
    else
    {
        ImGui::SetWindowPos(ImVec2(position.x, position.y), ImGuiCond_FirstUseEver);
        ImGui::SetWindowSize(ImVec2(size.x, size.y), ImGuiCond_FirstUseEver);
    }
}
void EditorWindow::end()
{
    ImGui::End();
}