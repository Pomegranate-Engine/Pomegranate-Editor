#ifndef POMEGRANATE_ENGINE_STANDARD_ECS_RENDERING_H
#define POMEGRANATE_ENGINE_STANDARD_ECS_RENDERING_H

#include "ecs.h"
#include "vec_math.h"
#include "color.h"
#include "window.h"
#include "standard_ecs.h"
#include "resource_manager.h"
#include "texture.h"

namespace Pomegranate
{
    static bool render_default_sort(EntityRef a, EntityRef b)
    {
        if(a->has_component<Transform>() && b->has_component<Transform>()) {
            Transform *a_transform = a->get_component<Transform>();
            Transform *b_transform = b->get_component<Transform>();
            return a_transform->z_index < b_transform->z_index;
        }
        return false;
    }
    class DebugCircle : public Component
    {
    public:
        float radius;
        Color color;
        DebugCircle();
    };

    class Sprite : public Component
    {
    public:
        Vec2 pivot;
        Vec2 offset;
        bool flip_horizontal;
        bool flip_vertical;
        Texture* texture;
        Color color;
        Sprite();
        void init(Pomegranate::Entity *) override;
        void load_texture(const char* path);
    };

    class AnimatedSprite : public Component
    {
    public:
        Texture* texture;
        Color color;
        Vec2 pivot;
        Vec2 offset;
        bool flip_horizontal;
        bool flip_vertical;
        int frame_x;
        int frame_y;
        int horizontal_frames;
        int vertical_frames;
        AnimatedSprite();
        void load_texture(const char* path);
    };

    class Render: public System
    {
    public:
        Render();
        void draw(Entity* entity) override;
        static void sprite(Entity*);
        static void debug_circle(Entity*);
        static void animated_sprite(Entity *e);

        static void tilemap(Entity *entity);
    };

    class Camera: public Component
    {
    public:
        float zoom = 1.0;
        Camera();
        void init(Pomegranate::Entity *) override;
        static void make_current(EntityRef entity);
        static EntityRef current;
        static Vec2 current_render_position;
        static float current_render_zoom;
    };

    class Tilemap : public Component
    {
    private:
        std::vector<Vec2i*> tiles;
    public:
        Texture* tileset_texture;
        int tileset_horizontal_tiles;
        int tileset_vertical_tiles;
        int tileset_tile_width;
        int tileset_tile_height;
        Color color;
        int width;
        int height;
        Tilemap();
        void load_texture(const char* path);
        void add_layer();
        int get_layer_count();
        void set_tileset_tile_size(int width, int height);
        void set_tile(Vec2i pos, Vec2i tile, int layer=0);
        void fill_tile(Vec2i a, Vec2i b, Vec2i tile, int layer=0);
        void place_multitile(Vec2i pos, Vec2i tile_a, Vec2i tile_b, int layer=0);
        Vec2i get_tile(Vec2i pos, int layer=0);
        Vec2i get_tile(int index, int layer=0);
    };
}


#endif //POMEGRANATE_ENGINE_STANDARD_ECS_RENDERING_H
