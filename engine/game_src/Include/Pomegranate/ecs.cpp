#include "ecs.h"

namespace Pomegranate
{
    //Globals
    std::vector<System*> System::global_systems = std::vector<System*>();
    std::unordered_map<uint32_t ,Entity*> Entity::entities = std::unordered_map<uint32_t,Entity*>();
    std::unordered_map<std::string, std::function<Component*()>> Component::component_types = std::unordered_map<std::string, std::function<Component*()>>();
    std::unordered_map<std::string, std::function<System*()>> System::system_types = std::unordered_map<std::string, std::function<System*()>>();
    uint32_t Entity::entity_count = 0;
    uint32_t Group::group_count = 0;
    std::unordered_map<std::string, Group*> Group::groups = std::unordered_map<std::string, Group*>();
    std::unordered_map<uint32_t, Group*> Group::groups_id = std::unordered_map<uint32_t, Group*>();
    std::vector<Entity*> Entity::destroy_queue = std::vector<Entity*>();

    Entity::Entity()
    {
        this->id = Entity::entity_count++;
        Entity::entities.emplace(this->id,this);
        this->components = std::unordered_multimap<const std::type_info*,Component*>();
        this->parents = std::vector<Group*>();
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

    Group::Group(const std::string& name)
    {
        this->entities = std::vector<Entity*>();
        this->systems = std::vector<System*>();
        this->child_groups = std::vector<Group*>();
        this->name = name;
        groups.emplace(name,this);
        this->id = Group::group_count++;
        groups_id.emplace(this->id,this);
    }

    Group::~Group()
    {
        //Remove this group from the groups map
        groups.erase(this->name);
    }

    void Group::add_entity(Entity* entity)
    {
        entity->add_to_group(this);
        this->entities.push_back(entity);
    }

    void Group::remove_entity(Entity* entity)
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

    void Group::add_system(System * system)
    {
        this->systems.push_back(system);
    }

    void Group::remove_system(System * system)
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
    bool Group::has_system(System * system)
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

    void Group::add_group(Group* Group)
    {
        this->child_groups.push_back(Group);
        Group->parent = this;
    }

    void Group::remove_group(Group* Group)
    {
        for (int i = 0; i < child_groups.size(); ++i)
        {
            if(child_groups[i] == Group)
            {
                child_groups.erase(child_groups.begin() + i);
                return;
            }
        }
    }

    void Group::tick()
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

    void Group::draw(const std::function<bool(Entity*, Entity*)>& sortingFunction)
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

    Group* Group::get_group(const std::string& name)
    {
        return groups[name];
    }
    std::vector<Entity*>* Group::get_entities()
    {
        return &this->entities;
    }
    std::vector<System*>* Group::get_systems()
    {
        return &this->systems;
    }
    std::vector<Group*>* Group::get_child_groups()
    {
        return &this->child_groups;
    }

    Group *Group::get_parent() {
        return this->parent;
    }

    void Entity::add_to_group(Group * group)
    {
        this->parents.push_back(group);
    }

    void Entity::remove_from_group(Group * group)
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

    std::vector<Group*> Entity::get_parent_groups()
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

    Entity *Entity::duplicate()
    {
        auto* entity = new Entity();
        entity->name = this->name;
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
                if(data.first == &typeid(int))
                {
                    *(int*)data.second = *(int*)my->component_data[type].second;
                }
                if(data.first == &typeid(float))
                {
                    *(float*)data.second = *(float*)my->component_data[type].second;
                }
                if(data.first == &typeid(double))
                {
                    *(double*)data.second = *(double*)my->component_data[type].second;
                }
                if(data.first == &typeid(bool))
                {
                    *(bool*)data.second = *(bool*)my->component_data[type].second;
                }
                if(data.first == &typeid(std::string))
                {
                    *(std::string*)data.second = *(std::string*)my->component_data[type].second;
                }
                if(data.first == &typeid(Vec2))
                {
                    *(Vec2*)data.second = *(Vec2*)my->component_data[type].second;
                }
                if(data.first == &typeid(Vec3))
                {
                    *(Vec3*)data.second = *(Vec3*)my->component_data[type].second;
                }
                if(data.first == &typeid(Color))
                {
                    *(Color*)data.second = *(Color*)my->component_data[type].second;
                }
                if(data.first == &typeid(Texture*))
                {
                    *(Texture**)data.second = *(Texture**)my->component_data[type].second;
                }
                if(data.first == &typeid(Audio*))
                {
                    *(Audio**)data.second = *(Audio**)my->component_data[type].second;
                }
                if(data.first == &typeid(TTFFont*))
                {
                    *(TTFFont**)data.second = *(TTFFont**)my->component_data[type].second;
                }
            }
        }
        return entity;
    }

    void Component::init(Entity *){}
}