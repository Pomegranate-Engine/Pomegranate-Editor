#ifndef POMEGRANATE_ENGINE_COMPILE_AND_RUN_H
#define POMEGRANATE_ENGINE_COMPILE_AND_RUN_H

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include "editor.h"

void compile_project(const std::string& project_name,const std::string& base_scene);
void run_project();

#endif //POMEGRANATE_ENGINE_COMPILE_AND_RUN_H
