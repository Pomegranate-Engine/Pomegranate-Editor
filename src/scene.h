#ifndef SCENE_H
#define SCENE_H
#include <Pomegranate/pomegranate.h>
#include <Pomegranate/ecs.h>
#include <Pomegranate/standard_ecs.h>
#include <Pomegranate/standard_ecs_physics.h>
#include <Pomegranate/standard_ecs_rendering.h>
#include<fstream>
#include"json.hpp"
#include"editor.h"
#include "lua.h"

using namespace Pomegranate;
using json = nlohmann::json;

class SceneGroup : public Group
{
public:
    std::string path;
    SceneGroup(std::string name, std::string path);
    void instantiate();
};

void unload_all();
json save_scene_as_json(GroupRef scene);
void save_scene(const char* path, GroupRef scene);
GroupRef open_scene_from_json(json j);
GroupRef open_scene(const char* path);
GroupRef create_default_scene();

#endif //SCENE_H
