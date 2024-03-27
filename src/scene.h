#ifndef SCENE_H
#define SCENE_H
#include <Pomegranate/pomegranate.h>
#include <Pomegranate/ecs.h>
#include<fstream>
#include"json.hpp"

using namespace Pomegranate;
using json = nlohmann::json;
inline extern EntityGroup* currently_opened_scene = nullptr;
void save_scene(const char* path, EntityGroup* scene);
EntityGroup* open_scene(const char* path);

#endif //SCENE_H
