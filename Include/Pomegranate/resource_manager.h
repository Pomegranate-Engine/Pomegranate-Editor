#ifndef POMEGRANATE_ENGINE_RESOURCE_MANAGER_H
#define POMEGRANATE_ENGINE_RESOURCE_MANAGER_H
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <map>
#include <filesystem>
#include "vec_math.h"
#include "window.h"
namespace Pomegranate
{
    struct Resource
    {
    public:
        std::string path;
    };

    class ResourceRef
    {
    private:
        static std::map<std::string, void*> resources_refs;
    public:
        Resource* resource;
        ResourceRef();
        ResourceRef(Resource* resource);
        ~ResourceRef();
        template<typename T> T* operator->();
        template<typename T> T* operator*();
        template<typename T> T* get();
        bool operator==(const ResourceRef& other);
        bool operator!=(const ResourceRef& other);
        explicit operator bool();
        static void destroy(Resource* resource);
        static void destroy(ResourceRef resource);
    };

    class ResourceManager
    {
    private:
    public:
        static std::map<std::string,void*> resources;
        template<typename T> static ResourceRef load(const std::string& path);
        template<typename T> static ResourceRef reload(const std::string& path);
        template<typename T> static void unload(const std::string& path); //Unsafe
        static bool exists(const std::string& path);
    };
}

#include "resource_manager.inl"
#endif //POMEGRANATE_ENGINE_RESOURCE_MANAGER_H
