#include "input_manager.h"
namespace Pomegranate
{
    InputManager::InputManager() = default;

    InputManager::~InputManager() = default;

    void InputManager::process_event(SDL_Event event)
    {
        InputManager::mouse_delta = Vec2();
        InputManager::mouse_moved = false;
        InputManager::mouse_scrolled = false;
        InputManager::mouse_scroll = Vec2(0,0);

        InputManager::press = -1;
        switch (event.type)
        {
            case SDL_EVENT_KEY_DOWN:
                InputManager::keys[event.key.keysym.scancode] = true;
            //Set press to char form of key pressed
            if(event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
                press = 8;
            else if(event.key.keysym.scancode == SDL_SCANCODE_RETURN)
                press = 12;
            else if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
                press = 2;
            else if(event.key.keysym.scancode == SDL_SCANCODE_LEFT)
                press = 1;
            break;case SDL_EVENT_TEXT_INPUT:InputManager::press = (unsigned char)event.text.text[0];
            break;case SDL_EVENT_KEY_UP:InputManager::keys[event.key.keysym.scancode] = false;
            break;case SDL_EVENT_MOUSE_BUTTON_DOWN:InputManager::mouse_buttons[event.button.button] = true;
            break;case SDL_EVENT_MOUSE_BUTTON_UP:InputManager::mouse_buttons[event.button.button] = false;
            break;case SDL_EVENT_MOUSE_MOTION:InputManager::mouse_position = Vec2(event.motion.x, event.motion.y);
            InputManager::mouse_delta = Vec2(event.motion.xrel, event.motion.yrel);
            InputManager::mouse_moved = true;
            break;case SDL_EVENT_MOUSE_WHEEL:
                InputManager::mouse_scroll = Vec2(event.wheel.x, event.wheel.y);
                InputManager::mouse_scrolled = true;
                printf("mouse scrolled\n");
            break;default:break;
        }
    }

    bool InputManager::get_key(int key_code)
    {
        return InputManager::keys[key_code];
    }

    bool InputManager::get_mouse_button(int mouse_code)
    {
        return InputManager::mouse_buttons[mouse_code];
    }

    Vec2 InputManager::get_mouse_position()
    {
        return InputManager::mouse_position;
    }

    Vec2 InputManager::get_mouse_delta()
    {
        return InputManager::mouse_delta;
    }

    bool InputManager::get_mouse_moved()
    {
        return InputManager::mouse_moved;
    }

    bool InputManager::get_mouse_scrolled()
    {
        return InputManager::mouse_scrolled;
    }

    Vec2 InputManager::get_mouse_scroll()
    {
        return InputManager::mouse_scroll;
    }

    int InputManager::get_text()
    {
        return InputManager::press;
    }

    void InputManager::init()
    {
        InputManager::keys = new bool[SDL_NUM_SCANCODES];
        InputManager::mouse_buttons = new bool[8];
        for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
        {
            InputManager::keys[i] = false;
        }
        for (int i = 0; i < 8; ++i)
        {
            InputManager::mouse_buttons[i] = false;
        }
        InputManager::mouse_position = Vec2();
        InputManager::mouse_delta = Vec2();
        InputManager::mouse_moved = false;
        InputManager::mouse_scrolled = false;
        InputManager::mouse_scroll = Vec2();
    }

    bool* InputManager::keys;
    bool* InputManager::mouse_buttons;
    Vec2 InputManager::mouse_position;
    Vec2 InputManager::mouse_delta;
    bool InputManager::mouse_moved;
    bool InputManager::mouse_scrolled;
    Vec2 InputManager::mouse_scroll;
    int InputManager::press;
}