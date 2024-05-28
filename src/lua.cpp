#include "lua.h"

LuaComponentScript::LuaComponentScript(std::string path)
{
    print_info("Loading Lua script: " + path);
    this->path = path;
}

void LuaComponentScript::run_script()
{

}

void LuaComponentScript::set(std::string name, std::any value)
{
    component_data[name] = value;
}

void LuaComponent::init(Entity *e)
{
    register_component(LuaComponent);
    push_data<std::vector<LuaComponentScript*>>("scripts", &scripts);
}

LuaComponentScript* LuaComponent::get_component(std::string name)
{
    for(auto script : scripts)
    {
        if(script->name == name)
        {
            return script;
        }
    }
    return nullptr;
}

//Lua systems

LuaSystemScript::LuaSystemScript(std::string path)
{
    print_info("Loading Lua script: " + path);
    this->path = path;
    run_script();
}

LuaSystemScript::~LuaSystemScript()
{

}

void LuaSystemScript::run_script()
{
    L = luaL_newstate();
    luaL_openlibs(L);
    luaL_openpomegranate(L);
    luaL_requiref(L, "pomegranate", luaL_openpomegranate, 1);
    lua_pop(L, 1);
    luaL_dofile(L, path.c_str());

    //Check errors
    if(lua_isstring(L, -1))
    {
        print_error(lua_tostring(L, -1));
    }
}

void LuaSystem::tick(Entity *e)
{

}

void LuaSystem::draw(Entity *e)
{

}

void LuaSystem::pre_tick()
{

}

void LuaSystem::post_tick() {

}

void LuaSystem::pre_draw() {

}

void LuaSystem::post_draw() {

}

LuaSystem::LuaSystem()
{
    script = ResourceManager::load<LuaSystemScript>("res/PlayerController.lua");
    script->run_script();
}

#pragma region debug
//Dubug functions register

static int lua_print_info(lua_State *L)
{
    print_info(lua_tostring(L, 1));
    return 0;
}

static int lua_print_error(lua_State *L)
{
    print_error(lua_tostring(L, 1));
    return 0;
}

static int lua_print_warn(lua_State *L)
{
    print_warn(lua_tostring(L, 1));
    return 0;
}

static int lua_print_debug(lua_State *L)
{
    print_debug(lua_tostring(L, 1));
    return 0;
}

static int lua_print_pass(lua_State *L)
{
    print_pass(lua_tostring(L, 1));
    return 0;
}

static int lua_print_fail(lua_State *L)
{
    print_fail(lua_tostring(L, 1));
    return 0;
}

#pragma endregion

#pragma region editor

static int lua_notify_pass(lua_State *L)
{
    const char* title = lua_tostring(L, 1);
    const char* message = lua_tostring(L, 2);
    Notify::notify({ResourceManager::load<Texture>("engine/check.png"),EditorTheme::color_palette_green,title, message});
    return 0;
}

static int lua_notify_error(lua_State *L)
{
    const char* title = lua_tostring(L, 1);
    const char* message = lua_tostring(L, 2);
    Notify::notify({ResourceManager::load<Texture>("engine/error.png"),EditorTheme::color_palette_red,title, message});
    return 0;
}

static int lua_notify_warn(lua_State *L)
{
    const char* title = lua_tostring(L, 1);
    const char* message = lua_tostring(L, 2);
    Notify::notify({ResourceManager::load<Texture>("engine/warning.png"),EditorTheme::color_palette_yellow,title, message});
    return 0;
}

#pragma endregion
//Vec2

#pragma region math

static int lua_vec2_index(lua_State* L);
static int lua_vec2_set_index(lua_State* L);
static int lua_vec2_add(lua_State* L);
static int lua_vec2_sub(lua_State* L);
static int lua_vec2_mul(lua_State* L);
static int lua_vec2_div(lua_State* L);

