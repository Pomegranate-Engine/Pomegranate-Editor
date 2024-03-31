#ifndef POMEGRANATE_LIBRARY_EDITOR_WINDOW_H
#define POMEGRANATE_LIBRARY_EDITOR_WINDOW_H
#include "imgui.h"
#include "Pomegranate/pomegranate.h"

using namespace Pomegranate;

enum DockSlot
{
    DOCK_SLOT_LEFT,
    DOCK_SLOT_RIGHT,
    DOCK_SLOT_BOTTOM,
    DOCK_SLOT_CENTER,
    DOCK_SLOT_FLOATING
};

class EditorWindow
{
public:
    DockSlot dock_slot = DOCK_SLOT_FLOATING;
    bool mouse_released = false;
    bool open = true;
    int flags = 0;
    Pomegranate::Vec2i position;
    Pomegranate::Vec2i size;
    std::string name;
    void begin();
    virtual void render()=0;
    void end();
    virtual void update()=0;

};


#endif //POMEGRANATE_LIBRARY_EDITOR_WINDOW_H
