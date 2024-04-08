#ifndef POMEGRANATE_ENGINE_LUA_H
#define POMEGRANATE_ENGINE_LUA_H
#include "Pomegranate/pomegranate.h"
#include "Pomegranate/ecs.h"
using namespace Pomegranate;

#include <Lua/lua.hpp>
#include <any>

class LuaComponentScript : public Resource
{
public:
    std::string script;
    std::string name;
    lua_State* L;
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
int luaopen_pomegranate(lua_State* L);
int vec2_new(lua_State* L);
int vec2_add(lua_State* L);
int vec2_sub(lua_State* L);
int vec2_mul(lua_State* L);
int vec2_div(lua_State* L);
int vec2_normalized(lua_State* L);
int vec2_length(lua_State* L);
int vec2_dot(lua_State* L);
int vec2_lerp(lua_State* L);
int vec2_rotate(lua_State* L);
int vec2_distance_to(lua_State* L);
int vec2_angle_to(lua_State* L);
int vec2_tostring(lua_State* L);
int vec2_gc(lua_State* L);
int vec2_index(lua_State* L);
int vec2_set_index(lua_State* L);

int vec3_new(lua_State* L);
int vec3_add(lua_State* L);
int vec3_sub(lua_State* L);
int vec3_mul(lua_State* L);
int vec3_div(lua_State* L);
int vec3_normalized(lua_State* L);
int vec3_length(lua_State* L);
int vec3_dot(lua_State* L);
int vec3_cross(lua_State* L);
int vec3_lerp(lua_State* L);
int vec3_distance_to(lua_State* L);
int vec3_angle_to(lua_State* L);
int vec3_tostring(lua_State* L);
int vec3_gc(lua_State* L);
int vec3_index(lua_State* L);
int vec3_set_index(lua_State* L);

int color_new(lua_State* L);
int color_add(lua_State* L);
int color_sub(lua_State* L);
int color_mul(lua_State* L);
int color_div(lua_State* L);
int color_tostring(lua_State* L);
int color_gc(lua_State* L);
int color_index(lua_State* L);
int color_set_index(lua_State* L);

std::unordered_map<std::string, std::any> lua_checkcomponent(lua_State* L);

#include "lua.inl"
#endif //POMEGRANATE_ENGINE_LUA_H
