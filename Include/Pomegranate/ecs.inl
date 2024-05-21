namespace Pomegranate
{
    template <typename T> void Component::push_data(const std::string& name, void *data)
    {
        auto p = std::pair<const std::type_info*, void*>(&typeid(T), data);
        this->component_data[name] = p;
    }
    template <typename T> T Component::get(const std::string& name)
    {
        if(this->component_data[name].second != nullptr)
        {
            return *(T*)this->component_data[name].second;
        }
        else
        {
            print_warn("Component " + std::string(typeid(T).name()) + " does not have data " + std::string(name) + "! Returning default value.");
            return T();
        }
    }
    template <typename T> void Component::set(const char *name, T value)
    {
        *(T*)this->component_data[name].second = value;
    }

    template <typename T> inline T* Entity::get_component()
    {
        if(typeid(T) == typeid(Transform))
        {
            return (T*)this->transform;
        }
        if(this->has_component<T>())
        {
            T* component = (T*)this->components.equal_range(&typeid(T)).first->second;
            return component;
        }
        else
        {
            return nullptr;
        }
    }
    template <typename T> inline bool Entity::has_single_component()
    {
        const std::type_info* type = &typeid(T);
        if(type == &typeid(Transform) && this->transform != nullptr)
            return true;
        if(components.size() == 0)
            return false;
        return this->components.find(type) != this->components.end();
    }
    template <typename... T> inline bool Entity::has_component()
    {
        return (has_single_component<T>() && ...);
    }

    template<typename T> inline T *Entity::require_component()
    {
        if(this->has_component<T>())
        {
            return this->get_component<T>();
        }
        else
        {
            this->add_component<T>();
            return this->get_component<T>();
        }
    }

    template<typename T> inline T* Entity::add_component()
    {
        if(this->has_single_component<T>())
        {
            print_warn("Entity already has component " + std::string(typeid(T).name()) + "!");
            return nullptr;
        }
        T* component = new T();
        component->init(this);
        std::pair<const std::type_info *, Component *> pair(&typeid(T), component);
        this->components.insert(pair);
        if(typeid(T) == typeid(Transform))
        {
            this->transform = (Transform*)component;
        }
        return component;
    }
    template<typename... T> inline void Entity::add_components()
    {
        (add_component<T>(), ...);
    }

    template<typename T> inline void Component::register_component_with_name(std::string name)
    {
        Component::component_types[name] = []() -> Component* { return new T(); };
    }

    template<typename T> inline void System::register_system_with_name(std::string name)
    {
        System::system_types[name] = []() -> System* { return new T(); };
    }

    template<typename... T> GroupRef AutoGroup::create(const std::string& name)
    {
        AutoGroup* group = new AutoGroup(name);
        (group->add_component_type<T>(), ...);
        return group;
    }

    template <typename T> void AutoGroup::add_component_type()
    {
        component_types.push_back(&typeid(T));
    }

    template <typename T> void AutoGroup::remove_component_type()
    {
        auto it = std::find(component_types.begin(), component_types.end(), &typeid(T));
        if(it != component_types.end())
        {
            component_types.erase(it);
        }
    }
}