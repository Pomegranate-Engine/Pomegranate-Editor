#include "ecs.h"

namespace Pomegranate
{
    //Globals
    std::vector<System*> System::global_systems = std::vector<System*>();
    std::unordered_map<uint32_t ,Entity*> Entity::entities = std::unordered_map<uint32_t,Entity*>();
    std::unordered_map<std::string, std::function<Component*()>> Component::component_types = std::unordered_map<std::string, std::function<Component*()>>();
    std::unordered_map<std::string, std::function<System*()>> System::system_types = std::unordered_map<std::string, std::function<System*()>>();
    uint32_t Entity::entity_count = 0;
    uint32_t EntityGroup::group_count = 0;
    std::unordered_map<std::string, EntityGroup*> EntityGroup::groups = std::unordered_map<std::string, EntityGroup*>();
    std::unordered_map<uint32_t, EntityGroup*> EntityGroup::groups_id = std::unordered_map<uint32_t, EntityGroup*>();
    std::vector<Entity*> Entity::destroy_queue = std::vector<Entity*>();

    Entity::Entity()
    {
        this->id = Entity::entity_count++;
        Entity::entities.emplace(this->id,this);
        this->components = std::unordered_multimap<const std::type_info*,Component*>();
        this->parents = std::vector<EntityGroup*>();
        this->refs = std::vector<Entity*>();
        this->name = "New Entity";
    }

    uint32_t Entity::get_id() const
    {
        return this->id;
    }
    Component* Entity::get_component(const char* name)
    {
        for (auto c : components)
        {
            if (std::string(c.first->name()) == std::string(name))
            {
                return c.second;
            }
        }
        return nullptr;
    }

    Component* Entity::add_component(const char *name)
    {
        //Check if it starts with "class"
        if(has_component(name))
        {
            print_error("Component " + std::string(name) + " already exists in entity " + std::to_string(this->id) + "!");
            return nullptr;
        }
        std::string n = std::string(name);
        auto component = Component::component_types[n]();
        component->init(this);
        std::pair<const std::type_info*, Component *> pair(&typeid(*component), component);
        this->components.insert(pair);
        return component;
    }

    bool Entity::has_component(const char * name)
    {
        for (auto c : components)
        {
            if (std::string(c.first->name()) == std::string(name))
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


    void Entity::remove_component(Component* component)
    {
        for (auto c : components)
        {
            if (c.second == component)
            {
                delete c.second;
                components.erase(c.first);
                print_info("Component Removed");
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
        this->id = EntityGroup::group_count++;
        groups_id.emplace(this->id,this);
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
        for (int i = 0; i < systems.size(); ++i)
        {
            if(systems[i] == system)
            {
                delete system;
                systems.erase(systems.begin() + i);
                return;
            }
        }
    }
    bool EntityGroup::has_system(System * system)
    {
        for (auto & sys : systems)
        {
            if (sys == system)
            {
                return true;
            }
        }
        return false;
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

    std::vector<Entity *> *EntityGroup::get_all_entities()
    {
        //Recursively get all entities
        std::vector<Entity*> *all_entities = new std::vector<Entity*>();
        for(auto & entity : this->entities)
        {
            all_entities->push_back(entity);
        }
        for(auto & group : this->child_groups)
        {
            auto *child_entities = group->get_all_entities();
            for(auto & entity : *child_entities)
            {
                all_entities->push_back(entity);
            }
            delete child_entities;
        }
        return all_entities;
    }

    void EntityGroup::set_id(uint32_t id)
    {
        //Remove the old id
        EntityGroup::groups_id.erase(this->id);
        this->id = id;
        //Add the new id
        EntityGroup::groups_id.emplace(this->id,this);
        if(EntityGroup::group_count <= id)
        {
            EntityGroup::group_count = id + 1;
        }
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
        this->orphan();
        //delete from Entity::entities
        Entity::entities.erase(this->id);

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

    std::unordered_multimap<const std::type_info*,Component*> Entity::get_components() {
        return components;
    }

    Entity* Entity::duplicate()
    {
        auto* entity = Entity::create(this->name);
        for (auto & component : this->components)
        {
            if(entity->get_component(component.first->name()) == nullptr)
            {
                entity->add_component(component.first->name());
            }
            //Set data
            auto* my = this->get_component(component.first->name());
            auto* c = entity->get_component(component.first->name());
            for (auto& [type,data] : c->component_data)
            {
                std::memcpy(data.second, my->component_data[type].second, sizeof(data.first));
            }
        }
        return entity;
    }

    void Entity::set_id(uint32_t id)
    {
        //Remove the old id
        Entity::entities.erase(this->id);
        this->id = id;
        //Add the new id
        Entity::entities.emplace(this->id,this);
        if(Entity::entity_count <= id)
        {
            Entity::entity_count = id + 1;
        }
    }

    Entity* Entity::create(std::string name)
    {
        Entity* entity = new Entity();
        return entity;
    }

    void Component::init(Entity *){}

    AutoGroup::AutoGroup(const std::string& name) : EntityGroup(name)
    {
        this->component_types = std::vector<const std::type_info*>();
    }
    AutoGroup::~AutoGroup()
    {
        for (auto & component : this->component_types)
        {
            delete component;
        }
        this->component_types.clear();
    }
    std::vector<Entity*> AutoGroup::find_entities()
    {
        std::vector<Entity*> enties = std::vector<Entity*>();
        for (auto & entity : Entity::entities)
        {
            bool has_all = true;
            for (auto & component : this->component_types)
            {
                if(entity.second->has_component(component->name()))
                {

                }
                else
                {
                    has_all = false;
                    break;
                }
            }
            if(has_all) {
                enties.push_back(entity.second);
            }
        }
        return enties;
    }
    void AutoGroup::add_component_type(std::string name)
    {
        this->component_types.push_back(&typeid(*Component::component_types[name]()));
    }
    void AutoGroup::remove_component_type(std::string name)
    {
        for (int i = 0; i < this->component_types.size(); ++i)
        {
            if(this->component_types[i]->name() == name)
            {
                this->component_types.erase(this->component_types.begin() + i);
                return;
            }
        }
    }
    void AutoGroup::tick()
    {
        std::vector<Entity*> enties = find_entities();
        //Add entities to this group if they're not already
        for (auto & entity : enties)
        {
            if(!std::count(this->entities.begin(),this->entities.end(),entity))
            {
                this->add_entity(entity);
            }
        }
        //Remove entities from this group if they're not in the list
        for (auto & entity : this->entities)
        {
            if(!std::count(enties.begin(),enties.end(),entity))
            {
                this->remove_entity(entity);
                print_info("Entity removed from group");
            }
        }
        //Tick the group
        EntityGroup::tick();
    }
    void AutoGroup::draw(const std::function<bool(Entity*, Entity*)>& sortingFunction)
    {
        std::vector<Entity*> enties = find_entities();
        //Add entities to this group if they're not already
        for (auto & entity : enties)
        {
            if(!std::count(this->entities.begin(),this->entities.end(),entity))
            {
                this->add_entity(entity);
            }
        }
        //Remove entities from this group if they're not in the list
        for (auto & entity : this->entities)
        {
            if(!std::count(enties.begin(),enties.end(),entity))
            {
                this->remove_entity(entity);
                print_info("Entity removed from group");
            }
        }
        //Draw the group
        EntityGroup::draw(sortingFunction);
    }
}