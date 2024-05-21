#include "standard_ecs.h"
namespace Pomegranate
{
    PositionLink::PositionLink()
    {
        this->link = nullptr;
        this->offset = Vec2(0.0, 0.0);
        register_component(PositionLink);
        //TODO: Add support for entity links in lua
        push_data<Vec2>("offset", &this->offset);
    }

    RotationLink::RotationLink()
    {
        this->link = nullptr;
        this->offset = 0.0;
        register_component(RotationLink);
        //TODO: Add support for entity links in lua
        push_data<float>("offset", &this->offset);
    }

    ScaleLink::ScaleLink()
    {
        this->link = nullptr;
        this->offset = Vec2(0.0, 0.0);
        register_component(ScaleLink);
        //TODO: Add support for entity links in lua
        push_data<Vec2>("offset", &this->offset);
    }

    void TransformLinkages::tick(Entity *entity)
    {
        if(entity->has_component<Transform>())
        {
            if (entity->has_component<PositionLink>())
            {
                auto *t = entity->transform;
                auto *l = entity->get_component<PositionLink>();
                t->pos = l->link->pos + l->offset;
            }
            if (entity->has_component<RotationLink>())
            {
                auto *t = entity->transform;
                auto *l = entity->get_component<RotationLink>();
                t->rot = l->link->rot + l->offset;
            }
            if (entity->has_component<ScaleLink>())
            {
                auto *t = entity->transform;
                auto *l = entity->get_component<ScaleLink>();
                t->scale = l->link->scale + l->offset;
            }
        }
    }

    TransformLinkages::TransformLinkages()
    {
        register_system(TransformLinkages);
    }
}