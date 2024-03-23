#ifndef POMEGRANATE_LIBRARY_EDITOR_WINDOW_H
#define POMEGRANATE_LIBRARY_EDITOR_WINDOW_H
#include "imgui.h"
#include "Pomegranate/pomegranate.h"

class EditorWindow
{
public:
    bool open = true;
    int flags = 0;
    Pomegranate::Vec2i position;
    Pomegranate::Vec2i size;
    bool docked = false;
    std::string name;
    void begin();
    virtual void render()=0;
    void end();
    virtual void update()=0;

};


#endif //POMEGRANATE_LIBRARY_EDITOR_WINDOW_H
