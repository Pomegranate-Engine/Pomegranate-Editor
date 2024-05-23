#include "notifications.h"

std::vector<Notification> Notify::notifications = std::vector<Notification>();
uint32_t Notification::NotificationCount = 0;

Notification::Notification(Texture *icon, Vec3 color, const std::string& title, const std::string& content, float lifetime)
{
    this->icon = icon;
    this->title = title;
    this->content = content;
    this->lifetime = lifetime;
    this->time_alive = 0;
    this->color = color;
    this->id = NotificationCount++;
}

float linear_interpolate(float a, float b, float t)
{
    return (b-a)*t+a;
}

void Notify::render(float dt)
{
    for (int i = notifications.size()-1; i>0 ; --i)
    {
        float alpha = notifications[i].time_alive/(notifications[i].time_alive+0.1);
        if(alpha < 0)
        {
            alpha = 0;
        }
        float w;
        float h;
        SDL_GetCurrentRenderOutputSize(Window::current->get_sdl_renderer(),&(int&)w,&(int&)h);

        int window_w;
        int window_h;
        SDL_GetWindowSize(Window::current->get_sdl_window(),&window_w,&window_h);
        //Get render scale
        float scale_x = (float)w/(float)window_w;
        float scale_y = (float)h/(float)window_h;

        ImGui::PushStyleColor(ImGuiCol_WindowBg,ImVec4(notifications[i].color.x/255.0f,notifications[i].color.y/255.0f,notifications[i].color.z/255.0f, alpha));
        if(ImGui::Begin((notifications[i].title + "##" + std::to_string(notifications[i].id)).c_str(), nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_AlwaysFront))
        {
            ImGui::SetWindowSize(ImVec2(0,64),ImGuiCond_Always);
            if(notifications[i].icon != nullptr)
            {
                ImGui::Image((void*)notifications[i].icon->get_sdl_texture(), ImVec2(18,18), ImVec2(0,0), ImVec2(1,1));
                ImGui::SameLine();
            }
            ImGui::Text("%s", notifications[i].title.c_str());
            ImGui::Separator();
            ImGui::Text("%s", notifications[i].content.c_str());
            if(notifications[i].time_alive == 0) {
                ImGui::SetWindowPos(
                        ImVec2(w / scale_x - ImGui::GetWindowSize().x - 8.0f, h / scale_y - ((notifications.size() - i) * 72.0f) - 8.0f),
                        ImGuiCond_Always);
                Vec2 a = Vec2(0,ImGui::GetWindowPos().y);
                Vec2 b = Vec2(0, h / scale_y - ((notifications.size() - i) * 72.0f) - 8.0f);

                ImGui::SetWindowPos(
                        ImVec2(linear_interpolate(w / scale_x - 8, w / scale_x - ImGui::GetWindowSize().x - 8, 0.0), a.lerp(b, 10.0f * dt).y),
                        ImGuiCond_Always);
            }
            else
            {
                Vec2 a = Vec2(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y);
                Vec2 b = Vec2(0, h / scale_y - ((notifications.size() - i) * 72) - 8.0f);

                float t = notifications[i].time_alive;
                if(notifications[i].time_alive < notifications[i].lifetime-1)
                {
                    if (t > 1) {
                        t = 1;
                    }
                    ImGui::SetWindowPos(
                            ImVec2(linear_interpolate(a.x, w / scale_x - ImGui::GetWindowSize().x - 8, t), a.lerp(b, 10 * dt).y),
                            ImGuiCond_Always);
                }
                else
                {
                    t = (t-(notifications[i].lifetime-1));
                    ImGui::SetWindowPos(
                            ImVec2(linear_interpolate(a.x, w / scale_x - 8, t), a.lerp(b, 10 * dt).y),
                            ImGuiCond_Always);
                }
            }
            ImGui::End();
        }
        ImGui::PopStyleColor();
        notifications[i].time_alive += dt;
        if(notifications[i].time_alive > notifications[i].lifetime)
        {
            notifications.erase(notifications.begin() + i,notifications.begin()+i+1);
        }
    }
}

void Notify::notify(const Notification& notification)
{
    notifications.push_back(notification);
}
