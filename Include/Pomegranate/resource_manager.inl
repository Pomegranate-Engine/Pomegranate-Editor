namespace Pomegranate
{
    ResourceRef::ResourceRef()
    {
        resource = nullptr;
    }

    ResourceRef::ResourceRef(Resource* resource)
    {
        this->resource = resource;
    }

    ResourceRef::~ResourceRef()
    {
        //Find the resource in the resources map and delete it
        for(auto& r : ResourceManager::resources)
        {
            if(r.second == resource)
            {
                ResourceManager::resources.erase(r.first);
                break;
            }
        }
    }

    template <typename T> T* ResourceRef::operator->()
    {
        return (T*)resource;
    }

    template <typename T> T*  ResourceRef::operator*()
    {
        return (T*)resource;
    }

    template <typename T> T* ResourceRef::get()
    {
        return (T*)resource;
    }

    bool ResourceRef::operator==(const ResourceRef& other)
    {
        return resource == other.resource;
    }

    bool ResourceRef::operator!=(const ResourceRef& other)
    {
        return resource != other.resource;
    }

    ResourceRef::operator bool()
    {
        return resource!=nullptr;
    }

    void ResourceRef::destroy(Resource* resource)
    {
        //Nullify all references to the resource
        for(auto& r : ResourceManager::resources)
        {
            if(r.second == resource)
            {
                ResourceManager::resources.erase(r.first);
            }
        }
    }

    void ResourceRef::destroy(ResourceRef resource)
    {
        //Nullify all references to the resource
        for(auto& r : ResourceManager::resources)
        {
            if(r.second == resource.resource)
            {
                ResourceManager::resources.erase(r.first);
            }
        }
    }



    template <typename T> ResourceRef ResourceManager::load(const std::string& path)
    {
        if(resources.find(path)!=resources.end())
        {
            return (T*)resources[path];
        }
        else
        {
            auto* r = new T(path.c_str());
            ((Resource*)r)->path = path;
            resources[path] = r;
            return r;
        }
        return nullptr;
    }

    template <typename T> ResourceRef ResourceManager::reload(const std::string& path)
    {
        if(resources.find(path)!=resources.end())
        {
            delete (T*)resources[path];
            resources.erase(path);
        }
        return load<T>(path);
    }

    template <typename T> void ResourceManager::unload(const std::string& path)
    {
        ResourceRef::destroy((Resource*)resources[path]);
        delete (T)resources[path];
        resources.erase(path);
    }
}