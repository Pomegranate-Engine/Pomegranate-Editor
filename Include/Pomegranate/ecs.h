#ifndef POMEGRANATE_ENGINE_ECS_H
#define POMEGRANATE_ENGINE_ECS_H
#include <typeinfo>
#include <iostream>
#include <vector>
#include <unordered_set>
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
    class Group;

    class EntityRef
    {
    private:
        /* data */
        Entity* entity;
        static std::unordered_set<EntityRef*> refs;
    public:
        EntityRef();
        EntityRef(Entity* entity);
        ~EntityRef();
        Entity* operator->();
        Entity* operator=(Entity* entity);
        Entity* operator=(const EntityRef& entity);
        bool operator==(Entity* entity);
        bool operator!=(Entity* entity);
        bool operator==(const EntityRef& entity);
        bool operator!=(const EntityRef& entity);
        Entity* get();
        static void destroy(Entity* entity);
    };

    class GroupRef
    {
    private:
        /* data */
        Group* group;
        static std::unordered_set<GroupRef*> refs;
    public:
        GroupRef();
        GroupRef(Group* group);
        ~GroupRef();
        Group* operator->();
        Group* operator=(Group* group);
        Group* operator=(const GroupRef& group);
        bool operator==(Group* group);
        bool operator!=(Group* group);
        bool operator==(const GroupRef& group);
        bool operator!=(const GroupRef& group);
        Group* get();
        static void destroy(Group* group);
    };

    class Component
    {
    private:

    public:
        virtual ~Component()=default;
        virtual void init(Entity*);
        std::map<std::string,std::pair<const std::type_info*, void*>> component_data;
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
        std::vector<GroupRef> parents;
    public:
        uint32_t id;
        std::string name;
        std::unordered_multimap<const std::type_info*,Component*> get_components();
        template <typename T> T* add_component();
        template <typename... T> void add_components();
        Component* add_component(const char* name);
        void remove_component(Component*);
        void add_to_group(GroupRef);
        void remove_from_group(GroupRef);
        std::vector<GroupRef> get_parent_groups();
        template <typename T> T* get_component();
        Component* get_component(const char*);
        template <typename T> T* require_component();
        template <typename T> bool has_single_component();
        template <typename... T> bool has_component();
        bool has_component(const char*);
        EntityRef duplicate();
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
        static EntityRef create(std::string name);
    };

    class Group
    {
    private:
        /* data */

    public:
        std::vector<EntityRef> entities;
        std::vector<System*> systems;
        std::vector<GroupRef> child_groups;
        GroupRef parent = nullptr;
        static uint32_t group_count;
        uint32_t id;
        std::string name;
        explicit Group(const std::string& name);

        ~Group();
        GroupRef get_parent();
        virtual void add_entity(EntityRef);
        virtual void remove_entity(EntityRef);
        virtual void add_system(System*);
        virtual void remove_system(System*);
        virtual bool has_system(System*);
        virtual void add_group(GroupRef);
        virtual void remove_group(GroupRef);
        virtual void force_destroy();
        virtual void destroy();
        virtual void tick();
        virtual void orphan();
        virtual void draw(const std::function<bool(EntityRef, EntityRef)>& sortingFunction);
        virtual std::vector<EntityRef> get_entities();
        virtual std::vector<EntityRef> get_all_entities();
        virtual std::vector<System*>* get_systems();
        virtual std::vector<GroupRef> get_child_groups();
        void set_id(uint32_t id);
        static std::unordered_map<std::string,Group*> groups;
        static std::unordered_map<uint32_t,Group*> groups_id;
        static std::vector<Group*> destroy_queue;
        static GroupRef get_group(const std::string& name);
        static void apply_destruction_queue();
    };

    class AutoGroup : public Group
    {
    private:
        /* data */
    public:
        std::vector<const std::type_info*> component_types;
        std::vector<EntityRef> find_entities();
        AutoGroup(const std::string& name);

        template<typename... T> static GroupRef create(const std::string& name);
        ~AutoGroup();
        template <typename T> void add_component_type();
        void add_component_type(std::string);
        template <typename T> void remove_component_type();
        void remove_component_type(std::string);
        void tick() override;
        void draw(const std::function<bool(EntityRef, EntityRef)>& sortingFunction) override;
    };
}

#include "ecs.inl"

#endif //POMEGRANATE_ENGINE_ECS_H