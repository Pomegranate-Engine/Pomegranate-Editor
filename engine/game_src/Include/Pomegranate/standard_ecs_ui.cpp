#include "standard_ecs_ui.h"

namespace Pomegranate
{
    int UITransform::UI_ID = 0;

    void UIController::tick(Entity *entity)
    {

    }
    void UIController::pre_draw()
    {
        //Begin full screen window
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("FULL SCREEN", nullptr,
                     ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoBackground);

        //Remove window margin
        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::SetWindowSize(ImVec2((float)Window::current->get_width(), (float)Window::current->get_height()));
    }
    void UIController::draw(Entity *entity)
    {
        if(entity->has_component<UITransform>())
        {
            auto transform = entity->get_component<UITransform>();
            ImGui::SetCursorPos(ImVec2(transform->position.x, transform->position.y));
            if (entity->has_component<UIText>())
            {
                auto text = entity->get_component<UIText>();
                ImGui::TextColored(ImVec4((float) text->color.r / 255.0f, (float) text->color.g / 255.0f,
                                          (float) text->color.b / 255.0f, (float) text->color.a / 255.0f),
                                   "%s", text->text.c_str());
            }
            if (entity->has_component<UIButton>())
            {
                auto button = entity->get_component<UIButton>();
                if (ImGui::Button((button->text+"##Button"+std::to_string(transform->id)).c_str(), ImVec2(transform->size.x, transform->size.y)))
                {
                    if(button->callback != nullptr)
                        button->callback(entity);
                }
            }
            if (entity->has_component<UITextField>())
            {
                auto text_field = entity->get_component<UITextField>();

                if(text_field->multiline)
                {
                    ImGui::SetNextItemWidth(transform->size.x);
                    ImGui::InputTextMultiline((std::string(text_field->placeholder_text)+"##Text Field"+std::to_string(transform->id)).c_str(), &text_field->text, ImVec2(transform->size.x, transform->size.y));
                }
                else
                {
                    ImGui::SetNextItemWidth(transform->size.x);
                    ImGui::InputText((std::string(text_field->placeholder_text)+"##Text Field"+std::to_string(transform->id)).c_str(), &text_field->text, ImGuiInputTextFlags_EnterReturnsTrue);
                }
            }
            if (entity->has_component<UIDropdown>())
            {
                auto dropdown = entity->get_component<UIDropdown>();
                //Set size
                ImGui::SetNextItemWidth(transform->size.x);
                if(ImGui::BeginCombo((std::string("##Combo")+std::to_string(transform->id)).c_str(), dropdown->options[dropdown->selected_option].c_str()))
                {
                    for (int n = 0; n < dropdown->options.size(); n++)
                    {
                        bool is_selected = (dropdown->selected_option == n);
                        if (ImGui::Selectable(dropdown->options[n].c_str(), is_selected))
                        {
                            dropdown->selected_option = n;
                        }
                        if (is_selected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }
            }
            if (entity->has_component<UICheckbox>())
            {
                auto checkbox = entity->get_component<UICheckbox>();
                //Set size
                ImGui::SetNextItemWidth(transform->size.x);
                ImGui::Checkbox((std::string(checkbox->text)+"##Checkbox"+std::to_string(transform->id)).c_str(), &checkbox->checked);
            }
            if (entity->has_component<UISlider>())
            {
                auto slider = entity->get_component<UISlider>();
                //Set size
                ImGui::SetNextItemWidth(transform->size.x);
                ImGui::SliderFloat((std::string("##Slider")+std::to_string(transform->id)).c_str(), &slider->value, slider->min, slider->max);
            }
        }
    }

    void UIController::post_draw()
    {
        ImGui::End();
        ImGui::PopStyleVar();
    }

    UIController::UIController()
    {
        register_system(UIController);
    }

    UIText::UIText()
    {
        this->text = "";
        this->color = Color(255, 255, 255, 255);
        register_component(UIText);
        push_data<std::string>("text", &this->text);
        push_data<Color>("color", &this->color);
    }

    void UIText::init(Entity *entity)
    {
        entity->require_component<UITransform>();
    }

    UIButton::UIButton()
    {
        this->text = "";
        this->text_color = Color(255, 255, 255, 255);
        this->background_color = Color(0, 0, 0, 255);
        this->callback = nullptr;
        register_component(UIButton);
        push_data<std::string>("text", &this->text);
        push_data<Color>("text_color", &this->text_color);
        push_data<Color>("background_color", &this->background_color);
    }

    void UIButton::init(Entity *entity)
    {
        entity->require_component<UITransform>();
    }

    UITransform::UITransform()
    {
        this->position = Vec2(0, 0);
        this->size = Vec2(64, 64);
        this->id = UITransform::UI_ID++;
        register_component(UITransform);
        push_data<Vec2>("position", &this->position);
        push_data<Vec2>("size", &this->size);
        push_data<int>("id", &this->id);
    }

    UITextField::UITextField()
    {
        this->text = "";
        this->placeholder_text = "Enter text here";
        this->text_color = Color(255, 255, 255, 255);
        this->background_color = Color(0, 0, 0, 255);
        this->multiline = false;
        register_component(UITextField);
        push_data<std::string>("text", &this->text);
        push_data<std::string>("placeholder_text", &this->placeholder_text);
        push_data<Color>("text_color", &this->text_color);
        push_data<Color>("background_color", &this->background_color);
        push_data<bool>("multiline", &this->multiline);
    }

    void UITextField::init(Entity *entity)
    {
        entity->require_component<UITransform>();
    }

    UIDropdown::UIDropdown()
    {
        this->text = "";
        this->options = std::vector<std::string>();
        this->selected_option = 0;
        this->text_color = Color(255, 255, 255, 255);
        this->background_color = Color(0, 0, 0, 255);
        register_component(UIDropdown);
        push_data<std::string>("text", &this->text);
        push_data<int>("selected_option", &this->selected_option);
        //TODO: Push data for options
        push_data<Color>("text_color", &this->text_color);
        push_data<Color>("background_color", &this->background_color);
    }

    void UIDropdown::init(Entity *entity)
    {
        entity->require_component<UITransform>();
    }

    UICheckbox::UICheckbox()
    {
        this->text = "";
        this->checked = false;
        this->text_color = Color(255, 255, 255, 255);
        this->background_color = Color(0, 0, 0, 255);
        register_component(UICheckbox);
        push_data<std::string>("text", &this->text);
        push_data<bool>("checked", &this->checked);
        push_data<Color>("text_color", &this->text_color);
        push_data<Color>("background_color", &this->background_color);
    }

    void UICheckbox::init(Entity *entity)
    {
        entity->require_component<UITransform>();
    }

    UISlider::UISlider()
    {
        register_component(UISlider);
        this->value = 0.0f;
        this->min = 0.0f;
        this->max = 1.0f;
        push_data<float>("value", &this->value);
        push_data<float>("min", &this->min);
        push_data<float>("max", &this->max);
        push_data<Color>("slider_color", &this->slider_color);
        push_data<Color>("background_color", &this->background_color);
    }

    void UISlider::init(Entity *entity)
    {
        entity->require_component<UITransform>();
    }
}
