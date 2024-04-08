#ifndef POMEGRANATE_ENGINE_HOTKEY_MANAGER_H
#define POMEGRANATE_ENGINE_HOTKEY_MANAGER_H
#include "Pomegranate/pomegranate.h"
#include <vector>
#include<unordered_map>
#include<functional>
using namespace Pomegranate;
class Hotkey
{
public:
    std::vector<SDL_Scancode> keys;
    std::unordered_map<std::string,std::function<void()>> actions;
    bool pressed;
    Hotkey(std::vector<SDL_Scancode> keys,const std::string& name, const std::function<void()>& action);
    void add_action(const std::string& name,const std::function<void()>& action);
    std::string get_key_string();
};

class HotkeyManager
{
    static std::unordered_map<std::string,Hotkey> hotkeys;
public:
    static void add_hotkey(Hotkey hotkey);
    static void disable_hotkey(const std::string& key);
    static void tick();
};


#endif //POMEGRANATE_ENGINE_HOTKEY_MANAGER_H
