#ifndef POMEGRANATE_ENGINE_INPUT_MANAGER_H
#define POMEGRANATE_ENGINE_INPUT_MANAGER_H
#include "SDL.h"
#include "vec_math.h"

namespace Pomegranate
{
    class InputManager
    {
    public:
        /* data */
        static bool* keys;
        static bool* mouse_buttons;
        static Vec2 mouse_position;
        static Vec2 mouse_delta;
        static bool mouse_moved;
        static bool mouse_scrolled;
        static Vec2 mouse_scroll;
        static int press;
    public:
        InputManager();
        ~InputManager();
        static void init();
        static void process_event(SDL_Event event);
        static bool get_key(int);
        static bool get_mouse_button(int);
        static Vec2 get_mouse_position();
        static Vec2 get_mouse_delta();
        static bool get_mouse_moved();
        static bool get_mouse_scrolled();
        static Vec2 get_mouse_scroll();
        static int get_text();
    };
}

#endif //POMEGRANATE_ENGINE_INPUT_MANAGER_H
