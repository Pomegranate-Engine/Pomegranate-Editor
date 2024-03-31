#include "theme.h"

float EditorTheme::rounding = 0.0f;
Vec3 EditorTheme::editor_text = Vec3(0.00f, 0.00f, 0.00f);
Vec3 EditorTheme::editor_title_bar = Vec3(0.00f, 0.00f, 0.00f);
Vec3 EditorTheme::editor_elements = Vec3(0.00f, 0.00f, 0.00f);
Vec3 EditorTheme::editor_inputs = Vec3(0.00f, 0.00f, 0.00f);
Vec3 EditorTheme::editor_hover = Vec3(0.00f, 0.00f, 0.00f);
Vec3 EditorTheme::editor_popups = Vec3(0.00f, 0.00f, 0.00f);
Vec3 EditorTheme::editor_body = Vec3(0.00f, 0.00f, 0.00f);
Vec3 EditorTheme::scene_view_y = Vec3(0.00f, 0.00f, 0.00f);
Vec3 EditorTheme::scene_view_x = Vec3(0.00f, 0.00f, 0.00f);
Vec3 EditorTheme::scene_view_background = Vec3(0.00f, 0.00f, 0.00f);
Vec3 EditorTheme::scene_hierarchy_entity = Vec3(0.00f, 0.00f, 0.00f);
Vec3 EditorTheme::scene_hierarchy_group = Vec3(0.00f, 0.00f, 0.00f);
Vec3 EditorTheme::scene_hierarchy_system = Vec3(0.00f, 0.00f, 0.00f);
Vec3 EditorTheme::color_palette_red = Vec3(0.00f, 0.00f, 0.00f);
Vec3 EditorTheme::color_palette_yellow = Vec3(0.00f, 0.00f, 0.00f);
Vec3 EditorTheme::color_palette_green = Vec3(0.00f, 0.00f, 0.00f);
Vec3 EditorTheme::color_palette_blue = Vec3(0.00f, 0.00f, 0.00f);
Vec3 EditorTheme::color_palette_purple = Vec3(0.00f, 0.00f, 0.00f);
Vec3 EditorTheme::color_palette_white = Vec3(0.00f, 0.00f, 0.00f);

void theming(Vec3 color_for_text, Vec3 color_for_head, Vec3 color_for_body, Vec3 color_for_pops, Vec3 color_for_hover, Vec3 color_for_inputs,  Vec3 color_for_elements)
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_Text] = ImVec4( color_for_text.x, color_for_text.y, color_for_text.z, 1.00f );
    style.Colors[ImGuiCol_TextDisabled] = ImVec4( color_for_text.x, color_for_text.y, color_for_text.z, 0.58f );
    style.Colors[ImGuiCol_WindowBg] = ImVec4( color_for_body.x, color_for_body.y, color_for_body.z, 1.0f );
    style.Colors[ImGuiCol_Border] = ImVec4( color_for_hover.x, color_for_hover.y, color_for_hover.z, 1.00f );
    style.Colors[ImGuiCol_BorderShadow] = ImVec4( color_for_body.x, color_for_body.y, color_for_body.z, 0.00f );
    style.Colors[ImGuiCol_FrameBg] = ImVec4( color_for_inputs.x, color_for_inputs.y, color_for_inputs.z, 1.00f );
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4( color_for_hover.x, color_for_hover.y, color_for_hover.z, 1.0f );
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ImGuiCol_TitleBg] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.75f );
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.47f );
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4( color_for_elements.x, color_for_elements.y, color_for_elements.z, 1.00f );
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.21f );
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4( color_for_hover.x, color_for_hover.y, color_for_hover.z, 1.0f );
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ImGuiCol_CheckMark] = ImVec4( color_for_elements.x, color_for_elements.y, color_for_elements.z, 0.80f );
    style.Colors[ImGuiCol_SliderGrab] = ImVec4( color_for_elements.x, color_for_elements.y, color_for_elements.z, 0.50f );
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4( color_for_elements.x, color_for_elements.y, color_for_elements.z, 1.00f );
    style.Colors[ImGuiCol_Button] = ImVec4( color_for_elements.x, color_for_elements.y, color_for_elements.z, 0.50f );
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4( color_for_hover.x, color_for_hover.y, color_for_hover.z, 1.0f );
    style.Colors[ImGuiCol_ButtonActive] = ImVec4( color_for_elements.x, color_for_elements.y, color_for_elements.z, 1.00f );
    style.Colors[ImGuiCol_Header] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.76f );
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4( color_for_hover.x, color_for_hover.y, color_for_hover.z, 1.0f );
    style.Colors[ImGuiCol_HeaderActive] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.15f );
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4( color_for_hover.x, color_for_hover.y, color_for_hover.z, 1.0f );
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 1.00f );
    style.Colors[ImGuiCol_PlotLines] = ImVec4( color_for_text.x, color_for_text.y, color_for_text.z, 0.63f );
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4( color_for_hover.x, color_for_hover.y, color_for_hover.z, 1.0f );
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4( color_for_text.x, color_for_text.y, color_for_text.z, 0.63f );
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4( color_for_hover.x, color_for_hover.y, color_for_hover.z, 1.0f );
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4( color_for_head.x, color_for_head.y, color_for_head.z, 0.43f );
    style.Colors[ImGuiCol_PopupBg] = ImVec4( color_for_pops.x, color_for_pops.y, color_for_pops.z, 0.92f );
}

