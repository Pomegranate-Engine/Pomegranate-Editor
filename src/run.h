#ifndef POMEGRANATE_LIVESHARESERVER_RUN_H
#define POMEGRANATE_LIVESHARESERVER_RUN_H
#include <Pomegranate/pomegranate.h>
#include "editor.h"
using namespace Pomegranate;

class ProjectDebugger
{
public:
    static std::vector<ProjectDebugger*> debuggers;
    SDL_Texture* texture;
    ProjectDebugger();

    void tick();

    static void tick_debuggers();
};


#endif //POMEGRANATE_LIVESHARESERVER_RUN_H
