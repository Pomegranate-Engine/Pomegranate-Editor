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
    static std::string current_scene_path;
    static GroupRef current_scene;
    static int current_scene_index;
    static int max_history;
    static std::string cmake_generator;
    static void action();
};


#endif //POMEGRANATE_ENGINE_EDITOR_H
