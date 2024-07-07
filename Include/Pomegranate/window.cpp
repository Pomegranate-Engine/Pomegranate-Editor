#include "window.h"
namespace Pomegranate
{
    const Window* Window::current = nullptr;

    Window::Window(const char* title, int width, int height)
    {
        this->title = title;
        this->width = width;
        this->height = height;

        this->window = nullptr;
        this->renderer = nullptr;
    }

    Window::~Window()
    {
        this->close();
    }

    int Window::get_width() const
    {
        return this->width;
    }

    int Window::get_height() const
    {
        return this->height;
    }

    SDL_Window* Window::get_sdl_window() const
    {
        return this->window;
    }
    SDL_Renderer* Window::get_sdl_renderer() const
    {
        return this->renderer;
    }

    int Window::open(bool maximized)
    {
        make_current();
        bool HIGH_DPI = false;
#if defined(__APPLE__) //TODO: Implement better solution for HIGH_DPI
        HIGH_DPI = false;
#endif
        int flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
        if(maximized)
        {
            flags |= SDL_WINDOW_MAXIMIZED;
        }
        if(HIGH_DPI)
        {
            flags = flags | SDL_WINDOW_HIGH_PIXEL_DENSITY;
        }
        this->window = SDL_CreateWindow(this->title, this->width, this->height, flags); //TODO: Add HIGH_DPI support
        if (!this->window)
        {
            std::cout << "Failed to create window\n" << std::endl;
            return -1;
        }

        this->renderer = SDL_CreateRenderer(this->window, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if(HIGH_DPI)
        {
            SDL_SetRenderScale(renderer, 2, 2);
        }
        if (!this->renderer)
        {
            SDL_DestroyWindow(this->window);
            SDL_Quit();
            print_error("Failed to create renderer");
            return -1;
        }
        return 0;
    }

    void Window::close()
    {
        SDL_DestroyWindow(this->window);
        SDL_DestroyRenderer(this->renderer);
    }

    const char *Window::get_title() const
    {
        return this->title;
    }
    void Window::make_current()
    {
        Window::current = this;
    }

    void Window::set_title(const char *new_title)
    {
        this->title = new_title;
        SDL_SetWindowTitle(this->window, new_title);
    }

    void Window::set_size(int new_width, int new_height)
    {
        this->width = new_width;
        this->height = new_height;
        SDL_SetWindowSize(this->window, width, height);
    }

    void Window::set_icon(const char *path)
    {
        SDL_Surface* icon = IMG_Load(path);
        SDL_SetWindowIcon(this->window, icon);
        SDL_DestroySurface(icon);
    }

    SDL_GLContext Window::get_gl_context() const {
           return SDL_GL_GetCurrentContext();
    }
}
