#ifndef POMEGRANATE_ENGINE_MENU_BAR_H
#define POMEGRANATE_ENGINE_MENU_BAR_H
#include "imgui.h"
#include "Pomegranate/pomegranate.h"
#include "Pomegranate/ecs.h"
#include <ostream>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
using namespace Pomegranate;

std::string save_scene(EntityGroup* group);
void draw_menu_bar(EntityGroup* group);

#endif //POMEGRANATE_ENGINE_MENU_BAR_H
