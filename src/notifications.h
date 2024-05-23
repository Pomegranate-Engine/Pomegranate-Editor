#ifndef POMEGRANATE_ENGINE_NOTIFICATIONS_H
#define POMEGRANATE_ENGINE_NOTIFICATIONS_H
#include <string>
#include <vector>
#include "Pomegranate/Texture.h"
#include "Imgui/imgui.h"
#include "Pomegranate/pomegranate.h"
#include "editor.h"
#include "theme.h"
#include<backends/imgui_impl_sdl3.h>
#include<backends/imgui_impl_sdlrenderer3.h>
#include<Imgui/imgui_internal.h>
using namespace Pomegranate;

struct Notification
{
public:
    static uint32_t NotificationCount;
    Vec3 color;
    uint32_t id;
    ResourceRef icon;
    std::string title;
    std::string content;
    float lifetime;
    float time_alive;
    Notification(ResourceRef icon, Vec3 color, const std::string& title, const std::string& content, float lifetime=5);
};

class Notify
{
public:
    static std::vector<Notification> notifications;
    static void notify(const Notification& notification);
    static void render(float dt);
};

#endif //POMEGRANATE_ENGINE_NOTIFICATIONS_H
