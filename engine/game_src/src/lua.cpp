#include "lua.h"

LuaComponentScript::LuaComponentScript(std::string path)
{
    print_info("Loading Lua script: " + path);
    this->path = path;
}

void LuaComponentScript::run_script()
{
    L = luaL_newstate();
    luaL_openlibs(L);
    luaopen_pomegranate(L);
    int result = luaL_dofile(L, path.c_str());

    if(result != 0)
    {
        print_error("Error loading Lua script: " + path);
        print_error(lua_tostring(L, -1));
    }
    else
    {
        //Retrieve the return value from the script
        lua_gettop(L);
        this->component_data = lua_checkcomponent(L);
        if(this->component_data.find("name")!=this->component_data.end())
        {
            this->name = std::any_cast<std::string>(this->component_data["name"]);
        }
        else
        {
            print_error("Lua script does not contain a name field: " + path);
        }
    }
    lua_close(L);
    print_info("Lua script loaded: " + path);
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

int vec2_new(lua_State *L) {
    Vec2 *vec = (Vec2 *)lua_newuserdata(L, sizeof(Vec2));
    vec->x = luaL_checknumber(L, 1);
    vec->y = luaL_checknumber(L, 2);
    luaL_getmetatable(L, "Vec2");
    lua_setmetatable(L, -2);
    return 1;
}

int vec2_index(lua_State *L) {
    Vec2 *vec = (Vec2 *)luaL_checkudata(L, 1, "Vec2");
    const char *key = luaL_checkstring(L, 2);
    if (strcmp(key, "x") == 0) {
        lua_pushnumber(L, vec->x);
    } else if (strcmp(key, "y") == 0) {
        lua_pushnumber(L, vec->y);
    } else {
        lua_pushnil(L);
    }
    return 1;
}

int vec2_add(lua_State *L) {
    Vec2 *v1 = (Vec2 *)luaL_checkudata(L, 1, "Vec2");
    Vec2 *v2 = (Vec2 *)luaL_checkudata(L, 2, "Vec2");
    Vec2 *result = (Vec2 *)lua_newuserdata(L, sizeof(Vec2));
    *result = *v1 + *v2;
    luaL_getmetatable(L, "Vec2");
    lua_setmetatable(L, -2);
    return 1;
}

int vec2_sub(lua_State *L) {
    Vec2 *v1 = (Vec2 *)luaL_checkudata(L, 1, "Vec2");
    Vec2 *v2 = (Vec2 *)luaL_checkudata(L, 2, "Vec2");
    Vec2 *result = (Vec2 *)lua_newuserdata(L, sizeof(Vec2));
    *result = *v1 - *v2;
    luaL_getmetatable(L, "Vec2");
    lua_setmetatable(L, -2);
    return 1;
}

int vec2_mul(lua_State *L) {
    Vec2 *v1 = (Vec2 *)luaL_checkudata(L, 1, "Vec2");
    float scalar = luaL_checknumber(L, 2);
    Vec2 *result = (Vec2 *)lua_newuserdata(L, sizeof(Vec2));
    *result = *v1 * scalar;
    luaL_getmetatable(L, "Vec2");
    lua_setmetatable(L, -2);
    return 1;
}

int vec2_div(lua_State *L) {
    Vec2 *v1 = (Vec2 *)luaL_checkudata(L, 1, "Vec2");
    float scalar = luaL_checknumber(L, 2);
    Vec2 *result = (Vec2 *)lua_newuserdata(L, sizeof(Vec2));
    *result = *v1 / scalar;
    luaL_getmetatable(L, "Vec2");
    lua_setmetatable(L, -2);
    return 1;
}

int vec2_normalized(lua_State *L) {
    Vec2 *v1 = (Vec2 *)luaL_checkudata(L, 1, "Vec2");
    Vec2 *result = (Vec2 *)lua_newuserdata(L, sizeof(Vec2));
    *result = v1->normalized();
    luaL_getmetatable(L, "Vec2");
    lua_setmetatable(L, -2);
    return 1;
}

int vec2_length(lua_State *L) {
    Vec2 *v1 = (Vec2 *)luaL_checkudata(L, 1, "Vec2");
    lua_pushnumber(L, v1->length());
    return 1;
}

int vec2_dot(lua_State *L) {
    Vec2 *v1 = (Vec2 *)luaL_checkudata(L, 1, "Vec2");
    Vec2 *v2 = (Vec2 *)luaL_checkudata(L, 2, "Vec2");
    lua_pushnumber(L, v1->dot(*v2));
    return 1;
}

int vec2_lerp(lua_State *L) {
    Vec2 *v1 = (Vec2 *)luaL_checkudata(L, 1, "Vec2");
    Vec2 *v2 = (Vec2 *)luaL_checkudata(L, 2, "Vec2");
    float t = luaL_checknumber(L, 3);
    Vec2 *result = (Vec2 *)lua_newuserdata(L, sizeof(Vec2));
    *result = v1->lerp(*v2, t);
    luaL_getmetatable(L, "Vec2");
    lua_setmetatable(L, -2);
    return 1;
}

int vec2_rotate(lua_State *L) {
    Vec2 *v1 = (Vec2 *)luaL_checkudata(L, 1, "Vec2");
    float angle = luaL_checknumber(L, 2);
    Vec2 *result = (Vec2 *)lua_newuserdata(L, sizeof(Vec2));
    *result = v1->rotate(angle);
    luaL_getmetatable(L, "Vec2");
    lua_setmetatable(L, -2);
    return 1;
}

int vec2_distance_to(lua_State *L) {
    Vec2 *v1 = (Vec2 *)luaL_checkudata(L, 1, "Vec2");
    Vec2 *v2 = (Vec2 *)luaL_checkudata(L, 2, "Vec2");
    lua_pushnumber(L, v1->distance_to(*v2));
    return 1;
}

int vec2_angle_to(lua_State *L) {
    Vec2 *v1 = (Vec2 *)luaL_checkudata(L, 1, "Vec2");
    Vec2 *v2 = (Vec2 *)luaL_checkudata(L, 2, "Vec2");
    lua_pushnumber(L, v1->angle_to(*v2));
    return 1;
}

int vec2_gc(lua_State *L) {
    Vec2 *vec = (Vec2 *)luaL_checkudata(L, 1, "Vec2");
    //delete vec;
    return 0;
}

int vec2_tostring(lua_State *L) {
    Vec2 *vec = (Vec2 *)luaL_checkudata(L, 1, "Vec2");
    lua_pushfstring(L, "Vec2(%f, %f)", vec->x, vec->y);
    return 1;
}

int vec2_set_index(lua_State *L) {
    Vec2 *vec = (Vec2 *)luaL_checkudata(L, 1, "Vec2");
    const char *key = luaL_checkstring(L, 2);
    if (strcmp(key, "x") == 0) {
        vec->x = luaL_checknumber(L, 3);
    } else if (strcmp(key, "y") == 0) {
        vec->y = luaL_checknumber(L, 3);
    } else {
        // If key is neither "x" nor "y", raise an error
        luaL_error(L, "Invalid key '%s' for Vec2", key);
    }
    return 0;
}

const struct luaL_Reg vec2_functions[] = {
        {"new", vec2_new},
        {"__add", vec2_add},
        {"__sub", vec2_sub},
        {"__mul", vec2_mul},
        {"__div", vec2_div},
        {"normalized", vec2_normalized},
        {"length", vec2_length},
        {"dot", vec2_dot},
        {"lerp", vec2_lerp},
        {"rotate", vec2_rotate},
        {"distance_to", vec2_distance_to},
        {"angle_to", vec2_angle_to},
        {"__gc", vec2_gc},
        {"__index", vec2_index},
        {"__tostring", vec2_tostring},
        {"__newindex", vec2_set_index},
        {nullptr, nullptr}
};

//Vec3

int vec3_new(lua_State *L) {
    Vec3 *vec = (Vec3 *)lua_newuserdata(L, sizeof(Vec3));
    vec->x = luaL_checknumber(L, 1);
    vec->y = luaL_checknumber(L, 2);
    vec->z = luaL_checknumber(L, 3);
    luaL_getmetatable(L, "Vec3");
    lua_setmetatable(L, -2);
    return 1;
}

int vec3_index(lua_State *L) {
    Vec3 *vec = (Vec3 *)luaL_checkudata(L, 1, "Vec3");
    const char *key = luaL_checkstring(L, 2);
    if (strcmp(key, "x") == 0) {
        lua_pushnumber(L, vec->x);
    } else if (strcmp(key, "y") == 0) {
        lua_pushnumber(L, vec->y);
    } else if (strcmp(key, "z") == 0) {
        lua_pushnumber(L, vec->z);
    } else {
        lua_pushnil(L);
    }
    return 1;
}

int vec3_add(lua_State *L) {
    Vec3 *v1 = (Vec3 *)luaL_checkudata(L, 1, "Vec3");
    Vec3 *v2 = (Vec3 *)luaL_checkudata(L, 2, "Vec3");
    Vec3 *result = (Vec3 *)lua_newuserdata(L, sizeof(Vec3));
    *result = *v1 + *v2;
    luaL_getmetatable(L, "Vec3");
    lua_setmetatable(L, -2);
    return 1;
}

int vec3_sub(lua_State *L) {
    Vec3 *v1 = (Vec3 *)luaL_checkudata(L, 1, "Vec3");
    Vec3 *v2 = (Vec3 *)luaL_checkudata(L, 2, "Vec3");
    Vec3 *result = (Vec3 *)lua_newuserdata(L, sizeof(Vec3));
    *result = *v1 - *v2;
    luaL_getmetatable(L, "Vec3");
    lua_setmetatable(L, -2);
    return 1;
}

int vec3_mul(lua_State *L) {
    Vec3 *v1 = (Vec3 *)luaL_checkudata(L, 1, "Vec3");
    float scalar = luaL_checknumber(L, 2);
    Vec3 *result = (Vec3 *)lua_newuserdata(L, sizeof(Vec3));
    *result = *v1 * scalar;
    luaL_getmetatable(L, "Vec3");
    lua_setmetatable(L, -2);
    return 1;
}

int vec3_div(lua_State *L) {
    Vec3 *v1 = (Vec3 *)luaL_checkudata(L, 1, "Vec3");
    float scalar = luaL_checknumber(L, 2);
    Vec3 *result = (Vec3 *)lua_newuserdata(L, sizeof(Vec3));
    *result = *v1 / scalar;
    luaL_getmetatable(L, "Vec3");
    lua_setmetatable(L, -2);
    return 1;
}

int vec3_normalized(lua_State *L) {
    Vec3 *v1 = (Vec3 *)luaL_checkudata(L, 1, "Vec3");
    Vec3 *result = (Vec3 *)lua_newuserdata(L, sizeof(Vec3));
    *result = v1->normalized();
    luaL_getmetatable(L, "Vec3");
    lua_setmetatable(L, -2);
    return 1;
}

int vec3_length(lua_State *L) {
    Vec3 *v1 = (Vec3 *)luaL_checkudata(L, 1, "Vec3");
    lua_pushnumber(L, v1->length());
    return 1;
}

int vec3_dot(lua_State *L) {
    Vec3 *v1 = (Vec3 *)luaL_checkudata(L, 1, "Vec3");
    Vec3 *v2 = (Vec3 *)luaL_checkudata(L, 2, "Vec3");
    lua_pushnumber(L, v1->dot(*v2));
    return 1;
}

int vec3_cross(lua_State *L) {
    Vec3 *v1 = (Vec3 *)luaL_checkudata(L, 1, "Vec3");
    Vec3 *v2 = (Vec3 *)luaL_checkudata(L, 2, "Vec3");
    Vec3 *result = (Vec3 *)lua_newuserdata(L, sizeof(Vec3));
    *result = v1->cross(*v2);
    luaL_getmetatable(L, "Vec3");
    lua_setmetatable(L, -2);
    return 1;
}

int vec3_lerp(lua_State *L) {
    Vec3 *v1 = (Vec3 *)luaL_checkudata(L, 1, "Vec3");
    Vec3 *v2 = (Vec3 *)luaL_checkudata(L, 2, "Vec3");
    float t = luaL_checknumber(L, 3);
    Vec3 *result = (Vec3 *)lua_newuserdata(L, sizeof(Vec3));
    *result = v1->lerp(*v2, t);
    luaL_getmetatable(L, "Vec3");
    lua_setmetatable(L, -2);
    return 1;
}

int vec3_distance_to(lua_State *L) {
    Vec3 *v1 = (Vec3 *)luaL_checkudata(L, 1, "Vec3");
    Vec3 *v2 = (Vec3 *)luaL_checkudata(L, 2, "Vec3");
    lua_pushnumber(L, v1->distance_to(*v2));
    return 1;
}

int vec3_angle_to(lua_State *L) {
    Vec3 *v1 = (Vec3 *)luaL_checkudata(L, 1, "Vec3");
    Vec3 *v2 = (Vec3 *)luaL_checkudata(L, 2, "Vec3");
    lua_pushnumber(L, v1->angle_to(*v2));
    return 1;
}

int vec3_gc(lua_State *L) {
    Vec3 *vec = (Vec3 *)luaL_checkudata(L, 1, "Vec3");
    delete vec;
    return 0;
}

int vec3_tostring(lua_State *L) {
    Vec3 *vec = (Vec3 *)luaL_checkudata(L, 1, "Vec3");
    lua_pushfstring(L, "Vec3(%f, %f, %f)", vec->x, vec->y, vec->z);
    return 1;
}

int vec3_set_index(lua_State *L) {
    Vec3 *vec = (Vec3 *)luaL_checkudata(L, 1, "Vec3");
    const char *key = luaL_checkstring(L, 2);
    if (strcmp(key, "x") == 0) {
        vec->x = luaL_checknumber(L, 3);
    } else if (strcmp(key, "y") == 0) {
        vec->y = luaL_checknumber(L, 3);
    } else if (strcmp(key, "z") == 0) {
        vec->z = luaL_checknumber(L, 3);
    } else {
        // If key is neither "x" nor "y", raise an error
        luaL_error(L, "Invalid key '%s' for Vec3", key);
    }
    return 0;
}

const struct luaL_Reg vec3_functions[] = {
        {"new", vec3_new},
        {"__add", vec3_add},
        {"__sub", vec3_sub},
        {"__mul", vec3_mul},
        {"__div", vec3_div},
        {"normalized", vec3_normalized},
        {"length", vec3_length},
        {"dot", vec3_dot},
        {"cross", vec3_cross},
        {"lerp", vec3_lerp},
        {"distance_to", vec3_distance_to},
        {"angle_to", vec3_angle_to},
        {"__gc", vec3_gc},
        {"__index", vec3_index},
        {"__tostring", vec3_tostring},
        {"__newindex", vec3_set_index},
        {nullptr, nullptr}
};

//Color

int color_new(lua_State *L) {
    Color *color = (Color *)lua_newuserdata(L, sizeof(Color));
    color->r = luaL_checknumber(L, 1);
    color->g = luaL_checknumber(L, 2);
    color->b = luaL_checknumber(L, 3);
    color->a = luaL_checknumber(L, 4);
    luaL_getmetatable(L, "Color");
    lua_setmetatable(L, -2);
    return 1;
}

int color_index(lua_State *L) {
    Color *color = (Color *)luaL_checkudata(L, 1, "Color");
    const char *key = luaL_checkstring(L, 2);
    if (strcmp(key, "r") == 0) {
        lua_pushnumber(L, color->r);
    } else if (strcmp(key, "g") == 0) {
        lua_pushnumber(L, color->g);
    } else if (strcmp(key, "b") == 0) {
        lua_pushnumber(L, color->b);
    } else if (strcmp(key, "a") == 0) {
        lua_pushnumber(L, color->a);
    } else {
        lua_pushnil(L);
    }
    return 1;
}

int color_add(lua_State *L) {
    Color *c1 = (Color *)luaL_checkudata(L, 1, "Color");
    Color *c2 = (Color *)luaL_checkudata(L, 2, "Color");
    Color *result = (Color *)lua_newuserdata(L, sizeof(Color));
    *result = *c1 + *c2;
    luaL_getmetatable(L, "Color");
    lua_setmetatable(L, -2);
    return 1;
}

int color_sub(lua_State *L) {
    Color *c1 = (Color *)luaL_checkudata(L, 1, "Color");
    Color *c2 = (Color *)luaL_checkudata(L, 2, "Color");
    Color *result = (Color *)lua_newuserdata(L, sizeof(Color));
    *result = *c1 - *c2;
    luaL_getmetatable(L, "Color");
    lua_setmetatable(L, -2);
    return 1;
}

int color_mul(lua_State *L) {
    Color *c1 = (Color *)luaL_checkudata(L, 1, "Color");
    float scalar = luaL_checknumber(L, 2);
    Color *result = (Color *)lua_newuserdata(L, sizeof(Color));
    *result = *c1 * scalar;
    luaL_getmetatable(L, "Color");
    lua_setmetatable(L, -2);
    return 1;
}

int color_div(lua_State *L) {
    Color *c1 = (Color *)luaL_checkudata(L, 1, "Color");
    float scalar = luaL_checknumber(L, 2);
    Color *result = (Color *)lua_newuserdata(L, sizeof(Color));
    *result = *c1 / scalar;
    luaL_getmetatable(L, "Color");
    lua_setmetatable(L, -2);
    return 1;
}

int color_tostring(lua_State *L) {
    Color *color = (Color *)luaL_checkudata(L, 1, "Color");
    lua_pushfstring(L, "Color(%f, %f, %f, %f)", color->r, color->g, color->b, color->a);
    return 1;
}

int color_gc(lua_State *L) {
    Color *color = (Color *)luaL_checkudata(L, 1, "Color");
    delete color;
    return 0;
}

int color_set_index(lua_State *L) {
    Color *color = (Color *)luaL_checkudata(L, 1, "Color");
    const char *key = luaL_checkstring(L, 2);
    if (strcmp(key, "r") == 0) {
        color->r = luaL_checknumber(L, 3);
    } else if (strcmp(key, "g") == 0) {
        color->g = luaL_checknumber(L, 3);
    } else if (strcmp(key, "b") == 0) {
        color->b = luaL_checknumber(L, 3);
    } else if (strcmp(key, "a") == 0) {
        color->a = luaL_checknumber(L, 3);
    } else {
        // If key is neither "r", "g", "b" nor "a", raise an error
        luaL_error(L, "Invalid key '%s' for Color", key);
    }
    return 0;
}

const struct luaL_Reg color_functions[] = {
        {"new", color_new},
        {"__add", color_add},
        {"__sub", color_sub},
        {"__mul", color_mul},
        {"__div", color_div},
        {"__tostring", color_tostring},
        {"__gc", color_gc},
        {"__index", color_index},
        {"__newindex", color_set_index},
        {nullptr, nullptr}
};

//Component

std::unordered_map<std::string, std::any> lua_checkcomponent(lua_State *L) {
    std::unordered_map<std::string, std::any> data;
    //Extract data from table argument
    lua_pushnil(L);
    while(lua_next(L, -2) != 0) {
        const char *key = lua_tostring(L, -2);
        if(lua_isnumber(L, -1)) {
            data[key] = (lua_tonumber(L, -1));
        } else if(lua_isstring(L, -1)) {
            data[key] = std::string(lua_tostring(L, -1));
        } else if(lua_isuserdata(L, -1)) {
            if(luaL_testudata(L, -1, "Vec2")) {
                Vec2 *vec = (Vec2 *)luaL_checkudata(L, -1, "Vec2");
                print_info("Vec2: " + std::to_string(vec->x) + ", " + std::to_string(vec->y));
                data[key] = *vec;
            } else if(luaL_testudata(L, -1, "Vec3")) {
                Vec3 *vec = (Vec3 *)luaL_checkudata(L, -1, "Vec3");
                print_info("Vec3: " + std::to_string(vec->x) + ", " + std::to_string(vec->y) + ", " + std::to_string(vec->z));
                data[key] = *vec;
            } else if(luaL_testudata(L, -1, "Color")) {
                Color *color = (Color *)luaL_checkudata(L, -1, "Color");
                print_info("Color: " + std::to_string(color->r) + ", " + std::to_string(color->g) + ", " + std::to_string(color->b) + ", " + std::to_string(color->a));
                data[key] = *color;
            }
        }
        lua_pop(L, 1);
    }
    return data;
}

int component_from(lua_State *L) {
    lua_checkcomponent(L);
    return 0;
}

const struct luaL_Reg component_functions[] = {
        {"from", component_from},
        {nullptr, nullptr}
};

int luaopen_pomegranate(lua_State *L)
{
    luaL_newmetatable(L, "Vec2");
    luaL_setfuncs(L, vec2_functions, 0);
    lua_pushvalue(L, -1);
    lua_setglobal(L, "Vec2");

    luaL_newmetatable(L, "Vec3");
    luaL_setfuncs(L, vec3_functions, 0);
    lua_pushvalue(L, -1);
    lua_setglobal(L, "Vec3");

    luaL_newmetatable(L, "Color");
    luaL_setfuncs(L, color_functions, 0);
    lua_pushvalue(L, -1);
    lua_setglobal(L, "Color");

    luaL_newmetatable(L, "Component");
    luaL_setfuncs(L, component_functions, 0);
    lua_pushvalue(L, -1);
    lua_setglobal(L, "Component");

    return 1;
}