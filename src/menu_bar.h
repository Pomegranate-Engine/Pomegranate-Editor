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
#include "scene.h"
#include "json.hpp"
#include "editor.h"
#include "compile_and_run.h"
#include "imfilebrowser.h"
#include <algorithm>

using json = nlohmann::json;
using namespace Pomegranate;
void draw_menu_bar();

#endif //POMEGRANATE_ENGINE_MENU_BAR_H
