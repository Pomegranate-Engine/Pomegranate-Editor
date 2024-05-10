enum ParticleEmission2DShape
{
    PARTICLE_EMISSION_SHAPE_2D_POINT,
    PARTICLE_EMISSION_SHAPE_2D_CIRCLE,
    PARTICLE_EMISSION_SHAPE_2D_RECTANGLE
};

class Particle
{
public:
    Vec2 position;
    Vec2 velocity;
    float rotation;
    float angular_velocity;
    float lifetime;
    float size;
    Color color;
    int frame;

    Particle()
    {
        position = Vec2();
        velocity = Vec2();
        rotation = 0.0;
        angular_velocity = 0.0;
        lifetime = 0.0;
        size = 0.0;
        color = Color(1.0f, 1.0f, 1.0f, 1.0f);
        frame = 0;
    }
};

class ParticleEmitter : public Component
{
public:
    bool emitting = true;
    float time_scale = 1.0;
    int max_particles = 100;
    float emission_rate = 1.0;
    float lifetime = 1.0;
    float min_size = 1.0;
    float max_size = 1.0;
    float min_rotation = 0.0;
    float max_rotation = 0.0;
    Texture* texture = nullptr;
    Color start_color = Color(1.0f, 1.0f, 1.0f, 1.0f);
    Color end_color = Color(1.0f, 1.0f, 1.0f, 1.0f);

    bool gravity_enabled = false;
    Vec2 gravity = Vec2(0.0, 980);

    float min_speed = 100.0;
    float max_speed = 200.0;

    float min_angular_speed;
    float max_angular_speed;

    Vec2 direction = Vec2(0.0, -1.0);
    float spread = 0.0;

    std::unordered_multimap<bool,Particle> particles;
    float delta;

    ParticleEmitter()
    {
        register_component(ParticleEmitter);
        push_data<bool>("emitting", &emitting);
        push_data<float>("time_scale", &time_scale);
        push_data<int>("max_particles", &max_particles);
        push_data<float>("emission_rate", &emission_rate);
        push_data<float>("lifetime", &lifetime);
        push_data<float>("min_size", &min_size);
        push_data<float>("max_size", &max_size);
        push_data<float>("min_rotation", &min_rotation);
        push_data<float>("max_rotation", &max_rotation);
        push_data<Texture*>("texture", &texture);
        push_data<Color>("start_color", &start_color);
        push_data<Color>("end_color", &end_color);


        //Gravity
        push_data<bool>("gravity_enabled", &gravity_enabled);
        push_data<Vec2>("gravity", &gravity);

        //Speed
        push_data<float>("min_speed", &min_speed);
        push_data<float>("max_speed", &max_speed);

        //Angular speed
        push_data<float>("min_angular_speed", &min_angular_speed);
        push_data<float>("max_angular_speed", &max_angular_speed);

        //Direction
        push_data<Vec2>("direction", &direction);
        push_data<float>("spread", &spread);

    }

    void init(Pomegranate::Entity *e) override
    {
        delta = 0.0;
        e->require_component<Transform>();
    }
};

class ParticleSystem : public System
{
public:
    ParticleSystem()
    {

    }

