#include "standard_ecs_rendering.h"
namespace Pomegranate
{
    EntityRef Camera::current = nullptr;
    Vec2 Camera::current_render_position = Vec2(0, 0);
    float Camera::current_render_zoom = 1.0;

    Sprite::Sprite()
    {
        print_info("Loading sprite");
        this->texture = ResourceManager::load<Texture>("res/none.png");
        this->color = Color(255, 255, 255, 255);
        this->pivot = Vec2(0.5, 0.5);
        this->offset = Vec2(0, 0);
        this->flip_horizontal = false;
        this->flip_vertical = false;
        register_component(Sprite);
        //TODO: Add texture support to lua
        push_data<Vec2>("pivot", &this->pivot);
        push_data<Vec2>("offset", &this->offset);
        push_data<bool>("flip_horizontal", &this->flip_horizontal);
        push_data<bool>("flip_vertical", &this->flip_vertical);
        push_data<Color>("color", &this->color);
        push_data<Texture*>("texture", &this->texture);
    }

    void Sprite::init(Entity *entity)
    {
        entity->require_component<Transform>();
    }

    void Sprite::load_texture(const char *path) {
        this->texture = ResourceManager::load<Texture>(path);
    }

    AnimatedSprite::AnimatedSprite()
    {
        this->texture = ResourceManager::load<Texture>("res/none.png");
        this->color = Color(255, 255, 255, 255);
        this->pivot = Vec2(0.5, 0.5);
        this->offset = Vec2(0, 0);
        this->flip_horizontal = false;
        this->flip_vertical = false;
        this->horizontal_frames = 1;
        this->vertical_frames = 1;
        this->frame_x = 0;
        this->frame_y = 0;
        register_component(Sprite);
        //TODO: Add texture support to lua
        push_data<Vec2>("pivot", &this->pivot);
        push_data<Vec2>("offset", &this->offset);
        push_data<bool>("flip_horizontal", &this->flip_horizontal);
        push_data<bool>("flip_vertical", &this->flip_vertical);
        push_data<Color>("color", &this->color);
        push_data<Texture*>("texture", &this->texture);
        push_data<int>("horizontal_frames", &this->horizontal_frames);
        push_data<int>("vertical_frames", &this->vertical_frames);
        push_data<int>("frame_x", &this->frame_x);
        push_data<int>("frame_y", &this->frame_y);
    }

    void AnimatedSprite::load_texture(const char *path)
    {
        this->texture = ResourceManager::load<Texture>(path);
    }

    Camera::Camera()
    {
        register_component(Camera);
    }

    void Camera::make_current(EntityRef entity)
    {
        if(!entity->has_component<Camera>())
        {
            print_error("Entity does not have a camera component");
            return;
        }
        current = entity;
    }

    void Camera::init(Pomegranate::Entity *e) {
        push_data<float>("zoom", &this->zoom);
        make_current(e);
    }

    DebugCircle::DebugCircle()
    {
        this->radius = 16.0;
        this->color = Color(255, 255, 255, 255);
        register_component(DebugCircle);
        push_data<float>("radius", &this->radius);
        push_data<Color>("color", &this->color);
    }

    void Render::draw(Entity* entity)
    {
        if(Camera::current != nullptr)
        {
            Camera::current_render_position = Camera::current->transform->pos;
            Camera::current_render_zoom = Camera::current->get_component<Camera>()->zoom;
        }
        if(entity->has_component<Transform>())
        {
            if (entity->has_component<Sprite>()) {
                Render::sprite(entity);
            }
            if(entity->has_component<AnimatedSprite>())
            {
                Render::animated_sprite(entity);
            }
            /*if(entity->has_component<DebugCircle>())
            {
            Render::debug_circle(entity);
            }
            if(entity->has_component<Tilemap>())
            {
            Render::tilemap(entity);
            }*/
        }

    }