Vec3 color_from_hex(const char* hex)
{
    unsigned int hex_color = std::stoul(hex, nullptr, 16);
    Vec3 color;
    color.x = ((hex_color >> 16) & 0xFF);
    color.y = ((hex_color >> 8) & 0xFF);
    color.z = (hex_color & 0xFF);
    return color;
}

void EditorTheme::load(const char* path)
{
    std::ifstream f(path);
    if(f.is_open())
    {
        json j = json::parse(f);
        
        EditorTheme::rounding = j["editor"]["rounding"].get<float>();
        EditorTheme::editor_text = color_from_hex(j["editor"]["text"].get<std::string>().c_str());
        EditorTheme::editor_title_bar = color_from_hex(j["editor"]["title_bar"].get<std::string>().c_str());
        EditorTheme::editor_body = color_from_hex(j["editor"]["body"].get<std::string>().c_str());
        EditorTheme::editor_popups = color_from_hex(j["editor"]["popups"].get<std::string>().c_str());
        EditorTheme::editor_hover = color_from_hex(j["editor"]["hover"].get<std::string>().c_str());
        EditorTheme::editor_inputs = color_from_hex(j["editor"]["inputs"].get<std::string>().c_str());
        EditorTheme::editor_elements = color_from_hex(j["editor"]["elements"].get<std::string>().c_str());
        EditorTheme::scene_hierarchy_entity = color_from_hex(j["scene_hierarchy"]["entity"].get<std::string>().c_str());
        EditorTheme::scene_hierarchy_group = color_from_hex(j["scene_hierarchy"]["group"].get<std::string>().c_str());
        EditorTheme::scene_hierarchy_system = color_from_hex(j["scene_hierarchy"]["system"].get<std::string>().c_str());
        EditorTheme::scene_view_y = color_from_hex(j["scene_view"]["y"].get<std::string>().c_str());
        EditorTheme::scene_view_x = color_from_hex(j["scene_view"]["x"].get<std::string>().c_str());
        EditorTheme::scene_view_background = color_from_hex(j["scene_view"]["background"].get<std::string>().c_str());
        EditorTheme::color_palette_red = color_from_hex(j["color_palette"]["red"].get<std::string>().c_str());
        EditorTheme::color_palette_yellow = color_from_hex(j["color_palette"]["yellow"].get<std::string>().c_str());
        EditorTheme::color_palette_green = color_from_hex(j["color_palette"]["green"].get<std::string>().c_str());
        EditorTheme::color_palette_blue = color_from_hex(j["color_palette"]["blue"].get<std::string>().c_str());
        EditorTheme::color_palette_purple = color_from_hex(j["color_palette"]["purple"].get<std::string>().c_str());
        EditorTheme::color_palette_white = color_from_hex(j["color_palette"]["white"].get<std::string>().c_str());

        ImGuiStyle *style = &ImGui::GetStyle();
        style->WindowRounding = EditorTheme::rounding;
        style->FrameRounding = EditorTheme::rounding;
        style->ScrollbarRounding = EditorTheme::rounding;
        style->GrabRounding = EditorTheme::rounding;
        style->TabRounding = EditorTheme::rounding;
        style->ChildRounding = EditorTheme::rounding;
        style->PopupRounding = EditorTheme::rounding;
        theming(EditorTheme::editor_text/255, EditorTheme::editor_title_bar/255, EditorTheme::editor_body/255, EditorTheme::editor_popups/255,
                           EditorTheme::editor_hover/255, EditorTheme::editor_inputs/255, EditorTheme::editor_elements/255);
    }
}