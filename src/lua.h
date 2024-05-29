#ifndef POMEGRANATE_ENGINE_LUA_H
#define POMEGRANATE_ENGINE_LUA_H
#include "Pomegranate/pomegranate.h"
#include "Pomegranate/ecs.h"
using namespace Pomegranate;

#include <lua.hpp>
#include <sol/sol.hpp>
#include <any>
#include"notifications.h"

extern lua_State* lua_state;

class LuaComponentScript : public Resource
{
public:
    std::string script;
    std::string name;

    std::unordered_map<std::string, std::pair<const std::type_info*, void*>> component_data;
    LuaComponentScript(std::string path);
    void run_script();
    template <typename T> void set(std::string name, void* value);
    template <typename T> T get(std::string name);
};

class LuaComponent : public Component
{
public:
    std::vector<LuaComponentScript*> scripts;
    void init(Entity *e) override;
    LuaComponentScript* get_component(std::string name);
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