    void Render::sprite(Entity*e) {
        auto t = e->transform;
        auto s = e->get_component<Sprite>();
        SDL_FRect r;
        int w = 0;
        int h = 0;
        SDL_QueryTexture(s->texture.get<Texture>()->get_sdl_texture(), nullptr, nullptr, &w, &h);
        int screen_w = 0;
        int screen_h = 0;
        float render_scale_x = 1.0;
        float render_scale_y = 1.0;
        SDL_GetRenderScale(Window::current->get_sdl_renderer(), &render_scale_x, &render_scale_y);
        SDL_GetCurrentRenderOutputSize(Window::current->get_sdl_renderer(), &screen_w, &screen_h);
        screen_w /= render_scale_x;
        screen_h /= render_scale_y;
        r.w = (float)w*t->scale.x*Camera::current_render_zoom;
        r.h = (float)h*t->scale.y*Camera::current_render_zoom;
        r.x = ((t->pos.x+s->offset.x-Camera::current_render_position.x)*Camera::current_render_zoom)+screen_w/2-r.w*s->pivot.x;
        r.y = ((t->pos.y+s->offset.y-Camera::current_render_position.y)*Camera::current_render_zoom)+screen_h/2-r.h*s->pivot.y;
        auto* center = new SDL_FPoint();
        center->x = r.w*s->pivot.x;
        center->y = r.h*s->pivot.y;
        SDL_SetTextureColorMod(s->texture.get<Texture>()->get_sdl_texture(), s->color.r, s->color.g, s->color.b);
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if(s->flip_horizontal)
        {
            flip = (SDL_RendererFlip)(flip|SDL_FLIP_HORIZONTAL);
        }
        if(s->flip_vertical)
        {
            flip = (SDL_RendererFlip)(flip|SDL_FLIP_VERTICAL);
        }

        SDL_RenderTextureRotated(Window::current->get_sdl_renderer(), s->texture.get<Texture>()->get_sdl_texture(), nullptr, &r, t->rot*180.0/3.14159, center, flip);
    }

    void Render::animated_sprite(Entity*e) {
        auto t = e->transform;
        auto s = e->get_component<AnimatedSprite>();
        SDL_FRect r;
        int w = 0;
        int h = 0;
        SDL_QueryTexture(s->texture.get<Texture>()->get_sdl_texture(), nullptr, nullptr, &w, &h);
        int frame_width = w/s->horizontal_frames;
        int frame_height = h/s->vertical_frames;
        int screen_w = 0;
        int screen_h = 0;
        float render_scale_x = 1.0;
        float render_scale_y = 1.0;
        SDL_GetRenderScale(Window::current->get_sdl_renderer(), &render_scale_x, &render_scale_y);
        SDL_GetCurrentRenderOutputSize(Window::current->get_sdl_renderer(), &screen_w, &screen_h);
        screen_w /= render_scale_x;
        screen_h /= render_scale_y;
        r.w = (float)frame_width*t->scale.x*Camera::current_render_zoom;
        r.h = (float)frame_height*t->scale.y*Camera::current_render_zoom;
        r.x = ((t->pos.x+s->offset.x-Camera::current_render_position.x)*Camera::current_render_zoom)+screen_w/2-r.w*s->pivot.x;
        r.y = ((t->pos.y+s->offset.y-Camera::current_render_position.y)*Camera::current_render_zoom)+screen_h/2-r.h*s->pivot.y;
        auto* center = new SDL_FPoint();
        center->x = r.w*s->pivot.x;
        center->y = r.h*s->pivot.y;
        SDL_SetTextureColorMod(s->texture.get<Texture>()->get_sdl_texture(), s->color.r, s->color.g, s->color.b);
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if(s->flip_horizontal)
        {
            flip = (SDL_RendererFlip)(flip|SDL_FLIP_HORIZONTAL);
        }
        if(s->flip_vertical)
        {
            flip = (SDL_RendererFlip)(flip|SDL_FLIP_VERTICAL);
        }
        SDL_FRect src = {(float)(frame_width*s->frame_x),(float)(frame_height*s->frame_y),(float)frame_width,(float)frame_height};

        SDL_RenderTextureRotated(Window::current->get_sdl_renderer(), s->texture.get<Texture>()->get_sdl_texture(), &src, &r, t->rot*180.0/3.14159, center, flip);

    }

