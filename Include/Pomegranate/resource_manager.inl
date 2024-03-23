namespace Pomegranate
{
    template <typename T> T* ResourceManager::load(const std::string& path)
    {
        if(resources.find(path)!=resources.end())
        {
            return (T*)resources[path];
        }
        else
        {
            auto* r = new T(path.c_str());
            resources[path] = r;
            return r;
        }
        return nullptr;
    }

    template <typename T> void ResourceManager::unload(const std::string& path)
    {
        delete (T)resources[path];
        resources.erase(path);
    }
}