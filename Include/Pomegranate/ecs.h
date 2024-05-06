#ifndef POMEGRANATE_ENGINE_ECS_H
#define POMEGRANATE_ENGINE_ECS_H
#include <typeinfo>
#include <iostream>
#include <vector>
#if defined(_OPENMP)
#include <omp.h>
#endif
#include<string>
#include "engine.h"
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <utility>
#include <typeindex>
#include "color.h"
#include "texture.h"
#include "audio.h"
#include "ttf_font.h"

namespace Pomegranate
{
    class Entity;
    class Component;
    class System;
    class EntityGroup;

    class Component
    {
    private:

    public:
        virtual ~Component()=default;
        virtual void init(Entity*);
        std::unordered_map<std::string,std::pair<const std::type_info*, void*>> component_data;
        static std::unordered_map<std::string, std::function<Component*()>> component_types;
        template<typename T> static void register_component_with_name(std::string name);
        template<typename T> void push_data(const std::string& name, void* data);
        template<typename T> T get(const std::string& name);
        template<typename T> void set(const char* name, T value);

    };
#define register_component(T) Component::register_component_with_name<T>(typeid(T).name())


    class System
    {
    private:
        /* data */
        static std::vector<System*> global_systems;
    public:
        bool active = true;
        System();
        ~System();
        virtual void init(Entity*);
        virtual void pre_tick();
        virtual void tick(Entity*);
        virtual void post_tick();
        virtual void pre_draw();
        virtual void draw(Entity*);
        virtual void post_draw();
        static void add_global_system(System*);
        static void remove_global_system(System*);
        static void global_system_tick();
        static void global_system_draw(const std::function<bool(Entity*, Entity*)>& sortingFunction);
        static std::unordered_map<std::string, std::function<System*()>> system_types;
        template<typename T> static void register_system_with_name(std::string name);
    };
#define register_system(T) System::register_system_with_name<T>(typeid(T).name())

    class Entity
    {
    private:
        /* data */
        std::unordered_multimap<const std::type_info*,Component*> components;
        std::vector<EntityGroup*> parents;
        std::vector<Entity*> refs;
    public:
        uint32_t id;
        std::string name;
        std::unordered_multimap<const std::type_info*,Component*> get_components();
        template <typename T> T* add_component();
        template <typename... T> void add_components();
        Component* add_component(const char* name);
        void remove_component(Component*);
        void add_to_group(EntityGroup*);
        void remove_from_group(EntityGroup*);
        std::vector<EntityGroup*> get_parent_groups();
        template <typename T> T* get_component();
        Component* get_component(const char*);
        template <typename T> T* require_component();
        template <typename T> bool has_single_component();
        template <typename... T> bool has_component();
        bool has_component(const char*);
        Entity* duplicate();
        Entity();
        ~Entity();
        [[nodiscard]] uint32_t get_id() const;
        void set_id(uint32_t id);
        //Add a static list of all entities
        static std::unordered_map<uint32_t,Entity*> entities;
        static uint32_t entity_count;
        static std::vector<Entity*> destroy_queue;
        void orphan();
        void destroy();
        void force_destroy();
        static void apply_destruction_queue();
        static Entity* create(std::string name);
    };

    class EntityGroup
    {
    private:
        /* data */

    public:
        std::vector<Entity*> entities;
        std::vector<System*> systems;
        std::vector<EntityGroup*> child_groups;
        EntityGroup* parent = nullptr;
        static uint32_t group_count;
        uint32_t id;
        std::string name;
        explicit EntityGroup(const std::string& name);

        ~EntityGroup();
        EntityGroup* get_parent();
        virtual void add_entity(Entity*);
        virtual void remove_entity(Entity*);
        virtual void add_system(System*);
        virtual void remove_system(System*);
        virtual bool has_system(System*);
        virtual void add_group(EntityGroup*);
        virtual void remove_group(EntityGroup*);
        virtual void tick();
        virtual void draw(const std::function<bool(Entity*, Entity*)>& sortingFunction);
        virtual std::vector<Entity*>* get_entities();
        virtual std::vector<Entity*>* get_all_entities();
        virtual std::vector<System*>* get_systems();
        virtual std::vector<EntityGroup*>* get_child_groups();
        void set_id(uint32_t id);
        static std::unordered_map<std::string,EntityGroup*> groups;
        static std::unordered_map<uint32_t,EntityGroup*> groups_id;
        static EntityGroup* get_group(const std::string& name);
    };

    class AutoGroup : public EntityGroup
    {
    private:
        /* data */
    public:
        std::vector<const std::type_info*> component_types;
        std::vector<Entity*> find_entities();
        AutoGroup(const std::string& name);

        template<typename... T> static AutoGroup* create(const std::string& name);
        ~AutoGroup();
        template <typename T> void add_component_type();
        void add_component_type(std::string);
        template <typename T> void remove_component_type();
        void remove_component_type(std::string);
        void tick() override;
        void draw(const std::function<bool(Entity*, Entity*)>& sortingFunction) override;
    };
}

#include "ecs.inl"

#endif //POMEGRANATE_ENGINE_ECS_H