    void Render::debug_circle(Entity* entity)
    {
        auto* t = entity->transform;
        auto* dc = entity->get_component<DebugCircle>();

        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), dc->color.r,dc->color.g,dc->color.b, dc->color.a);
        // Calculate angle step for each segment
        float angleStep = 2.0f * 3.14159f / 32.0f;

        // Draw the circle outline
        for (int i = 0; i < 32; ++i) {
            float angle1 = (float)i * angleStep;
            float angle2 = ((float)i + 1.0f) * angleStep;

            // Calculate the coordinates of the two points on the circle
            float x1 = t->pos.x-Camera::current->transform->pos.x + (dc->radius * cosf(angle1));
            float y1 = t->pos.y-Camera::current->transform->pos.y + (dc->radius * sinf(angle1));
            float x2 = t->pos.x-Camera::current->transform->pos.x + (dc->radius * cosf(angle2));
            float y2 = t->pos.y-Camera::current->transform->pos.y + (dc->radius * sinf(angle2));

            // Draw a line between the two points to create the circle outline
            SDL_RenderLine(Window::current->get_sdl_renderer(), x1, y1, x2, y2);
        }
    }

    void Render::tilemap(Entity* entity)
    {
        auto* t = entity->transform;
        auto* map = entity->get_component<Tilemap>();

        auto* camera_transform = Camera::current->transform;

        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), map->color.r,map->color.g,map->color.b, map->color.a);
        // Calculate angle step for each segment
        for (int z = 0; z < map->get_layer_count(); ++z)
        {
            int i = 0;
            for (int y = 0; y < map->height; ++y) {
                for (int x = 0; x < map->width; ++x) {
                    if (map->get_tile(i, z).x != -1) {
                        Vec2i tile = map->get_tile(i, z);
                        SDL_FRect src = {(float) tile.x * (float) map->tileset_tile_width,
                                         (float) tile.y * (float) map->tileset_tile_height, (float) map->tileset_tile_width,
                                         (float) map->tileset_tile_height};
                        SDL_FRect dst = {(float) (x * map->tileset_tile_width), (float) (y * map->tileset_tile_height),
                                         (float) map->tileset_tile_width, (float) map->tileset_tile_height};
                        dst.x *= t->scale.x;
                        dst.y *= t->scale.y;
                        dst.w *= t->scale.x;
                        dst.h *= t->scale.y;
                        dst.x -= camera_transform->pos.x;
                        dst.y -= camera_transform->pos.y;
                        dst.x += t->pos.x;
                        dst.y += t->pos.y;

                        SDL_RenderTexture(Window::current->get_sdl_renderer(), map->tileset_texture.get<Texture>()->get_sdl_texture(), &src, &dst);
                    }
                    i++;
                }
                i++;
            }
        }
    }

    Render::Render()
    {
        register_system(Render);
    }

    Tilemap::Tilemap()
    {
        this->width = 256;
        this->height = 256;
        this->tileset_horizontal_tiles=0;
        this->tileset_vertical_tiles=0;
        this->tileset_tile_width=0;
        this->tileset_tile_height=0;
        this->color = Color(255, 255, 255, 255);
        this->tiles = std::vector<Vec2i*>();
        tiles.push_back(new Vec2i[width * height]);
        for(int i = 0; i < width*height; i++)
        {
            tiles[0][i] = Vec2i(-1, -1);
        }
        this->tileset_texture = nullptr;
        register_component(Tilemap);
        //TODO: Add texture support to lua
        //TODO: add vector support to lua
        push_data<int>("width", &this->width);
        push_data<int>("height", &this->height);
        push_data<Color>("color", &this->color);
        push_data<int>("tileset_horizontal_tiles", &this->tileset_horizontal_tiles);
        push_data<int>("tileset_vertical_tiles", &this->tileset_vertical_tiles);
        push_data<int>("tileset_tile_width", &this->tileset_tile_width);
        push_data<int>("tileset_tile_height", &this->tileset_tile_height);
    }

    void Tilemap::load_texture(const char *path)
    {
        this->tileset_texture = ResourceManager::load<Texture>(path);
    }

    void Tilemap::set_tile(Vec2i pos, Vec2i tile, int layer)
    {
        int p = pos.x+pos.y*(width+1);
        if(p<width*height)
        {
            tiles[layer][p] = tile;
        }
        else
        {
            print_error("Tilemap index out of bounds");
        }
    }

    Vec2i Tilemap::get_tile(Vec2i pos, int layer)
    {
        int p = pos.x+pos.y*(width+1);
        if(p<width*height)
        {
            return tiles[layer][p];
        }
        else
        {
            return {-1,-1};
        }
    }

    void Tilemap::set_tileset_tile_size(int w, int h)
    {
        this->tileset_tile_width = w;
        this->tileset_tile_height = h;
        int wid = 0;
        int hei = 0;
        SDL_QueryTexture(this->tileset_texture.get<Texture>()->get_sdl_texture(), nullptr, nullptr, &wid, &hei);
        this->tileset_horizontal_tiles = 17;
        this->tileset_vertical_tiles = 8;
    }

    Vec2i Tilemap::get_tile(int index, int layer)
    {
        return (tiles[layer])[index];
    }

    void Tilemap::fill_tile(Vec2i a, Vec2i b, Vec2i tile, int layer)
    {
        for (int y = a.y; y <= b.y; ++y)
        {
            for (int x = a.x; x <= b.x; ++x)
            {
                set_tile(Vec2i(x, y), tile, layer);
            }
        }
    }

    void Tilemap::place_multitile(Vec2i pos, Vec2i tile_a, Vec2i tile_b, int layer)
    {
        for (int y = tile_a.y; y <= tile_b.y; ++y)
        {
            for (int x = tile_a.x; x <= tile_b.x; ++x)
            {
                set_tile(Vec2i(x - tile_a.x + pos.x, y - tile_a.y + pos.y), Vec2i(x, y), layer);
            }
        }
    }

    void Tilemap::add_layer()
    {
        tiles.push_back(new Vec2i[width * height]);
        for(int i = 0; i < width*height; i++)
        {
            tiles[tiles.size()-1][i] = Vec2i(-1, -1);
        }
    }

    int Tilemap::get_layer_count()
    {
        return (int)this->tiles.size();
    }
}
