#include "ecs.h"

namespace Pomegranate
{
    //Globals
    std::vector<System*> System::global_systems = std::vector<System*>();
    std::unordered_map<uint32_t ,Entity*> Entity::entities = std::unordered_map<uint32_t,Entity*>();
    std::unordered_map<std::string, std::function<Component*()>> Component::component_types = std::unordered_map<std::string, std::function<Component*()>>();
    std::unordered_map<std::string, std::function<System*()>> System::system_types = std::unordered_map<std::string, std::function<System*()>>();
    std::unordered_map<std::string, int> LuaComponent::lua_component_types = std::unordered_map<std::string, int>();
    LuaComponent* LuaComponent::current = nullptr;
    uint32_t Entity::entity_count = 0;
    std::unordered_map<std::string, EntityGroup*> EntityGroup::groups = std::unordered_map<std::string, EntityGroup*>();
    std::vector<Entity*> Entity::destroy_queue = std::vector<Entity*>();

    Entity::Entity()
    {
        this->id = Entity::entity_count++;
        Entity::entities.emplace(this->id,this);
        this->components = std::unordered_multimap<const std::type_info*,Component*>();
        this->parents = std::vector<EntityGroup*>();
        this->refs = std::vector<Entity*>();
        this->name = "NewEntity";
    }

    uint32_t Entity::get_id() const
    {
        return this->id;
    }
    Component* Entity::get_component(const char* name)
    {
        for (auto c : components)
        {
            if (std::string(c.first->name()) == "class " + std::string(name))
            {
                return c.second;
            }
        }
        return nullptr;
    }

    void Entity::add_component(const char *name)
    {
        //Check if it starts with "class"
        std::string n = std::string(name);
        auto component = Component::component_types[n]();
        component->init(this);
        std::pair<const std::type_info*, Component *> pair(&typeid(*component), component);
        this->components.insert(pair);
    }

    bool Entity::has_component(const char * name)
    {
        for (auto c : components)
        {
            if (std::string(c.first->name()) == "class " + std::string(name))
            {
                return true;
            }
        }
        return false;
    }

    Entity::~Entity()
    {
        for (auto & component : this->components)
        {
            delete component.second;
        }
        this->components.clear();
        Entity::entities.erase(this->id);
    }


    void Entity::remove_component(Component* component) {
        for (auto c : components)
        {
            if (c.second == component)
            {
                components.erase(c.first);
                delete component;
                return;
            }
        }
    }


    System::System() = default;

    System::~System() = default;

    void System::init(Entity*) {}
    void System::tick(Entity*) {}
    void System::draw(Entity*) {}
    void System::pre_tick(){}
    void System::pre_draw(){}
    void System::post_tick(){}
    void System::post_draw(){}

    void System::global_system_tick()
    {
        for(auto & system : System::global_systems)
        {
            if(system->active)
            {
                system->pre_tick();
                Entity::apply_destruction_queue();
                for (auto &entity: Entity::entities)
                {
                    system->tick(entity.second);
                }
                Entity::apply_destruction_queue();
                system->post_tick();
                Entity::apply_destruction_queue();
            }
        }
    }

    void System::add_global_system(System * system)
    {
        System::global_systems.push_back(system);
    }

    void System::remove_global_system(System * system)
    {
        for (int i = 0; i < System::global_systems.size(); ++i)
        {
            if (System::global_systems[i] == system)
            {
                System::global_systems.erase(System::global_systems.begin() + i);
                return;
            }
        }
    }

    void System::global_system_draw(const std::function<bool(Entity*, Entity*)>& sortingFunction)
    {
        //TODO: Sort entities based on transform component

        for (auto& system : System::global_systems)
        {
            if(system->active)
            {
                system->pre_draw();
                Entity::apply_destruction_queue();
                for (auto &entity: Entity::entities)
                {
                    system->draw(entity.second);
                }
                Entity::apply_destruction_queue();
                system->post_draw();
                Entity::apply_destruction_queue();
            }
        }
    }

    EntityGroup::EntityGroup(const std::string& name)
    {
        this->entities = std::vector<Entity*>();
        this->systems = std::vector<System*>();
        this->child_groups = std::vector<EntityGroup*>();
        this->name = name;
        groups.emplace(name,this);
    }

