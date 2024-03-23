#ifndef POMEGRANATE_ENGINE_RESOURCE_MANAGER_H
#define POMEGRANATE_ENGINE_RESOURCE_MANAGER_H
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <map>
#include "vec_math.h"
#include "window.h"
namespace Pomegranate
{
    struct Resource
    {
        std::string path;
    };

    class ResourceManager
    {
    private:
    public:
        static std::map<std::string,void*> resources;
        template<typename T> static T* load(const std::string& path);
        template<typename T> static void unload(const std::string& path); //Unsafe
    };
}

#include "resource_manager.inl"
#endif //POMEGRANATE_ENGINE_RESOURCE_MANAGER_H
