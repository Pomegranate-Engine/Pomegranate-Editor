#include "editor.h"

std::vector<json> Editor::history = {};
EntityGroup* Editor::current_scene = nullptr;
int Editor::current_scene_index = 0;
int Editor::max_history = 256;

void Editor::action()
{
    // Clear redo history
    if(history.size() > current_scene_index+1)
    {
        history.erase(history.begin()+current_scene_index+1,history.end());
    }
    // Action
    history.push_back(save_scene_as_json(current_scene));
    //Dump history to a file in engine_res/ folder history.txt
    std::ofstream file("engine_res/history.txt");
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
