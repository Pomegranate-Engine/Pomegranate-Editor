#ifndef POMEGRANATE_ENGINE_WINDOW_H
#define POMEGRANATE_ENGINE_WINDOW_H
#include <SDL.h>
#include "engine.h"
//OpenGL
#ifdef WIN32
#include <glew/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#endif

namespace Pomegranate
{
    class Window
    {
    private:
        /* data */
        int width, height;
        const char* title;
        SDL_Window* window;
        SDL_Renderer* renderer;
    public:

        Window(const char* title,int width, int height);
        ~Window();
        [[nodiscard]] int get_width() const;
        [[nodiscard]] int get_height() const;
        [[nodiscard]] const char* get_title() const;
        void set_title(const char* new_title);
        void set_size(int width, int height);
        void set_icon(const char* path);
        [[nodiscard]] SDL_Window* get_sdl_window() const;
        [[nodiscard]] SDL_Renderer* get_sdl_renderer() const;
        [[nodiscard]] SDL_GLContext get_gl_context() const;
        int open(bool maximized = true);
        void close();
        void make_current();
        static const Window* current;
    };
}

#endif //POMEGRANATE_ENGINE_WINDOW_H
