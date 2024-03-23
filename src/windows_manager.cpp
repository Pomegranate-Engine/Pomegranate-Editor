#include "windows_manager.h"

void WindowsManager::open_window(EditorWindow *window)
{
    this->windows.push_back(window);
}

void WindowsManager::close_window(EditorWindow *window)
{
    for (int i = 0; i < this->windows.size(); i++)
    {
        if (this->windows[i] == window)
        {
            this->windows.erase(this->windows.begin() + i);
            break;
        }
    }
}

void WindowsManager::render()
{
    for (auto & window : this->windows)
    {
        window->begin();
        window->render();
        window->end();
    }
}

void WindowsManager::update()
{
    for (auto & window : this->windows)
    {
        window->update();
    }
}
