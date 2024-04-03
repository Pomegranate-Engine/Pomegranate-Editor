#include "editor.h"

std::vector<json> Editor::history = {};
EntityGroup* Editor::current_scene = nullptr;
std::string Editor::current_scene_path = "";
int Editor::current_scene_index = 0;
int Editor::max_history = 256;
#ifdef _WIN32
std::string Editor::cmake_generator = "Visual Studio 16 2019";
#else
std::string Editor::cmake_generator = "Unix Makefiles";
#endif

void Editor::action()
{
    // Clear redo history
    if(history.size() > current_scene_index+1)
    {
        history.erase(history.begin()+current_scene_index+1,history.end());
    }
    // Action
    history.push_back(save_scene_as_json(current_scene));
    //Dump history to a file in engine/ folder history.txt
    std::ofstream file("engine/history.txt");
    for(auto& j : history)
    {
        file << j.dump(4) << std::endl;
    }
    file.close();
    current_scene_index++;

    if(current_scene_index > max_history)
    {
        history.erase(history.begin());
        current_scene_index--;
    }
}
