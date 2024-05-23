#include "resource_manager.h"
namespace Pomegranate
{
	std::map<std::string, void*> ResourceManager::resources = std::map<std::string, void*>();

    bool ResourceManager::exists(const std::string& path)
    {
        //Check if the resource exists at specified path in file system
        return std::filesystem::exists(path);
    }
}