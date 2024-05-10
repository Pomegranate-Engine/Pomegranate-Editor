#ifndef SCENE_H
#define SCENE_H
#include <Pomegranate/pomegranate.h>
#include <Pomegranate/ecs.h>
#include<fstream>
#include"json.hpp"
#include"editor.h"
#include"lua.h"

using namespace Pomegranate;
using json = nlohmann::json;
void unload_all();
json save_scene_as_json(Group* scene);
void save_scene(const char* path, Group* scene);
Group* open_scene_from_json(json j);
Group* open_scene(const char* path);

#endif //SCENE_H
