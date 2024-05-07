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
    std::unordered_map<std::string, GroupRef> Group::groups = std::unordered_map<std::string, GroupRef>();
    std::unordered_map<uint32_t, GroupRef> Group::groups_id = std::unordered_map<uint32_t, GroupRef>();
    std::vector<Entity*> Entity::destroy_queue = std::vector<Entity*>();
    std::unordered_set<EntityRef*> EntityRef::refs = std::unordered_set<EntityRef*>();
    std::unordered_set<GroupRef*> GroupRef::refs = std::unordered_set<GroupRef*>();
#pragma region Refs

#pragma region EntityRef
    EntityRef::EntityRef()
    {
        this->entity = nullptr;
        EntityRef::refs.insert(this);
    }
    EntityRef::EntityRef(Entity *entity)
    {
        this->entity = entity;
        EntityRef::refs.insert(this);
    }
    EntityRef::~EntityRef()
    {
        EntityRef::refs.erase(this);
    }
    Entity* EntityRef::operator->()
    {
        return this->entity;
    }
    Entity* EntityRef::operator=(Entity* entity)
    {
        this->entity = entity;
        return this->entity;
    }
    Entity* EntityRef::operator=(const EntityRef& entity)
    {
        this->entity = entity.entity;
        return this->entity;
    }
    bool EntityRef::operator==(Entity* entity)
    {
        return this->entity == entity;
    }
    bool EntityRef::operator!=(Entity* entity)
    {
        return this->entity != entity;
    }
    bool EntityRef::operator==(const EntityRef& entity)
    {
        return this->entity == entity.entity;
    }
    bool EntityRef::operator!=(const EntityRef& entity)
    {
        return this->entity != entity.entity;
    }
    Entity* EntityRef::get()
    {
        return this->entity;
    }
    void EntityRef::destroy(Entity* entity)
    {
        //Find all references to this entity and nullify them
        for (auto & ref : EntityRef::refs)
        {
            if(ref->entity == entity)
            {
                ref->entity = nullptr;
            }
        }
    }
#pragma endregion

#pragma region GroupRef

GroupRef::GroupRef()
{
    this->group = nullptr;
    GroupRef::refs.insert(this);
}

GroupRef::GroupRef(Group *group)
{
    this->group = group;
    GroupRef::refs.insert(this);
}

GroupRef::~GroupRef()
{
    GroupRef::refs.erase(this);
}

Group *GroupRef::operator->()
{
    return this->group;
}

Group *GroupRef::operator=(Group *group)
{
    this->group = group;
    return this->group;
}

Group *GroupRef::operator=(const GroupRef &group)
{
    this->group = group.group;
    return this->group;
}

bool GroupRef::operator==(Group *group)
{
    return this->group == group;
}

bool GroupRef::operator!=(Group *group)
{
    return this->group != group;
}

bool GroupRef::operator==(const GroupRef &group)
{
    return this->group == group.group;
}

bool GroupRef::operator!=(const GroupRef &group)
{
    return this->group != group.group;
}

Group *GroupRef::get()
{
    return this->group;
}

void GroupRef::destroy(Group *group)
{
    //Find all references to this entity and nullify them
    for (auto & ref : GroupRef::refs)
    {
        if(ref->group == group)
        {
            ref->group = nullptr;
        }
    }
}

#pragma endregion
#pragma endregion

#pragma region Entity
    Entity::Entity()
    {
        this->id = Entity::entity_count++;
        Entity::entities.emplace(this->id,this);
        this->components = std::unordered_multimap<const std::type_info*,Component*>();
        this->parents = std::vector<GroupRef>();
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
        this->orphan();
        //delete from Entity::entities
        //Destroy references
        EntityRef::destroy(this);
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

    void Entity::add_to_group(GroupRef group)
    {
        this->parents.push_back(group);
    }

    void Entity::remove_from_group(GroupRef group)
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

    std::vector<GroupRef> Entity::get_parent_groups()
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

    EntityRef Entity::duplicate()
    {
        auto entity = Entity::create(this->name);
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

    EntityRef Entity::create(std::string name)
    {
        Entity* entity = new Entity();
        return EntityRef(entity);
    }
#pragma endregion

#pragma region Component
    void Component::init(Entity *){}

#pragma endregion

#pragma region System

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

#pragma endregion

#pragma region Group

    Group::Group(const std::string& name)
    {
        this->entities = std::vector<EntityRef>();
        this->systems = std::vector<System*>();
        this->child_groups = std::vector<GroupRef>();
        this->name = name;
        groups.emplace(name,this);
        this->id = Group::group_count++;
        groups_id.emplace(this->id,this);
    }

    Group::~Group()
    {
        //Remove this group from the groups map
        groups.erase(this->name);
        groups_id.erase(this->id);
        //Remove all entities
        for (auto & entity : this->entities)
        {
            entity->remove_from_group(this);
        }
        //Delete references
        GroupRef::destroy(this);
    }

    void Group::add_entity(EntityRef entity)
    {
        entity->add_to_group(this);
        this->entities.push_back(entity);
    }

    void Group::remove_entity(EntityRef entity)
    {
        for (auto & entitie : entities)
        {
            if (entitie->get_id() == entity->get_id())
            {
                entitie->remove_from_group(this);
                entities.erase(std::remove(entities.begin(), entities.end(), entitie), entities.end());

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

    void Group::add_group(GroupRef Group)
    {
        this->child_groups.push_back(Group);
        Group->parent = this;
    }

    void Group::remove_group(GroupRef Group)
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
                    system->tick(entitie.get());
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

    void Group::draw(const std::function<bool(EntityRef, EntityRef)>& sortingFunction)
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
                    system->draw(entity.get());
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

    GroupRef Group::get_group(const std::string& name)
    {
        return groups[name];
    }
    std::vector<EntityRef> Group::get_entities()
    {
        return this->entities;
    }
    std::vector<System*>* Group::get_systems()
    {
        return &this->systems;
    }
    std::vector<GroupRef> Group::get_child_groups()
    {
        return this->child_groups;
    }

    GroupRef Group::get_parent() {
        return this->parent;
    }

    std::vector<EntityRef> Group::get_all_entities()
    {
        //Recursively get all entities
        std::vector<EntityRef> all_entities = std::vector<EntityRef>();
        for(auto & entity : this->entities)
        {
            all_entities.push_back(entity);
        }
        for(auto & group : this->child_groups)
        {
            auto child_entities = group->get_all_entities();
            for(auto & entity : child_entities)
            {
                all_entities.push_back(entity);
            }
        }
        return all_entities;
    }

    void Group::set_id(uint32_t id)
    {
        //Remove the old id
        Group::groups_id.erase(this->id);
        this->id = id;
        //Add the new id
        Group::groups_id.emplace(this->id,this);
        if(Group::group_count <= id)
        {
            Group::group_count = id + 1;
        }
    }

#pragma endregion

#pragma region AutoGroup

    AutoGroup::AutoGroup(const std::string& name) : Group(name)
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
    std::vector<EntityRef> AutoGroup::find_entities()
    {
        std::vector<EntityRef> enties = std::vector<EntityRef>();
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
        std::vector<EntityRef> enties = find_entities();
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
        Group::tick();
    }
    void AutoGroup::draw(const std::function<bool(EntityRef, EntityRef)>& sortingFunction)
    {
        std::vector<EntityRef> enties = find_entities();
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
        Group::draw(sortingFunction);
    }

#pragma endregion
}