static int lua_vec2_new(lua_State* L)
{
    float x = lua_tonumber(L, 1);
    float y = lua_tonumber(L, 2);
    Vec2* vec = new Vec2(x, y);
    lua_pushlightuserdata(L, vec);

    lua_newtable(L);
    lua_pushcfunction(L, lua_vec2_index);
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, lua_vec2_set_index);
    lua_setfield(L, -2, "__newindex");

    lua_pushcfunction(L, lua_vec2_add);
    lua_setfield(L, -2, "__add");

    lua_pushcfunction(L, lua_vec2_sub);
    lua_setfield(L, -2, "__sub");

    lua_pushcfunction(L, lua_vec2_mul);
    lua_setfield(L, -2, "__mul");

    lua_pushcfunction(L, lua_vec2_div);
    lua_setfield(L, -2, "__div");

    lua_setmetatable(L, -2);
    return 1;
}

//Index
static int lua_vec2_index(lua_State* L)
{
    Vec2* vec = (Vec2*)lua_touserdata(L, 1);
    const char* key = lua_tostring(L, 2);
    if(strcmp(key, "x") == 0)
    {
        lua_pushnumber(L, vec->x);
        return 1;
    }
    if(strcmp(key, "y") == 0)
    {
        lua_pushnumber(L, vec->y);
        return 1;
    }
    return 0;
}

//Set index
static int lua_vec2_set_index(lua_State* L)
{
    Vec2* vec = (Vec2*)lua_touserdata(L, 1);
    const char* key = lua_tostring(L, 2);
    if(strcmp(key, "x") == 0)
    {
        vec->x = lua_tonumber(L, 3);
        return 0;
    }
    if(strcmp(key, "y") == 0)
    {
        vec->y = lua_tonumber(L, 3);
        return 0;
    }
    return 0;
}

//Add
static int lua_vec2_add(lua_State* L)
{
    Vec2* vec1 = (Vec2*)lua_touserdata(L, 1);
    Vec2* vec2 = (Vec2*)lua_touserdata(L, 2);
    Vec2* vec = new Vec2(vec1->x + vec2->x, vec1->y + vec2->y);
    lua_pushlightuserdata(L, vec);
    return 1;
}

//Sub
static int lua_vec2_sub(lua_State* L)
{
    Vec2* vec1 = (Vec2*)lua_touserdata(L, 1);
    Vec2* vec2 = (Vec2*)lua_touserdata(L, 2);
    Vec2* vec = new Vec2(vec1->x - vec2->x, vec1->y - vec2->y);
    lua_pushlightuserdata(L, vec);
    return 1;
}

//Mul
static int lua_vec2_mul(lua_State* L)
{
    Vec2* vec1 = (Vec2*)lua_touserdata(L, 1);
    //Check if the second argument is a number
    if(lua_isnumber(L, 2))
    {
        float scalar = lua_tonumber(L, 2);
        Vec2* vec = new Vec2(vec1->x * scalar, vec1->y * scalar);
        lua_pushlightuserdata(L, vec);
        return 1;
    }
    else
    {
        Vec2* vec2 = (Vec2*)lua_touserdata(L, 2);
        Vec2* vec = new Vec2(vec1->x * vec2->x, vec1->y * vec2->y);
        lua_pushlightuserdata(L, vec);
        return 1;
    }
}

//Div
static int lua_vec2_div(lua_State* L)
{
    Vec2* vec1 = (Vec2*)lua_touserdata(L, 1);
    //Check if the second argument is a number
    if(lua_isnumber(L, 2))
    {
        float scalar = lua_tonumber(L, 2);
        Vec2* vec = new Vec2(vec1->x / scalar, vec1->y / scalar);
        lua_pushlightuserdata(L, vec);
        return 1;
    }
    else
    {
        Vec2* vec2 = (Vec2*)lua_touserdata(L, 2);
        Vec2* vec = new Vec2(vec1->x / vec2->x, vec1->y / vec2->y);
        lua_pushlightuserdata(L, vec);
        return 1;
    }
}

