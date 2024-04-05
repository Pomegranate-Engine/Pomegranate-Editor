#include "hotkey_manager.h"

#include <utility>

std::unordered_map<std::string,Hotkey> HotkeyManager::hotkeys;

Hotkey::Hotkey(std::vector<SDL_Scancode> keys, const std::string& name, const std::function<void()>& action)
{
    this->keys = std::move(keys);
    this->actions.emplace(name,action);
    this->pressed = false;
}

void Hotkey::add_action(const std::string& name, const std::function<void()>& action)
{
    if(actions.find(name) != actions.end())
        actions[name] = action;
    else
        actions.emplace(name,action);
}

std::string Hotkey::get_key_string()
{
    std::string key_string;
    for (auto& key : keys)
    {
        key_string += SDL_GetKeyName(key);
        key_string += " + ";
    }
    key_string.pop_back();
    key_string.pop_back();
    return key_string;
}

void HotkeyManager::add_hotkey(Hotkey hotkey)
{
    if(hotkeys.find(hotkey.get_key_string()) == hotkeys.end())
        hotkeys.emplace(hotkey.get_key_string(),hotkey);
    else
    {
        for (auto& action : hotkey.actions)
        {
            hotkeys.at(hotkey.get_key_string()).add_action(action.first,action.second);
        }
    }
}

void HotkeyManager::disable_hotkey(const std::string& key)
{
    //Check if action exists in a hotkey
    for (auto& hotkey : hotkeys)
    {
        if(hotkey.second.actions.find(key) != hotkey.second.actions.end())
        {
            hotkey.second.actions.erase(key);
        }
    }
}

void HotkeyManager::tick()
{
    for (auto& hotkey : hotkeys)
    {
        bool pressed = true;
        for (auto& key : hotkey.second.keys)
        {
            if (!InputManager::get_key(key))
            {
                pressed = false;
                break;
            }
        }
        if (pressed)
        {
            if(!hotkey.second.pressed) {
                print_info("Hotkey pressed: " + hotkey.first);

                for (auto &action: hotkey.second.actions) {
                    print_info("Action: " + action.first);
                    action.second();
                }
            }
            hotkey.second.pressed = true;
        }
        else
        {
            hotkey.second.pressed = false;
        }
    }
}