    bool update_particle(float d,ParticleEmitter* e, Transform* t, Particle* particle, std::unordered_multimap<bool,Particle>::iterator* itr = nullptr)
    {
        particle->lifetime -= d;
        if (particle->lifetime <= 0)
        {
            //Change particle key to false
            if(itr != nullptr)
            {
                return true;
            }
        }
        else
        {
            if (e->gravity_enabled)
            {
                particle->velocity += e->gravity * d;
            }
            particle->position += particle->velocity * d;
            particle->rotation += particle->angular_velocity * d;
            particle->color = e->start_color.mix(e->end_color, 1 - particle->lifetime / e->lifetime);
            //Draw particle
            Vec2 pos = particle->position;
            //Adjust position for camera position and zoom
            pos.x -= Camera::current_render_position.x;
            pos.y -= Camera::current_render_position.y;
            pos.x *= Camera::current_render_zoom;
            pos.y *= Camera::current_render_zoom;
            //Offset to sdl_renderer center
            int screen_w = 0;
            int screen_h = 0;
            float render_scale_x = 1.0;
            float render_scale_y = 1.0;
            SDL_GetRenderScale(Window::current->get_sdl_renderer(), &render_scale_x, &render_scale_y);
            SDL_GetCurrentRenderOutputSize(Window::current->get_sdl_renderer(), &screen_w, &screen_h);
            screen_w /= render_scale_x;
            screen_h /= render_scale_y;
            pos.x += screen_w / 2;
            pos.y += screen_h / 2;

            Vec2 size = Vec2(particle->size, particle->size);
            size.x *= Camera::current_render_zoom;
            size.y *= Camera::current_render_zoom;

            pos.x -= size.x / 2;
            pos.y -= size.y / 2;

            SDL_FRect rect = {pos.x, pos.y, size.x, size.y};

            if(e->texture == nullptr)
            {
                SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), particle->color.r, particle->color.g,
                                       particle->color.b, particle->color.a);
                SDL_RenderFillRect(Window::current->get_sdl_renderer(), &rect);
            }
            else
            {
                SDL_FRect src = {0, 0, (float)e->texture->get_size().x, (float)e->texture->get_size().y};
                SDL_FRect dst = {pos.x, pos.y, size.x, size.y};
                SDL_FPoint center = {size.x / 2, size.y / 2};
                SDL_SetTextureColorMod(e->texture->get_sdl_texture(), particle->color.r, particle->color.g, particle->color.b);
                SDL_SetTextureAlphaMod(e->texture->get_sdl_texture(), particle->color.a);
                SDL_RenderTextureRotated(Window::current->get_sdl_renderer(), e->texture->get_sdl_texture(), &src, &dst, (double)particle->rotation, &center, SDL_FLIP_NONE);
            }
        }
        return false;
    }

    void pre_draw() override
    {
        //Set camera position
        if(Camera::current != nullptr)
        {
            Camera::current_render_position = Camera::current->get_component<Transform>()->pos;
            Camera::current_render_zoom = Camera::current->get_component<Camera>()->zoom;
        }
    }
    void draw(Entity* e) override
    {
        if(e->has_component<ParticleEmitter>())
        {
            auto* emitter = e->get_component<ParticleEmitter>();
            auto* transform = e->get_component<Transform>();
            float d = delta_time * emitter->time_scale;
            if(emitter->particles.size() != emitter->max_particles)
            {
                emitter->particles.clear();
                for(int i = 0; i < emitter->max_particles; i++)
                {
                    emitter->particles.insert({false, Particle()});
                }
            }
            if(emitter->emitting) {
                emitter->delta += d * emitter->emission_rate;
                //Draw particles
                std::vector<bool> inactive = {false};
                while (emitter->delta > 1) {
                    emitter->delta--;
                    Particle p;
                    p.position = transform->pos;
                    p.velocity = emitter->direction;
                    //Randomize velocity based on spread (0-2PI)
                    float angle = (rand() % 1000) / 1000.0 * emitter->spread;
                    p.velocity = p.velocity.rotate(angle).rotate(transform->rot).rotate(-emitter->spread / 2.0);
                    p.velocity = p.velocity * (emitter->min_speed +
                                               (rand() % 1000) / 1000.0 * (emitter->max_speed - emitter->min_speed));
                    p.angular_velocity = emitter->min_angular_speed + (rand() % 1000) / 1000.0 *
                                                                      (emitter->max_angular_speed -
                                                                       emitter->min_angular_speed);
                    p.lifetime = emitter->lifetime;
                    p.size = emitter->min_size + (rand() % 1000) / 1000.0 * (emitter->max_size - emitter->min_size);
                    p.rotation = emitter->min_rotation +
                                 (rand() % 1000) / 1000.0 * (emitter->max_rotation - emitter->min_rotation);
                    p.color = emitter->start_color;
                    //Find first inactive particle
                    bool found = false;
                    auto i = emitter->particles.find(found);
                    if (i != emitter->particles.end()) {
                        //Extract particle and change it to true
                        auto handler = emitter->particles.extract(i);
                        handler.key() = true;
                        handler.mapped() = p;
                        emitter->particles.insert(std::move(handler));
                    }
                }
            }
            std::vector<std::unordered_multimap<bool,Particle>::iterator> to_remove;
            for (auto[itr, rangeEnd] = emitter->particles.equal_range(true); itr != rangeEnd; ++itr)
            {
                bool should_remove = update_particle(d,emitter, transform,&itr->second, &itr);
                if(should_remove)
                {
                    to_remove.push_back(itr);
                }
            }
            for(auto& i : to_remove)
            {
                //Set the key to false
                auto handler = emitter->particles.extract(i);
                handler.key() = false;
                emitter->particles.insert(std::move(handler));
            }
        }
    }
};