//Vec3
static int lua_vec3_index(lua_State* L);
static int lua_vec3_set_index(lua_State* L);
static int lua_vec3_add(lua_State* L);
static int lua_vec3_sub(lua_State* L);
static int lua_vec3_mul(lua_State* L);
static int lua_vec3_div(lua_State* L);

static int lua_vec3_new(lua_State* L)
{
    float x = lua_tonumber(L, 1);
    float y = lua_tonumber(L, 2);
    float z = lua_tonumber(L, 3);
    Vec3* vec = new Vec3(x, y, z);
    lua_pushlightuserdata(L, vec);

    lua_newtable(L);
    lua_pushcfunction(L, lua_vec3_index);
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, lua_vec3_set_index);
    lua_setfield(L, -2, "__newindex");

    lua_pushcfunction(L, lua_vec3_add);
    lua_setfield(L, -2, "__add");

    lua_pushcfunction(L, lua_vec3_sub);
    lua_setfield(L, -2, "__sub");

    lua_pushcfunction(L, lua_vec3_mul);
    lua_setfield(L, -2, "__mul");

    lua_pushcfunction(L, lua_vec3_div);
    lua_setfield(L, -2, "__div");

    lua_setmetatable(L, -2);
    return 1;
}

//Index
static int lua_vec3_index(lua_State* L)
{
    Vec3* vec = (Vec3*)lua_touserdata(L, 1);
    const char* key = lua_tostring(L, 2);
    if(strcmp(key, "x") == 0)
    {
        lua_pushnumber(L, vec->x);
        return 1;
    }
    if(strcmp(key, "y") == 0)
    {
        lua_pushnumber(L, vec->y);
        return 1;
    }
    if(strcmp(key, "z") == 0)
    {
        lua_pushnumber(L, vec->z);
        return 1;
    }
    return 0;
}

//Set index
static int lua_vec3_set_index(lua_State* L)
{
    Vec3* vec = (Vec3*)lua_touserdata(L, 1);
    const char* key = lua_tostring(L, 2);
    if(strcmp(key, "x") == 0)
    {
        vec->x = lua_tonumber(L, 3);
        return 0;
    }
    if(strcmp(key, "y") == 0)
    {
        vec->y = lua_tonumber(L, 3);
        return 0;
    }
    if(strcmp(key, "z") == 0)
    {
        vec->z = lua_tonumber(L, 3);
        return 0;
    }
    return 0;
}

//Add
static int lua_vec3_add(lua_State* L)
{
    Vec3* vec1 = (Vec3*)lua_touserdata(L, 1);
    Vec3* vec2 = (Vec3*)lua_touserdata(L, 2);
    Vec3* vec = new Vec3(vec1->x + vec2->x, vec1->y + vec2->y, vec1->z + vec2->z);
    lua_pushlightuserdata(L, vec);
    return 1;
}

//Sub
static int lua_vec3_sub(lua_State* L)
{
    Vec3* vec1 = (Vec3*)lua_touserdata(L, 1);
    Vec3* vec2 = (Vec3*)lua_touserdata(L, 2);
    Vec3* vec = new Vec3(vec1->x - vec2->x, vec1->y - vec2->y, vec1->z - vec2->z);
    lua_pushlightuserdata(L, vec);
    return 1;
}

//Mul
static int lua_vec3_mul(lua_State* L)
{
    Vec3* vec1 = (Vec3*)lua_touserdata(L, 1);
    //Check if the second argument is a number
    if(lua_isnumber(L, 2))
    {
        float scalar = lua_tonumber(L, 2);
        Vec3* vec = new Vec3(vec1->x * scalar, vec1->y * scalar, vec1->z * scalar);
        lua_pushlightuserdata(L, vec);
        return 1;
    }
    else
    {
        Vec3* vec2 = (Vec3*)lua_touserdata(L, 2);
        Vec3* vec = new Vec3(vec1->x * vec2->x, vec1->y * vec2->y, vec1->z * vec2->z);
        lua_pushlightuserdata(L, vec);
        return 1;
    }
}

