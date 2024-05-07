#ifndef POMEGRANATE_ENGINE_EDITOR_H
#define POMEGRANATE_ENGINE_EDITOR_H
#include "json.hpp"
#include "scene.h"
#include <vector>
#include <Pomegranate/ecs.h>

using namespace Pomegranate;
using json = nlohmann::json;

class Editor
{
public:
    static std::vector<json> history;
    static Group* current_scene;
    static int current_scene_index;
    static int max_history;
    static void action();
};


#endif //POMEGRANATE_ENGINE_EDITOR_H
