#ifndef POMEGRANATE_ENGINE_EXTENSION_HANDLER_H
#define POMEGRANATE_ENGINE_EXTENSION_HANDLER_H
#include <vector>


class Extension
{
public:
    const char* path;
};

class ExtensionHandler
{
    std::vector<Extension*> extensions;
public:
    ExtensionHandler();
    void load(const char* path);
    void unload(const char* path);
};


#endif
