#ifndef POMEGRANATE_LIBRARY_WINDOWS_MANAGER_H
#define POMEGRANATE_LIBRARY_WINDOWS_MANAGER_H
#include <vector>
#include "editor_window.h"

class WindowsManager
{
public:
    std::vector<EditorWindow*> windows;
    void open_window(EditorWindow* window);
    void close_window(EditorWindow* window);
    void render();
    void update();
};


#endif //POMEGRANATE_LIBRARY_WINDOWS_MANAGER_H