//Div
static int lua_vec3_div(lua_State* L)
{
    Vec3* vec1 = (Vec3*)lua_touserdata(L, 1);
    //Check if the second argument is a number
    if(lua_isnumber(L, 2))
    {
        float scalar = lua_tonumber(L, 2);
        Vec3* vec = new Vec3(vec1->x / scalar, vec1->y / scalar, vec1->z / scalar);
        lua_pushlightuserdata(L, vec);
        return 1;
    }
    else
    {
        Vec3* vec2 = (Vec3*)lua_touserdata(L, 2);
        Vec3* vec = new Vec3(vec1->x / vec2->x, vec1->y / vec2->y, vec1->z / vec2->z);
        lua_pushlightuserdata(L, vec);
        return 1;
    }
}

#pragma endregion

#pragma region ecs

static int lua_entity_index(lua_State* L);
static int lua_entity_set_index(lua_State* L);

static int lua_entity_new(lua_State* L)
{
    const char* name = lua_tostring(L, 1);
    Entity* e = new Entity();
    e->name = name;
    lua_pushlightuserdata(L, e);

    lua_newtable(L);
    lua_pushcfunction(L, lua_entity_index);
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, lua_entity_set_index);
    lua_setfield(L, -2, "__newindex");

    lua_setmetatable(L, -2);
    return 1;
}

static int lua_entity_index(lua_State* L)
{
    Entity* e = (Entity*)lua_touserdata(L, 1);
    const char* key = lua_tostring(L, 2);
    if(strcmp(key, "name") == 0)
    {
        lua_pushstring(L, e->name.c_str());
        return 1;
    }
    else if(strcmp(key, "id") == 0)
    {
        lua_pushnumber(L, e->get_id());
        return 1;
    }
    return 0;
}

static int lua_entity_set_index(lua_State* L)
{
    Entity* e = (Entity*)lua_touserdata(L, 1);
    const char* key = lua_tostring(L, 2);
    if(strcmp(key, "name") == 0)
    {
        e->name = lua_tostring(L, 3);
        return 0;
    }
    else if(strcmp(key, "id"))
    {
        e->set_id(lua_tonumber(L, 3));
        return 0;
    }
    return 0;
}

static int lua_group_set_index(lua_State* L)
{
    Group* g = (Group*)lua_touserdata(L, 1);
    const char* key = lua_tostring(L, 2);

    if(strcmp(key, "name") == 0)
    {
        g->name = lua_tostring(L, 3);
        return 0;
    }
    else if(strcmp(key, "id") == 0)
    {
        g->set_id(lua_tonumber(L, 3));
        return 0;
    }
    return 0;
}

static int lua_group_add_entity(lua_State* L)
{
    Group* g = (Group*)lua_touserdata(L, 1);
    Entity* e = (Entity*)lua_touserdata(L, 2);
    g->add_entity(e);

    return 0;
}

static int lua_group_add_group(lua_State* L)
{
    Group* g = (Group*)lua_touserdata(L, 1);
    Group* g2 = (Group*)lua_touserdata(L, 2);
    g->add_group(g2);

    return 0;
}

static int lua_group_add_system(lua_State* L)
{
    Group* g = (Group*)lua_touserdata(L, 1);
    System* s = (System*)lua_touserdata(L, 2);
    g->add_system(s);

    return 0;
}

