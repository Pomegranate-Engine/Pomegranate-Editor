#ifndef POMEGRANATE_ENGINE_EVENT_SYSTEM_H
#define POMEGRANATE_ENGINE_EVENT_SYSTEM_H
#include "ecs.h"
#include <functional>
namespace Pomegranate
{
    class Event
    {
    public:
        std::vector<std::pair<Entity*,std::function<void(Entity*)>>> callbacks;
        Event();
        void trigger();
        void add_listener(const std::function<void(Entity*)>& callback);
        void remove_listener(const std::function<void(Entity*)>& callback);
    };
}

#include "event_system.inl"

#endif //POMEGRANATE_ENGINE_EVENT_SYSTEM_H
