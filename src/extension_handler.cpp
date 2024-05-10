#include "extension_handler.h"
//dylib loading header
#ifdef __APPLE__
#include <dlfcn.h>
#endif
#include <iostream>
#include <string>

ExtensionHandler::ExtensionHandler()
{
    this->extensions = std::vector<Extension*>();
}

void ExtensionHandler::load(const char *path)
{
#ifdef __APPLE__
    void* handle = dlopen(path, RTLD_LAZY);
    if (!handle)
    {
        std::cerr << "Cannot open library: " << dlerror() << std::endl;
        return;
    }
    //Call the main function
    void (*main_func)() = (void (*)())dlsym(handle, "main");
    if (!main_func)
    {
        std::cerr << "Cannot load symbol main: " << dlerror() << std::endl;
        return;
    }
    main_func();
    Extension* extension = new Extension();
    extension->path = path;
    this->extensions.push_back(extension);
#endif
}

void ExtensionHandler::unload(const char *path)
{
    for (int i = 0; i < this->extensions.size(); i++)
    {
        if (this->extensions[i]->path == path)
        {
            this->extensions.erase(this->extensions.begin() + i);
            break;
        }
    }
}
