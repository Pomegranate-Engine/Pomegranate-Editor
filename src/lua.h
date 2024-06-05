#ifndef POMEGRANATE_ENGINE_LUA_H
#define POMEGRANATE_ENGINE_LUA_H
#include "Pomegranate/pomegranate.h"
#include "Pomegranate/ecs.h"
using namespace Pomegranate;

#include <lua.hpp>
#include <sol/sol.hpp>
#include <any>
#include"notifications.h"
#include<filesystem>

extern lua_State* lua_state;

class LuaComponentScript : public Resource
{
public:
    static std::unordered_map<std::string,LuaComponentScript*> lua_component_types;
    std::string script;
    std::string name;

    LuaComponentScript(std::string path);
    std::unordered_map<std::string, std::pair<const std::type_info*, void*>> run_script();
};

class LuaComponentData
{
public:
    LuaComponentScript* component;
    std::string name;
    std::unordered_map<std::string, std::pair<const std::type_info*, void*>> component_data;
    void init();
    template <typename T> void set(std::string name, void* value);
    template <typename T> T get(std::string name);
    LuaComponentData(LuaComponentScript* component);
};

class LuaComponent : public Component
{
public:
    std::vector<LuaComponentData*> scripts;
    void init(Entity *e) override;
    LuaComponentData* get_component(std::string name);
    LuaComponentData* add_component(std::string name);
    int lua_get_component(lua_State* L);
};

class LuaSystemScript : public Resource
{
public:
    std::string script;
    std::string name;

    LuaSystemScript(std::string path);
    ~LuaSystemScript();
    void run_script();
};

class LuaSystem : public System
{
private:
    LuaSystemScript* previous_script = nullptr;
public:

    LuaSystemScript* script;
    LuaSystem();
    void tick(Entity* e) override;
    void draw(Entity* e) override;
    void pre_tick() override;
    void post_tick() override;
    void pre_draw() override;
    void post_draw() override;
};

int luaL_openpomegranate(lua_State* L);

#include "lua.inl"
#endif //POMEGRANATE_ENGINE_LUA_H