    EntityGroup::~EntityGroup()
    {
        //Remove this group from the groups map
        groups.erase(this->name);
    }

    void EntityGroup::add_entity(Entity* entity)
    {
        entity->add_to_group(this);
        this->entities.push_back(entity);
    }

    void EntityGroup::remove_entity(Entity* entity)
    {
        for (auto & entitie : entities)
        {
            if (entitie->get_id() == entity->get_id())
            {
                entitie->remove_from_group(this);
                this->entities.erase(std::remove(this->entities.begin(), this->entities.end(), entitie), this->entities.end());
                return;
            }
        }
        for(auto & child_group : this->child_groups)
        {
            child_group->remove_entity(entity);
        }
    }

    void EntityGroup::add_system(System * system)
    {
        this->systems.push_back(system);
    }

    void EntityGroup::remove_system(System * system)
    {
        //TODO: Implement system removal
    }

    void EntityGroup::add_group(EntityGroup* entityGroup)
    {
        this->child_groups.push_back(entityGroup);
        entityGroup->parent = this;
    }

    void EntityGroup::remove_group(EntityGroup* entityGroup)
    {
        for (int i = 0; i < child_groups.size(); ++i)
        {
            if(child_groups[i] == entityGroup)
            {
                child_groups.erase(child_groups.begin() + i);
                return;
            }
        }
    }

    void EntityGroup::tick()
    {
        for(auto & system : this->systems)
        {
            if(system->active)
            {
                system->pre_tick();
                Entity::apply_destruction_queue();

                for (auto &entitie: this->entities)
                {
                    system->tick(entitie);
                }
                Entity::apply_destruction_queue();
                system->post_tick();
                Entity::apply_destruction_queue();
            }
        }
        for(auto & child_group : this->child_groups)
        {
            child_group->tick();
        }
    }

    void EntityGroup::draw(const std::function<bool(Entity*, Entity*)>& sortingFunction)
    {
        // Sort entities using the provided sorting function
        if(sortingFunction!= nullptr)
        std::sort(this->entities.begin(), this->entities.end(), sortingFunction);
        for(auto & system : this->systems)
        {
            if(system->active)
            {
                system->pre_draw();
                Entity::apply_destruction_queue();
                for (auto &entity: this->entities)
                {
                    system->draw(entity);
                }
                Entity::apply_destruction_queue();
                system->post_draw();
                Entity::apply_destruction_queue();
            }
        }
        for(auto & group : this->child_groups)
        {
            group->draw(sortingFunction);
        }
    }

    EntityGroup* EntityGroup::get_group(const std::string& name)
    {
        return groups[name];
    }
    std::vector<Entity*>* EntityGroup::get_entities()
    {
        return &this->entities;
    }
    std::vector<System*>* EntityGroup::get_systems()
    {
        return &this->systems;
    }
    std::vector<EntityGroup*>* EntityGroup::get_child_groups()
    {
        return &this->child_groups;
    }

    EntityGroup *EntityGroup::get_parent() {
        return this->parent;
    }

    void Entity::add_to_group(EntityGroup * group)
    {
        this->parents.push_back(group);
    }

    void Entity::remove_from_group(EntityGroup * group)
    {
        for (int i = 0; i < this->parents.size(); ++i)
        {
            if (this->parents[i] == group)
            {
                this->parents.erase(this->parents.begin() + i);
                return;
            }
        }
    }

    std::vector<EntityGroup*> Entity::get_parent_groups()
    {
        return this->parents;
    }

    void Entity::orphan()
    {
        for (auto & parent : this->parents)
        {
            parent->remove_entity(this);
        }
    }

    void Entity::destroy()
    {
        destroy_queue.push_back(this);
    }

    void Entity::force_destroy()
    {
        clean_refs();
        this->orphan();
        delete this;
    }

    void Entity::apply_destruction_queue()
    {
        for (auto & entity : destroy_queue)
        {
            entity->force_destroy();
        }
        destroy_queue.clear();
    }

    void Entity::clean_refs()
    {
        for (auto & ref : this->refs)
        {
            ref = nullptr;
        }
        this->refs.clear();
    }

    void Entity::get_ref(Entity * &e)
    {
        e = this;
        this->refs.push_back(e);
    }

    std::unordered_multimap<const std::type_info*,Component*> Entity::get_components() {
        return components;
    }

    void Component::init(Entity *){}
}