#include "draw.h"

namespace Pomegranate
{

    void Draw::rect(Vec2 pos, Vec2 size, Color color)
    {
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), color.r, color.g, color.b, color.a);
        SDL_RenderRect(Window::current->get_sdl_renderer(), new SDL_FRect({pos.x, pos.y, size.x, size.y}));
    }
    void Draw::filled_rect(Vec2 pos, Vec2 size, Color color)
    {
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(Window::current->get_sdl_renderer(), new SDL_FRect({pos.x, pos.y, size.x, size.y}));
    }
    void Draw::circle(Vec2 pos, float radius, Color color)
    {
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), color.r, color.g, color.b, color.a);

        //TODO: Implement circle drawing
    }
    void Draw::filled_circle(Vec2 pos, float radius, Color color)
    {
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), color.r, color.g, color.b, color.a);

        //TODO: Implement filled circle drawing
    }

    void Draw::line(Vec2 start, Vec2 end, Color color)
    {
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), color.r, color.g, color.b, color.a);
        SDL_RenderLine(Window::current->get_sdl_renderer(), start.x, start.y, end.x, end.y);
    }

    void Draw::pixel(Vec2 pos, Color color)
    {
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), color.r, color.g, color.b, color.a);
        SDL_RenderPoint(Window::current->get_sdl_renderer(), pos.x, pos.y);
    }

    void Draw::texture(Texture *texture, Vec2 pos, Vec2 size, Color color)
    {
        SDL_SetTextureColorMod(texture->get_sdl_texture(), color.r, color.g, color.b);
        SDL_SetTextureAlphaMod(texture->get_sdl_texture(), color.a);
        SDL_RenderTexture(Window::current->get_sdl_renderer(), texture->get_sdl_texture(), nullptr, new SDL_FRect({pos.x, pos.y, size.x, size.y}));
    }

    void Draw::texture(Texture *texture, Vec2 pos, Vec2 size, float angle, Color color)
    {
        SDL_SetTextureColorMod(texture->get_sdl_texture(), color.r, color.g, color.b);
        SDL_SetTextureAlphaMod(texture->get_sdl_texture(), color.a);
        SDL_RenderTextureRotated(Window::current->get_sdl_renderer(), texture->get_sdl_texture(), nullptr, new SDL_FRect({pos.x, pos.y, size.x, size.y}), angle, nullptr, SDL_FLIP_NONE);
    }

    void Draw::texture(Texture *texture, Vec2 pos, Vec2 size, Vec2 origin, float angle, Color color)
    {
        SDL_SetTextureColorMod(texture->get_sdl_texture(), color.r, color.g, color.b);
        SDL_SetTextureAlphaMod(texture->get_sdl_texture(), color.a);
        SDL_RenderTextureRotated(Window::current->get_sdl_renderer(), texture->get_sdl_texture(), nullptr, new SDL_FRect({pos.x, pos.y, size.x, size.y}), angle, new SDL_FPoint({origin.x, origin.y}), SDL_FLIP_NONE);
    }

    void Draw::texture(Texture *texture, Vec2 pos, Vec2 size, Vec2 origin, float angle, SDL_RendererFlip flip, Color color)
    {
        SDL_SetTextureColorMod(texture->get_sdl_texture(), color.r, color.g, color.b);
        SDL_SetTextureAlphaMod(texture->get_sdl_texture(), color.a);
        SDL_RenderTextureRotated(Window::current->get_sdl_renderer(), texture->get_sdl_texture(), nullptr, new SDL_FRect({pos.x, pos.y, size.x, size.y}), angle, new SDL_FPoint({origin.x, origin.y}), flip);
    }

    void Draw::texture(Texture *texture, Vec2 pos, Vec2 size, SDL_FRect *src, Vec2 origin, float angle, SDL_RendererFlip flip,Color color)
    {
        SDL_SetTextureColorMod(texture->get_sdl_texture(), color.r, color.g, color.b);
        SDL_SetTextureAlphaMod(texture->get_sdl_texture(), color.a);
        SDL_RenderTextureRotated(Window::current->get_sdl_renderer(), texture->get_sdl_texture(), src, new SDL_FRect({pos.x, pos.y, size.x, size.y}), angle, new SDL_FPoint({origin.x, origin.y}), flip);
    }
};