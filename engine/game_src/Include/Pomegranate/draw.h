#ifndef RPG_ENGINE_DRAW_H
#define RPG_ENGINE_DRAW_H
#include "color.h"
#include "vec_math.h"
#include "texture.h"
namespace Pomegranate
{
    class Draw
            {
    public:
        static void rect(Vec2 pos, Vec2 size, Color color);
        static void filled_rect(Vec2 pos, Vec2 size, Color color);
        static void circle(Vec2 pos, float radius, Color color);
        static void filled_circle(Vec2 pos, float radius, Color color);
        static void line(Vec2 start, Vec2 end, Color color);
        static void pixel(Vec2 pos, Color color);
        static void texture(Texture* texture, Vec2 pos, Vec2 size, Color color);
        static void texture(Texture* texture, Vec2 pos, Vec2 size, float angle, Color color);
        static void texture(Texture* texture, Vec2 pos, Vec2 size, Vec2 origin, float angle, Color color);
        static void texture(Texture* texture, Vec2 pos, Vec2 size, Vec2 origin, float angle, SDL_RendererFlip flip, Color color);
        static void texture(Texture* texture, Vec2 pos, Vec2 size, SDL_FRect* src, Vec2 origin, float angle, SDL_RendererFlip flip, Color color);
    };
}

#endif //RPG_ENGINE_DRAW_H
