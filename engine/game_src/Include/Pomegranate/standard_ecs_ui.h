#ifndef POMEGRANATE_ENGINE_STANDARD_ECS_UI_H
#define POMEGRANATE_ENGINE_STANDARD_ECS_UI_H
#include <string>
#include <functional>
#include "ecs.h"
#include "color.h"
#include "vec_math.h"
#include "window.h"
#include<backends/imgui_impl_sdl3.h>
#include<backends/imgui_impl_sdlrenderer3.h>
#include<misc/cpp/imgui_stdlib.h>
namespace Pomegranate
{
    class UITransform : public Component
    {
    public:
        Vec2 position;
        Vec2 size;
        int id;
        UITransform();
        static int UI_ID;
    };

    class UIText : public Component
    {
    public:
        std::string text;
        Color color;
        UIText();
        void init(Entity* entity) override;
    };

    class UIButton : public Component
    {
    public:
        std::string text;
        Color text_color;
        Color background_color;
        std::function<void(Entity*)> callback;
        UIButton();
        void init(Entity* entity) override;
    };

    class UITextField : public Component
    {
    public:
        std::string text;
        std::string placeholder_text;
        Color text_color;
        Color background_color;
        bool multiline;
        UITextField();
        void init(Entity* entity) override;
    };

    class UIDropdown : public Component
    {
    public:
        std::string text;
        std::vector<std::string> options;
        int selected_option;
        Color text_color;
        Color background_color;
        UIDropdown();
        void init(Entity* entity) override;
    };

    class UICheckbox : public Component
    {
    public:
        std::string text;
        bool checked;
        Color text_color;
        Color background_color;
        UICheckbox();
        void init(Entity* entity) override;
    };

    class UISlider : public Component
    {
    public:
        float value;
        float min;
        float max;
        Color background_color;
        Color slider_color;
        UISlider();
        void init(Entity* entity) override;
    };



    class UIController : public System
    {
    public:
        UIController();
        void tick(Entity* entity) override;
        void pre_draw() override;
        void draw(Entity* entity) override;
        void post_draw() override;
    };
}

#endif //POMEGRANATE_ENGINE_STANDARD_ECS_UI_H