static int lua_group_index(lua_State* L)
{
    // Ensure the first argument is a userdata (Group*)
    Group* g = static_cast<Group*>(luaL_checkudata(L, 1, "Group"));
    if (!g) {
        luaL_error(L, "Expected a Group userdata");
        return 0; // Although luaL_error doesn't return, this is good practice
    }

    // Ensure the second argument is a string (key)
    const char* key = luaL_checkstring(L, 2);
    if (!key) {
        luaL_error(L, "Expected a string as key");
        return 0; // Again, good practice
    }

    if (strcmp(key, "add_system") == 0)
    {
        lua_pushcfunction(L, lua_group_add_system);
        return 1;
    }
    else if (strcmp(key, "add_entity") == 0)
    {
        lua_pushcfunction(L,lua_group_add_entity);
        return 1;
    }
    else if (strcmp(key, "add_group") == 0)
    {
        lua_pushcfunction(L, lua_group_add_group);
        return 1;
    }
    else if (strcmp(key, "name") == 0)
    {
        lua_pushstring(L, g->name.c_str());
        return 1;
    }
    else if (strcmp(key, "id") == 0)
    {
        lua_pushnumber(L, g->id);
        return 1;
    }

    return 0;
}

static int lua_find_group(lua_State* L)
{
    const char* name = lua_tostring(L, 1);
    GroupRef g = Group::get_group(name);
    Group* group = g.get();

    if(group == nullptr)
    {
        lua_pushnil(L);
        return 1;
    }

    lua_pushlightuserdata(L, group);

    lua_newtable(L);
    lua_pushcfunction(L, lua_group_index);
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, lua_group_set_index);
    lua_setfield(L, -2, "__newindex");

    lua_setmetatable(L, -2);

    return 1;
}

static int lua_group_new(lua_State* L)
{
    const char* name = lua_tostring(L, 1);
    Group* g = new Group(name);
    lua_pushlightuserdata(L, g);

    lua_newtable(L);
    lua_pushcfunction(L, lua_group_index);
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, lua_group_set_index);
    lua_setfield(L, -2, "__newindex");

    lua_setmetatable(L, -2);
    return 1;
}


#pragma endregion


luaL_Reg lua_pomegranate_debug[] = {
        {"print_info", lua_print_info},
        {"print_error", lua_print_error},
        {"print_warn", lua_print_warn},
        {"print_debug", lua_print_debug},
        {"print_pass", lua_print_pass},
        {"print_fail", lua_print_fail},
        {nullptr, nullptr}
};

luaL_Reg lua_pomegranate_editor[] = {
        {"notify_pass", lua_notify_pass},
        {"notify_error", lua_notify_error},
        {"notify_warn", lua_notify_warn},
        {nullptr, nullptr}
};

luaL_Reg lua_pomegranate_math[] = {
        {"Vec2", lua_vec2_new},
        {"Vec3", lua_vec3_new},
        {nullptr, nullptr}
};

luaL_Reg lua_pomegranate_ecs[] = {
        {"Entity", lua_entity_new},
        {"Group", lua_group_new},
        {"find_group", lua_find_group},
        {"group_add_entity", lua_group_add_entity},
        {"group_add_group", lua_group_add_group},
        {"group_add_system", lua_group_add_system},
        {nullptr, nullptr}
};

static int luaL_openpomegranate(lua_State *L)
{
    lua_newtable(L);

    // Create the debug subtable
    lua_newtable(L);
    luaL_setfuncs(L, lua_pomegranate_debug, 0);

    // Set the debug subtable into the pomegranate table
    lua_setfield(L, -2, "debug");

    // Create the editor subtable
    lua_newtable(L);
    luaL_setfuncs(L, lua_pomegranate_editor, 0);

    // Set the editor subtable into the pomegranate table
    lua_setfield(L, -2, "editor");

    // Create the math subtable
    lua_newtable(L);
    luaL_setfuncs(L, lua_pomegranate_math, 0);

    // Set the math subtable into the pomegranate table
    lua_setfield(L, -2, "math");

    // Create the ecs subtable
    lua_newtable(L);
    luaL_setfuncs(L, lua_pomegranate_ecs, 0);

    // Set the ecs subtable into the pomegranate table
    lua_setfield(L, -2, "ecs");
    return 1;
}