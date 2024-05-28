#ifndef POMEGRANATE_ENGINE_LUA_H
#define POMEGRANATE_ENGINE_LUA_H
#include "Pomegranate/pomegranate.h"
#include "Pomegranate/ecs.h"
using namespace Pomegranate;

#include <lua.hpp>
#include <sol/sol.hpp>
#include <any>
#include"notifications.h"

class LuaComponentScript : public Resource
{
public:
    std::string script;
    std::string name;
    std::unordered_map<std::string, std::any> component_data;
    LuaComponentScript(std::string path);
    void run_script();
    void set(std::string name, std::any value);
    template <typename T> T get(std::string name);
};

class LuaComponent : public Component
{
public:
    std::vector<LuaComponentScript*> scripts;
    void init(Entity *e) override;
    LuaComponentScript* get_component(std::string name);
};

class LuaSystemScript : public Resource
{
public:
    std::string script;
    std::string name;

    lua_State* L;

    LuaSystemScript(std::string path);
    ~LuaSystemScript();
    void run_script();
};

class LuaSystem : public System
{
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
