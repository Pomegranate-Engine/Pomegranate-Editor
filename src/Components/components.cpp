class PlayerComponent : public Component
{
public:
    float speed;
    float deceleration;
    Vec2 velocity;
    void init(Pomegranate::Entity * e) override
    {
        speed = 1.0;
        deceleration = 0.1;
        velocity = Vec2();
        e->require_component<Transform>();
        e->require_component<Sprite>();
        register_component(PlayerComponent);
        push_data<float>("speed", &speed);
        push_data<float>("deceleration", &deceleration);
        push_data<Vec2>("velocity", &velocity);
    }
};
class PlayerController : public System
{
    void tick(Pomegranate::Entity *e) override
    {
        if(e->has_component<LuaComponent>())
        {
            auto* player = e->get_component<LuaComponent>()->get_component("PlayerComponent");
            auto* transform = e->transform;
            Vec2 v = player->get<Vec2>("velocity");
            if (InputManager::get_key(SDL_SCANCODE_W)) {
                v.y -= 1*player->get<double>("speed");
            }
            if (InputManager::get_key(SDL_SCANCODE_S)) {
                v.y += 1*player->get<double>("speed");
            }
            if (InputManager::get_key(SDL_SCANCODE_A)) {
                v.x -= 1*player->get<double>("speed");
            }
            if (InputManager::get_key(SDL_SCANCODE_D)) {
                v.x += 1*player->get<double>("speed");
            }
            float deceleration = player->get<double>("deceleration");
            v -= v * deceleration*delta_time;
            transform->pos += v;
            player->set("velocity",v);
        }
    }
};
class CameraFollow : public Component
{
public:
    float speed = 1.0;
    Entity* target = nullptr;
    void init(Entity* e) override
    {
        speed = 1.0;
        target = nullptr;
        e->require_component<Transform>();
        e->require_component<Camera>();
        register_component(CameraFollow);
        push_data<float>("speed", &speed);
        push_data<Entity*>("target", &target);
    }
};
class CameraController : public System
{
public:
    void tick(Entity* e) override
    {
        if(e->has_component<CameraFollow>())
        {
            auto* camera_follow = e->get_component<CameraFollow>();
            auto* transform = e->transform;
            if(camera_follow->target)
            {
                if(camera_follow->target->has_component<Transform>())
                {
                    auto* target_transform = camera_follow->target->transform;
                    transform->pos = transform->pos + (target_transform->pos - transform->pos) * camera_follow->speed * delta_time;
                }
            }
        }
    }
};

class EditorDebug : public System
{
public:
    void draw(Pomegranate::Entity *e) override
    {
#ifdef EDITOR_MODE
        if(e->has_component<Sprite>() && e->has_component<Transform>())
        {
            //Draw bounding box
            auto* sprite = e->get_component<Sprite>();
            auto* transform = e->transform;
            SDL_FRect rect = {transform->pos.x, transform->pos.y, (float)sprite->texture->get_size().x*transform->scale.x,(float)sprite->texture->get_size().y*transform->scale.y};
            //Adjust for camera zoom position and stuff
            rect.x -= Camera::current->transform->pos.x;
            rect.y -= Camera::current->transform->pos.y;
            rect.x *= Camera::current->get_component<Camera>()->zoom;
            rect.y *= Camera::current->get_component<Camera>()->zoom;
            rect.w *= Camera::current->get_component<Camera>()->zoom;
            rect.h *= Camera::current->get_component<Camera>()->zoom;
            rect.x -= rect.w/2;
            rect.y -= rect.h/2;
            //Offset to sdl_renderer center
            int screen_w = 0;
            int screen_h = 0;
            float render_scale_x = 1.0;
            float render_scale_y = 1.0;
            SDL_GetRenderScale(Window::current->get_sdl_renderer(), &render_scale_x, &render_scale_y);
            SDL_GetCurrentRenderOutputSize(Window::current->get_sdl_renderer(), &screen_w, &screen_h);
            screen_w /= render_scale_x;
            screen_h /= render_scale_y;
            rect.x += screen_w/2;
            rect.y += screen_h/2;
            SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 255, 0, 0, 255);
            SDL_RenderRect(Window::current->get_sdl_renderer(), &rect);
        }
#endif
    }
};
class Transform3D : public Component
{
public:
    Vec3 pos;
    Vec3 scale;
    Vec3 rotation;
    void init(Entity* e) override
    {
        pos = Vec3();
        scale = Vec3(1.0,1.0,1.0);
        rotation = Vec3();
        register_component(Transform3D);
        push_data<Vec3>("pos", &pos);
        push_data<Vec3>("scale", &scale);
        push_data<Vec3>("rotation", &rotation);
    }
};

