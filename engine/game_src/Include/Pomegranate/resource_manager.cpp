#include "resource_manager.h"
namespace Pomegranate
{
	std::map<std::string, void*> ResourceManager::resources = std::map<std::string, void*>();
}