class CubeComponent : public Component
{
public:
    float size;
    void init(Entity* e) override
    {
        size = 1.0;
        e->require_component<Transform3D>();
        register_component(CubeComponent);
        push_data<float>("size", &size);
    }
};

class Render3D : public System
{
public:
    Mat projection;
    Mat view;
    Render3D()
    {
        projection = Mat::perspective(1.57, (float)Window::current->get_width()/(float)Window::current->get_height(), 0.01, 1000.0);
        view = Mat::look_at(Vec3(0,0,0),Vec3(0,0,1),Vec3(0,1,0));
    }
    void draw(Entity* e) override
    {
        if(e->has_component<Transform3D>() && e->has_component<CubeComponent>())
        {
            print_info("Drawing cube");
            auto* transform = e->get_component<Transform3D>();
            auto* cube = e->get_component<CubeComponent>();
            //Calculate cube vertices
            Vec3 vertices[8] = {
                    Vec3(-cube->size/2,-cube->size/2,-cube->size/2),
                    Vec3(cube->size/2,-cube->size/2,-cube->size/2),
                    Vec3(cube->size/2,cube->size/2,-cube->size/2),
                    Vec3(-cube->size/2,cube->size/2,-cube->size/2),
                    Vec3(-cube->size/2,-cube->size/2,cube->size/2),
                    Vec3(cube->size/2,-cube->size/2,cube->size/2),
                    Vec3(cube->size/2,cube->size/2,cube->size/2),
                    Vec3(-cube->size/2,cube->size/2,cube->size/2)
            };
            //Adjust based on transform
            for(int i = 0; i < 8; i++)
            {
                vertices[i] = vertices[i] * transform->scale;
                //Calculate angle (float) and axis for rotation
                Vec3 axis = Vec3(1,0,0);
                float angle = transform->rotation.x;
                vertices[i] = vertices[i].rotate(angle, axis);
                axis = Vec3(0,1,0);
                angle = transform->rotation.y;
                vertices[i] = vertices[i].rotate(angle, axis);
                axis = Vec3(0,0,1);
                angle = transform->rotation.z;
                vertices[i] = vertices[i].rotate(angle, axis);
                vertices[i] = vertices[i] + transform->pos;
                Vec4 v = Vec4(vertices[i].x, vertices[i].y, vertices[i].z, 1.0);
                //Adjust for matricies
                v = (projection*view)*v;
                vertices[i] = Vec3(v.x/v.w, v.y/v.w, v.z/v.w);
                //Adjust for screen

                int renderer_width = 0;
                int renderer_height = 0;
                SDL_GetCurrentRenderOutputSize(Window::current->get_sdl_renderer(), &renderer_width, &renderer_height);

                vertices[i].x *= (float)renderer_width/2;
                vertices[i].y *= (float)renderer_height/2;
                vertices[i].x += (float)renderer_width/2;
                vertices[i].y += (float)renderer_height/2;
            }
            //Draw lines
            SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 255, 255, 255, 255);
            for(int i = 0; i < 8; i++)
            {
                //Draw point
                SDL_RenderPoint(Window::current->get_sdl_renderer(), vertices[i].x, vertices[i].y);
            }
            //Draw lines
            for(int i = 0; i < 4; i++)
            {
                SDL_RenderLine(Window::current->get_sdl_renderer(), vertices[i].x, vertices[i].y, vertices[(i+1)%4].x, vertices[(i+1)%4].y);
                SDL_RenderLine(Window::current->get_sdl_renderer(), vertices[i+4].x, vertices[i+4].y, vertices[((i+1)%4)+4].x, vertices[((i+1)%4)+4].y);
                SDL_RenderLine(Window::current->get_sdl_renderer(), vertices[i].x, vertices[i].y, vertices[i+4].x, vertices[i+4].y);
            }
        }
    }
};

#include "particles.cpp"