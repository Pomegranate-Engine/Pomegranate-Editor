#include "lua.h"

std::unordered_map<std::string,LuaComponentScript*> LuaComponentScript::lua_component_types;

lua_State* lua_state;

LuaComponentScript::LuaComponentScript(std::string path)
{
    print_info("Loading Lua component script: " + path);
    this->path = path;
    //Load script into std::string
    std::ifstream file(path);
    std::string str;
    std::string line;
    while (std::getline(file, line))
    {
        str += line + "\n";
    }
    this->script = str;

    if(lua_state == nullptr)
        lua_state = luaL_newstate();
    run_script();
}

void LuaComponentScript::run_script()
{
    luaL_openlibs(lua_state);
    luaL_openpomegranate(lua_state);
    luaL_requiref(lua_state, "pomegranate", luaL_openpomegranate, 1);
    lua_pop(lua_state, 1);
    if (luaL_dostring(lua_state, script.c_str())) {
        printf("Error: %s\n", lua_tostring(lua_state, -1));
    }

    if (lua_istable(lua_state, -1)) {
        print_info("Script loaded successfully");
        //Get all the fields
        lua_pushnil(lua_state);
        while (lua_next(lua_state, -2) != 0) {
            //Get the key
            const char* key = lua_tostring(lua_state, -2);
            print_info("Key: " + std::string(key));
            //Get the value

            if(strcmp(key,"requires") == 0)
            {
                //Get the value
                if (lua_istable(lua_state, -1)) {
                    lua_pushnil(lua_state);
                    while (lua_next(lua_state, -2) != 0) {
                        const char* required = lua_tostring(lua_state, -1);
                        print_info("Required: " + std::string(required));
                        lua_pop(lua_state, 1);
                    }
                }
            }
            else if (lua_isnumber(lua_state, -1)) {
                double* value = new double(lua_tonumber(lua_state, -1));
                set<double>(key,value);
            }
            else if (lua_isboolean(lua_state, -1)) {
                bool* value = new bool(lua_toboolean(lua_state, -1));
                set<bool>(key,value);
            }
            else if (lua_isuserdata(lua_state, -1)) {
                if (lua_getmetatable(lua_state, -1)) {
                    lua_getfield(lua_state, -1, "type");
                    const char *type = lua_tostring(lua_state, -1);
                    if (strcmp(type, "Vec2") == 0) {
                        Vec2* vec = new Vec2(*(Vec2*)lua_touserdata(lua_state, -3));
                        set<Vec2>(key,vec);
                        lua_pop(lua_state, 1);
                    }
                    else if (strcmp(type, "Vec3") == 0) {
                        Vec3* vec = new Vec3(*(Vec3*)lua_touserdata(lua_state, -3));
                        set<Vec3>(key,vec);
                        lua_pop(lua_state, 1);
                    }
                    else {
                        printf("Unexpected type\n");
                    }
                    lua_pop(lua_state, 1);
                }
            }
            else if (lua_isstring(lua_state, -1)) {
                std::string* value = new std::string(lua_tostring(lua_state, -1));
                if (strcmp(key, "name") == 0) {
                    name = *value;
                    print_info("Name: " + name);
                    lua_component_types[name] = this;
                }
                else
                {
                    set<std::string>(key,value);
                }
            }
            lua_pop(lua_state, 1);
        }
    } else {
        printf("Unexpected result type\n");
    }
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

        if(script!= nullptr && script->name == name)
        {
            return script;
        }
    }
    return nullptr;
}

LuaComponentScript* LuaComponent::add_component(std::string name)
{
    if(LuaComponentScript::lua_component_types.find(name) != LuaComponentScript::lua_component_types.end())
    {
        LuaComponentScript* script = LuaComponentScript::lua_component_types[name];
        scripts.push_back(script);
        return script;
    }
    return nullptr;
}

int LuaComponent::lua_get_component(lua_State *L)
{

}

//Lua systems

LuaSystemScript::LuaSystemScript(std::string path)
{
    print_info("Loading Lua script: " + path);
    this->path = path;

    //Load script into std::string
    std::ifstream file(path);
    std::string str;
    std::string line;
    while (std::getline(file, line))
    {
        str += line + "\n";
    }
    this->script = str;

    if(lua_state == nullptr)
        lua_state = luaL_newstate();
    run_script();
}

LuaSystemScript::~LuaSystemScript()
{

}

void LuaSystemScript::run_script()
{
    luaL_openlibs(lua_state);
    luaL_openpomegranate(lua_state);
    luaL_requiref(lua_state, "pomegranate", luaL_openpomegranate, 1);
    lua_pop(lua_state, 1);
    luaL_dostring(lua_state, script.c_str());

    //Check errors
    if(lua_isstring(lua_state, -1))
    {
        print_error(lua_tostring(lua_state, -1));
    }
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

    lua_pushstring(L, "Vec2");
    lua_setfield(L, -2, "type");

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
    if(vec1 == nullptr)
        return 0;
    if(vec2 == nullptr)
        return 0;
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
        if(vec2 == nullptr)
        {
            luaL_error(L, "Invalid argument");
        }
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

    //Push a type field
    lua_pushstring(L, "Vec3");
    lua_setfield(L, -2, "type");

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
        print_info("Getting z");
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
        print_info("Setting z");
        vec->z = lua_tonumber(L, 3);
        print_info(std::to_string(lua_tonumber(L, 3)));
        print_info(std::to_string(vec->z));
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

#pragma region component


static int lua_component_index(lua_State* L)
{
    Component* c = *(Component**)lua_touserdata(L, 1);
    const char* key = lua_tostring(L, 2);
    if(c->component_data.find(std::string(key)) != c->component_data.end())
    {
        if(c->component_data[std::string(key)].first == &typeid(std::string))
        {
            lua_pushstring(L, ((std::string*)c->component_data[std::string(key)].second)->c_str());
            return 1;
        }
        else if(c->component_data[std::string(key)].first == &typeid(float))
        {
            lua_pushnumber(L, *(float*)c->component_data[std::string(key)].second);
            return 1;
        }
        else if(c->component_data[std::string(key)].first == &typeid(double))
        {
            lua_pushnumber(L, *(double*)(c->component_data[std::string(key)].second));
            return 1;
        }
        else if(c->component_data[std::string(key)].first == &typeid(int))
        {
            lua_pushnumber(L, *(int*)c->component_data[std::string(key)].second);
            return 1;
        }
        else if(c->component_data[std::string(key)].first == &typeid(bool))
        {
            lua_pushboolean(L, *(bool*)c->component_data[std::string(key)].second);
            return 1;
        }
        else if(c->component_data[std::string(key)].first == &typeid(Vec2))
        {
            //Create full user data with metatable don't make a new vec2
            Vec2* vec = (Vec2*)c->component_data[std::string(key)].second;
            lua_pushlightuserdata(L, vec);
            //Add metatable and functions
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
            lua_pushstring(L, "Vec2");
            lua_setfield(L, -2, "type");
            lua_setmetatable(L, -2);
            return 1;
        }
        else if(c->component_data[std::string(key)].first == &typeid(Vec3))
        {
            Vec3* vec = new Vec3(*(Vec3*)c->component_data[std::string(key)].second);
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
            lua_pushstring(L, "Vec3");
            lua_setfield(L, -2, "type");
            lua_setmetatable(L, -2);
            return 1;
        }
        else if(c->component_data[std::string(key)].first == &typeid(Texture*))
        {
            Texture* tex = *(Texture**)c->component_data[std::string(key)].second;
            lua_pushstring(L, tex->path.c_str());
            return 1;
        }
        else
        {
            print_error("Unknown type");
        }
    }
    else
    {
        lua_pushnil(L);
    }
    return 0;
}

static int lua_component_set_index(lua_State* L)
{
    Component* c = *(Component**)lua_touserdata(L, 1);
    const char* key = lua_tostring(L, 2);
    if(c->component_data[std::string(key)].first == &typeid(std::string))
    {
        *(std::string*)c->component_data[std::string(key)].second = lua_tostring(L, 3);
    }
    else if(c->component_data[std::string(key)].first == &typeid(float))
    {
        *(float*)c->component_data[std::string(key)].second = lua_tonumber(L, 3);
    }
    else if(c->component_data[std::string(key)].first == &typeid(double))
    {
        *(double*)c->component_data[std::string(key)].second = lua_tonumber(L, 3);
    }
    else if(c->component_data[std::string(key)].first == &typeid(int))
    {
        *(int*)c->component_data[std::string(key)].second = lua_tonumber(L, 3);
    }
    else if(c->component_data[std::string(key)].first == &typeid(bool))
    {
        *(bool*)c->component_data[std::string(key)].second = lua_toboolean(L, 3);
    }
    else if(c->component_data[std::string(key)].first == &typeid(Vec2))
    {
        Vec2* vec = (Vec2*)lua_touserdata(L, 3);
        *(Vec2*)c->component_data[std::string(key)].second = *vec;
    }
    else if(c->component_data[std::string(key)].first == &typeid(Vec3))
    {
        Vec3* vec = (Vec3*)lua_touserdata(L, 3);
        *(Vec3*)c->component_data[std::string(key)].second = *vec;
    }
    else if(c->component_data[std::string(key)].first == &typeid(Texture*))
    {
        print_info("Setting texture");
        *(Texture**)c->component_data[std::string(key)].second = ResourceManager::load<Texture>(lua_tostring(L, 3));
    }

    return 0;
}

static int push_component(lua_State* L, Component* c)
{
    lua_newuserdata(L, sizeof(Component*));
    *(Component**)lua_touserdata(L, -1) = c;
    lua_newtable(L);
    lua_pushcfunction(L, lua_component_index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, lua_component_set_index);
    lua_setfield(L, -2, "__newindex");
    lua_setmetatable(L, -2);
    return 1;
}

static int lua_lua_component_index(lua_State* L)
{
    LuaComponentScript* c = *(LuaComponentScript**)lua_touserdata(L, 1);
    const char* key = lua_tostring(L, 2);
    //Get user data
    if(c->component_data.find(std::string(key)) != c->component_data.end())
    {
        if(c->component_data[key].first == &typeid(std::string))
        {
            lua_pushstring(L, ((std::string*)c->component_data[key].second)->c_str());
            return 1;
        }
        else if(c->component_data[key].first == &typeid(float))
        {
            lua_pushnumber(L, *(float*)c->component_data[key].second);
            return 1;
        }
        else if(c->component_data[key].first == &typeid(double))
        {
            lua_pushnumber(L, *(double*)c->component_data[key].second);
            return 1;
        }
        else if(c->component_data[key].first == &typeid(int))
        {
            lua_pushnumber(L, *(int*)c->component_data[key].second);
            return 1;
        }
        else if(c->component_data[key].first == &typeid(bool))
        {
            lua_pushboolean(L, *(bool*)c->component_data[key].second);
            return 1;
        }
        else if(c->component_data[key].first == &typeid(Vec2))
        {
            Vec2* vec = (Vec2*)c->component_data[key].second;
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
            lua_pushstring(L, "Vec2");
            lua_setfield(L, -2, "type");
            lua_setmetatable(L, -2);
            return 1;
        }
        else if(c->component_data[key].first == &typeid(Vec3))
        {
            Vec3 *vec = (Vec3*)c->component_data[key].second;
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
            lua_pushstring(L, "Vec3");
            lua_setfield(L, -2, "type");
            lua_setmetatable(L, -2);
            return 1;
        }
    }
    return 0;
}

static int lua_lua_component_set_index(lua_State* L)
{
    LuaComponentScript* c = *(LuaComponentScript**)lua_touserdata(L, 1);
    const char* key = lua_tostring(L, 2);
    if(c->component_data.find(std::string(key)) != c->component_data.end())
    {
        if(c->component_data[key].first == &typeid(std::string))
        {
            *(std::string*)c->component_data[key].second = lua_tostring(L, 3);
        }
        else if(c->component_data[key].first == &typeid(float))
        {
            *(float*)c->component_data[key].second = lua_tonumber(L, 3);
        }
        else if(c->component_data[key].first == &typeid(double))
        {
            *(double*)c->component_data[key].second = lua_tonumber(L, 3);
        }
        else if(c->component_data[key].first == &typeid(int))
        {
            *(int*)c->component_data[key].second = lua_tonumber(L, 3);
        }
        else if(c->component_data[key].first == &typeid(bool))
        {
            *(bool*)c->component_data[key].second = lua_toboolean(L, 3);
        }
        else if(c->component_data[key].first == &typeid(Vec2))
        {
            Vec2* vec = (Vec2*)lua_touserdata(L, 3);
            *(Vec2*)c->component_data[key].second = *vec;
        }
        else if(c->component_data[key].first == &typeid(Vec3))
        {
            Vec3* vec = (Vec3*)lua_touserdata(L, 3);
            *(Vec3*)c->component_data[key].second = *vec;
        }
    }
    return 0;
}

static int push_lua_component(lua_State* L, LuaComponentScript* c)
{
    lua_newuserdata(L, sizeof(LuaComponentScript*));
    *(LuaComponentScript**)lua_touserdata(L, -1) = c;
    lua_newtable(L);
    lua_pushcfunction(L, lua_lua_component_index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, lua_lua_component_set_index);
    lua_setfield(L, -2, "__newindex");
    lua_setmetatable(L, -2);
    return 1;
}

#pragma endregion

#pragma region entity

static int lua_entity_add_component(lua_State* L)
{
    Entity* e = *(Entity**)lua_touserdata(L, 1);
    std::string name = lua_tostring(L, 2);
    e->add_component(name.c_str());
    return 0;
}

static int lua_entity_get_component(lua_State* L)
{
    Entity* e = *(Entity**)lua_touserdata(L, 1);
    std::string name = lua_tostring(L, 2);
    if(!e->has_component(name.c_str()))
    {
        LuaComponent *lc = e->get_component<LuaComponent>();
        if(lc != nullptr)
        {
            LuaComponentScript* script = lc->get_component(name);
            print_info("Pushing lua component");
            push_lua_component(L, script);
        }
    }
    else
    {
        Component* c = e->get_component(name.c_str());
        push_component(L, c);
        return 1;
    }
}

static int lua_entity_has_component(lua_State* L)
{
    Entity* e = *(Entity**)lua_touserdata(L, 1);
    std::string name = lua_tostring(L, 2);
    bool has = e->has_component(name.c_str());
    if(!has)
    {
        //Check if it is a lua component
        LuaComponent *lc = e->get_component<LuaComponent>();
        if(lc != nullptr)
        {
            LuaComponentScript* script = lc->get_component(name);
            if(script != nullptr)
            {
                lua_pushboolean(L, true);
                return 1;
            }
            else
            {
                lua_pushboolean(L, false);
                return 1;
            }
        }
        else
        {
            lua_pushboolean(L, false);
            return 1;
        }
    }
    lua_pushboolean(L, true);
    return 1;
}

static int lua_entity_destroy(lua_State* L)
{
    Entity* e = *(Entity**)lua_touserdata(L, 1);
    e->destroy();
    return 0;
}

static int lua_entity_index(lua_State* L)
{
    Entity* e = *(Entity**)lua_touserdata(L, 1);
    const char* key = lua_tostring(L, 2);
    if(strcmp(key, "name") == 0)
    {
        lua_pushstring(L, e->name.c_str());
        return 1;
    }
    else if(strcmp(key, "id") == 0)
    {
        lua_pushnumber(L, e->id);
        return 1;
    }
    else if(strcmp(key, "addComponent") == 0)
    {
        lua_pushcfunction(L, lua_entity_add_component);
        return 1;
    }
    else if(strcmp(key, "getComponent") == 0)
    {
        lua_pushcfunction(L, lua_entity_get_component);
        return 1;
    }
    else if(strcmp(key, "hasComponent") == 0)
    {
        lua_pushcfunction(L, lua_entity_has_component);
        return 1;
    }
    else if(strcmp(key, "destroy") == 0)
    {
        lua_pushcfunction(L, lua_entity_destroy);
        return 1;
    }

    return 0;
}
static int lua_entity_set_index(lua_State* L)
{
    Entity* e = *(Entity**)lua_touserdata(L, 1);
    const char* key = lua_tostring(L, 2);
    if(strcmp(key, "name") == 0)
    {
        e->name = lua_tostring(L, 3);
        return 0;
    }
    else if(strcmp(key, "id") == 0)
    {
        e->set_id(lua_tonumber(L, 3));
        return 0;
    }
    return 0;
}

static int lua_entity_new(lua_State* L)
{
    Entity* e = new Entity();
    //get name
    e->name = lua_tostring(L, 1);
    //We will push full userdata for the entity
    lua_newuserdata(L, sizeof(Entity*));
    //Set the entity pointer
    *(Entity**)lua_touserdata(L, -1) = e;
    //Set the metatable
    lua_newtable(L);
    lua_pushcfunction(L, lua_entity_index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, lua_entity_set_index);
    lua_setfield(L, -2, "__newindex");
    lua_setmetatable(L, -2);
    return 1;
}

#pragma endregion

#pragma region group

static int lua_group_add_entity(lua_State* L)
{
    Group* g = *(Group**)lua_touserdata(L, 1);
    Entity* e = *(Entity**)lua_touserdata(L, 2);
    g->add_entity(e);
    return 0;
}

static int lua_group_remove_entity(lua_State* L)
{
    Group* g = *(Group**)lua_touserdata(L, 1);
    Entity* e = *(Entity**)lua_touserdata(L, 2);
    g->remove_entity(e);
    return 0;
}

static int lua_group_add_group(lua_State* L)
{
    Group* g = *(Group**)lua_touserdata(L, 1);
    Group* g2 = *(Group**)lua_touserdata(L, 2);
    g->add_group(g2);
    return 0;
}

static int lua_group_remove_group(lua_State* L)
{
    Group* g = *(Group**)lua_touserdata(L, 1);
    Group* g2 = *(Group**)lua_touserdata(L, 2);
    g->remove_group(g2);
    return 0;
}

static int lua_group_add_system(lua_State* L)
{
    Group* g = *(Group**)lua_touserdata(L, 1);
    LuaSystem* s = *(LuaSystem**)lua_touserdata(L, 2);
    g->add_system(s);
    return 0;
}

static int lua_group_remove_system(lua_State* L)
{
    Group* g = *(Group**)lua_touserdata(L, 1);
    LuaSystem* s = *(LuaSystem**)lua_touserdata(L, 2);
    g->remove_system(s);
    return 0;
}

static int lua_group_index(lua_State* L)
{
    Group* g = *(Group**)lua_touserdata(L, 1);
    const char* key = lua_tostring(L, 2);

    if(strcmp(key, "name") == 0)
    {
        lua_pushstring(L, g->name.c_str());
        return 1;
    }
    else if(strcmp(key, "id") == 0)
    {
        lua_pushnumber(L, g->id);
        return 1;
    }
    else if(strcmp(key,"addEntity") == 0)
    {
        lua_pushcfunction(L, lua_group_add_entity);
        return 1;
    }
    else if(strcmp(key,"removeEntity") == 0)
    {
        lua_pushcfunction(L, lua_group_remove_entity);
        return 1;
    }
    else if(strcmp(key,"addGroup") == 0)
    {
        lua_pushcfunction(L, lua_group_add_group);
        return 1;
    }
    else if(strcmp(key,"removeGroup") == 0)
    {
        lua_pushcfunction(L, lua_group_remove_group);
        return 1;
    }
    else if(strcmp(key,"addSystem") == 0)
    {
        lua_pushcfunction(L, lua_group_add_system);
        return 1;
    }
    else if(strcmp(key,"removeSystem") == 0)
    {
        lua_pushcfunction(L, lua_group_remove_system);
        return 1;
    }

    return 0;
}

static int lua_group_set_index(lua_State* L)
{
    Group* g = *(Group**)lua_touserdata(L, 1);
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

static int lua_group_new(lua_State* L)
{
    const char* name = lua_tostring(L, 1);
    Group* g = new Group(name);
    if(g == nullptr)
    {
        lua_pushnil(L);
        return 1;
    }
    lua_newuserdata(L, sizeof(Group*));
    *(Group**)lua_touserdata(L, -1) = g;
    lua_newtable(L);
    lua_pushcfunction(L, lua_group_index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, lua_group_set_index);
    lua_setfield(L, -2, "__newindex");
    lua_setmetatable(L, -2);
    return 1;
}

static int lua_find_group(lua_State* L)
{
    const char* name = lua_tostring(L, 1);
    Group* g = Group::get_group(name).get();
    if(g == nullptr)
    {
        lua_pushnil(L);
        return 1;
    }
    lua_newuserdata(L, sizeof(Group*));
    *(Group**)lua_touserdata(L, -1) = g;
    lua_newtable(L);
    lua_pushcfunction(L, lua_group_index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, lua_group_set_index);
    lua_setfield(L, -2, "__newindex");
    lua_setmetatable(L, -2);
    return 1;
}

#pragma endregion

#pragma region input

static int lua_input_key_down(lua_State* L)
{
    int key = lua_tonumber(L, 1);
    bool pressed = InputManager::get_key(key);
    lua_pushboolean(L, pressed);
    return 1;
}

static int lua_input_key_up(lua_State* L)
{
    int key = lua_tonumber(L, 1);
    bool pressed = !InputManager::get_key(key);
    lua_pushboolean(L, pressed);
    return 1;
}

static int lua_input_get_mouse_pos(lua_State* L)
{
    Vec2 pos = InputManager::get_mouse_position();
    //Lua light user data
    Vec2* vec = new Vec2(pos);
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
    lua_pushstring(L, "Vec2");
    lua_setfield(L, -2, "type");
    lua_setmetatable(L, -2);

    return 1;
}

static int lua_input_mouse_down(lua_State* L)
{
    int button = lua_tonumber(L, 1);
    bool pressed = InputManager::get_mouse_button(button);
    lua_pushboolean(L, pressed);
    return 1;
}

static int lua_input_mouse_up(lua_State* L)
{
    int button = lua_tonumber(L, 1);
    bool pressed = !InputManager::get_mouse_button(button);
    lua_pushboolean(L, pressed);
    return 1;
}

static int lua_input_get_axis(lua_State* L)
{
    int positive = lua_tonumber(L, 1);
    int negative = lua_tonumber(L, 2);
    float value = 0;
    if(InputManager::get_key(positive))
    {
        value += 1;
    }
    if(InputManager::get_key(negative))
    {
        value -= 1;
    }
    lua_pushnumber(L, value);
    return 1;
}

static int lua_input_get_vector(lua_State* L)
{
    int up = lua_tonumber(L, 1);
    int down = lua_tonumber(L, 2);
    int left = lua_tonumber(L, 3);
    int right = lua_tonumber(L, 4);
    Vec2 value = Vec2(0, 0);
    SDL_SCANCODE_UP;
    SDL_SCANCODE_DOWN;
    SDL_SCANCODE_LEFT;
    SDL_SCANCODE_RIGHT;
    if(InputManager::get_key(up))
    {
        value.y += 1;
    }
    if(InputManager::get_key(down))
    {
        value.y -= 1;
    }
    if(InputManager::get_key(left))
    {
        value.x -= 1;
    }
    if(InputManager::get_key(right))
    {
        value.x += 1;
    }
    Vec2* vec = new Vec2(value);
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
    lua_pushstring(L, "Vec2");
    lua_setfield(L, -2, "type");
    lua_setmetatable(L, -2);
    return 1;
}

#pragma endregion

#pragma region time

static int lua_get_delta_time(lua_State* L)
{
    lua_pushnumber(L, delta_time);
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

luaL_Reg lua_pomegranate_vec2[] = {
        {"new", lua_vec2_new},
        {nullptr, nullptr}
};

luaL_Reg lua_pomegranate_vec3[] = {
        {"new", lua_vec3_new},
        {nullptr, nullptr}
};

luaL_Reg lua_pomegranate_entity[] = {
        {"new", lua_entity_new},
        {nullptr, nullptr}
};

luaL_Reg lua_pomegranate_group[] = {
        {"new", lua_group_new},
        {"find", lua_find_group},
        {nullptr, nullptr}
};

luaL_Reg lua_pomegranate_input[] = {
        {"isKeyDown", lua_input_key_down},
        {"isKeyUp", lua_input_key_up},
        {"isMouseDown", lua_input_mouse_down},
        {"isMouseUp", lua_input_mouse_up},
        {"getMousePos", lua_input_get_mouse_pos},
        {"getAxis", lua_input_get_axis},
        {"getVector", lua_input_get_vector},
        {nullptr, nullptr}
};

luaL_Reg lua_pomegranate_time[]= {
        {"getDeltaTime", lua_get_delta_time},
        {nullptr, nullptr}
};

int luaL_openpomegranate(lua_State *L)
{
    lua_newtable(L);

    // Create the debug subtable
    lua_newtable(L);
    luaL_setfuncs(L, lua_pomegranate_debug, 0);
    lua_setfield(L, -2, "debug");

    lua_newtable(L);
    luaL_setfuncs(L, lua_pomegranate_editor, 0);
    lua_setfield(L, -2, "editor");

    lua_newtable(L);
    luaL_setfuncs(L, lua_pomegranate_vec2, 0);
    lua_setfield(L, -2, "vec2");

    lua_newtable(L);
    luaL_setfuncs(L, lua_pomegranate_vec3, 0);
    lua_setfield(L, -2, "vec3");

    lua_newtable(L);
    luaL_setfuncs(L, lua_pomegranate_entity, 0);
    lua_setfield(L, -2, "entity");

    lua_newtable(L);
    luaL_setfuncs(L, lua_pomegranate_group, 0);
    lua_setfield(L, -2, "group");

    lua_newtable(L);
    luaL_setfuncs(L, lua_pomegranate_input, 0);
    //Push all scancodes
#pragma region scancodes
    lua_pushnumber(L, SDL_SCANCODE_UNKNOWN);
    lua_setfield(L, -2, "KEY_UNKNOWN");
    lua_pushnumber(L, SDL_SCANCODE_A);
    lua_setfield(L, -2, "KEY_A");
    lua_pushnumber(L, SDL_SCANCODE_B);
    lua_setfield(L, -2, "KEY_B");
    lua_pushnumber(L, SDL_SCANCODE_C);
    lua_setfield(L, -2, "KEY_C");
    lua_pushnumber(L, SDL_SCANCODE_D);
    lua_setfield(L, -2, "KEY_D");
    lua_pushnumber(L, SDL_SCANCODE_E);
    lua_setfield(L, -2, "KEY_E");
    lua_pushnumber(L, SDL_SCANCODE_F);
    lua_setfield(L, -2, "KEY_F");
    lua_pushnumber(L, SDL_SCANCODE_G);
    lua_setfield(L, -2, "KEY_G");
    lua_pushnumber(L, SDL_SCANCODE_H);
    lua_setfield(L, -2, "KEY_H");
    lua_pushnumber(L, SDL_SCANCODE_I);
    lua_setfield(L, -2, "KEY_I");
    lua_pushnumber(L, SDL_SCANCODE_J);
    lua_setfield(L, -2, "KEY_J");
    lua_pushnumber(L, SDL_SCANCODE_K);
    lua_setfield(L, -2, "KEY_K");
    lua_pushnumber(L, SDL_SCANCODE_L);
    lua_setfield(L, -2, "KEY_L");
    lua_pushnumber(L, SDL_SCANCODE_M);
    lua_setfield(L, -2, "KEY_M");
    lua_pushnumber(L, SDL_SCANCODE_N);
    lua_setfield(L, -2, "KEY_N");
    lua_pushnumber(L, SDL_SCANCODE_O);
    lua_setfield(L, -2, "KEY_O");
    lua_pushnumber(L, SDL_SCANCODE_P);
    lua_setfield(L, -2, "KEY_P");
    lua_pushnumber(L, SDL_SCANCODE_Q);
    lua_setfield(L, -2, "KEY_Q");
    lua_pushnumber(L, SDL_SCANCODE_R);
    lua_setfield(L, -2, "KEY_R");
    lua_pushnumber(L, SDL_SCANCODE_S);
    lua_setfield(L, -2, "KEY_S");
    lua_pushnumber(L, SDL_SCANCODE_T);
    lua_setfield(L, -2, "KEY_T");
    lua_pushnumber(L, SDL_SCANCODE_U);
    lua_setfield(L, -2, "KEY_U");
    lua_pushnumber(L, SDL_SCANCODE_V);
    lua_setfield(L, -2, "KEY_V");
    lua_pushnumber(L, SDL_SCANCODE_W);
    lua_setfield(L, -2, "KEY_W");
    lua_pushnumber(L, SDL_SCANCODE_X);
    lua_setfield(L, -2, "KEY_X");
    lua_pushnumber(L, SDL_SCANCODE_Y);
    lua_setfield(L, -2, "KEY_Y");
    lua_pushnumber(L, SDL_SCANCODE_Z);
    lua_setfield(L, -2, "KEY_Z");
    lua_pushnumber(L, SDL_SCANCODE_1);
    lua_setfield(L, -2, "KEY_1");
    lua_pushnumber(L, SDL_SCANCODE_2);
    lua_setfield(L, -2, "KEY_2");
    lua_pushnumber(L, SDL_SCANCODE_3);
    lua_setfield(L, -2, "KEY_3");
    lua_pushnumber(L, SDL_SCANCODE_4);
    lua_setfield(L, -2, "KEY_4");
    lua_pushnumber(L, SDL_SCANCODE_5);
    lua_setfield(L, -2, "KEY_5");
    lua_pushnumber(L, SDL_SCANCODE_6);
    lua_setfield(L, -2, "KEY_6");
    lua_pushnumber(L, SDL_SCANCODE_7);
    lua_setfield(L, -2, "KEY_7");
    lua_pushnumber(L, SDL_SCANCODE_8);
    lua_setfield(L, -2, "KEY_8");
    lua_pushnumber(L, SDL_SCANCODE_9);
    lua_setfield(L, -2, "KEY_9");
    lua_pushnumber(L, SDL_SCANCODE_0);
    lua_setfield(L, -2, "KEY_0");
    lua_pushnumber(L, SDL_SCANCODE_RETURN);
    lua_setfield(L, -2, "KEY_RETURN");
    lua_pushnumber(L, SDL_SCANCODE_ESCAPE);
    lua_setfield(L, -2, "KEY_ESCAPE");
    lua_pushnumber(L, SDL_SCANCODE_BACKSPACE);
    lua_setfield(L, -2, "KEY_BACKSPACE");
    lua_pushnumber(L, SDL_SCANCODE_TAB);
    lua_setfield(L, -2, "KEY_TAB");
    lua_pushnumber(L, SDL_SCANCODE_SPACE);
    lua_setfield(L, -2, "KEY_SPACE");
    lua_pushnumber(L, SDL_SCANCODE_MINUS);
    lua_setfield(L, -2, "KEY_MINUS");
    lua_pushnumber(L, SDL_SCANCODE_EQUALS);
    lua_setfield(L, -2, "KEY_EQUALS");
    lua_pushnumber(L, SDL_SCANCODE_LEFTBRACKET);
    lua_setfield(L, -2, "KEY_LEFTBRACKET");
    lua_pushnumber(L, SDL_SCANCODE_RIGHTBRACKET);
    lua_setfield(L, -2, "KEY_RIGHTBRACKET");
    lua_pushnumber(L, SDL_SCANCODE_BACKSLASH);
    lua_setfield(L, -2, "KEY_BACKSLASH");
    lua_pushnumber(L, SDL_SCANCODE_NONUSHASH);
    lua_setfield(L, -2, "KEY_NONUSHASH");
    lua_pushnumber(L, SDL_SCANCODE_SEMICOLON);
    lua_setfield(L, -2, "KEY_SEMICOLON");
    lua_pushnumber(L, SDL_SCANCODE_APOSTROPHE);
    lua_setfield(L, -2, "KEY_APOSTROPHE");
    lua_pushnumber(L, SDL_SCANCODE_GRAVE);
    lua_setfield(L, -2, "KEY_GRAVE");
    lua_pushnumber(L, SDL_SCANCODE_COMMA);
    lua_setfield(L, -2, "KEY_COMMA");
    lua_pushnumber(L, SDL_SCANCODE_PERIOD);
    lua_setfield(L, -2, "KEY_PERIOD");
    lua_pushnumber(L, SDL_SCANCODE_SLASH);
    lua_setfield(L, -2, "KEY_SLASH");
    lua_pushnumber(L, SDL_SCANCODE_CAPSLOCK);
    lua_setfield(L, -2, "KEY_CAPSLOCK");
    lua_pushnumber(L, SDL_SCANCODE_F1);
    lua_setfield(L, -2, "KEY_F1");
    lua_pushnumber(L, SDL_SCANCODE_F2);
    lua_setfield(L, -2, "KEY_F2");
    lua_pushnumber(L, SDL_SCANCODE_F3);
    lua_setfield(L, -2, "KEY_F3");
    lua_pushnumber(L, SDL_SCANCODE_F4);
    lua_setfield(L, -2, "KEY_F4");
    lua_pushnumber(L, SDL_SCANCODE_F5);
    lua_setfield(L, -2, "KEY_F5");
    lua_pushnumber(L, SDL_SCANCODE_F6);
    lua_setfield(L, -2, "KEY_F6");
    lua_pushnumber(L, SDL_SCANCODE_F7);
    lua_setfield(L, -2, "KEY_F7");
    lua_pushnumber(L, SDL_SCANCODE_F8);
    lua_setfield(L, -2, "KEY_F8");
    lua_pushnumber(L, SDL_SCANCODE_F9);
    lua_setfield(L, -2, "KEY_F9");
    lua_pushnumber(L, SDL_SCANCODE_F10);
    lua_setfield(L, -2, "KEY_F10");
    lua_pushnumber(L, SDL_SCANCODE_F11);
    lua_setfield(L, -2, "KEY_F11");
    lua_pushnumber(L, SDL_SCANCODE_F12);
    lua_setfield(L, -2, "KEY_F12");
    lua_pushnumber(L, SDL_SCANCODE_PRINTSCREEN);
    lua_setfield(L, -2, "KEY_PRINTSCREEN");
    lua_pushnumber(L, SDL_SCANCODE_SCROLLLOCK);
    lua_setfield(L, -2, "KEY_SCROLLLOCK");
    lua_pushnumber(L, SDL_SCANCODE_PAUSE);
    lua_setfield(L, -2, "KEY_PAUSE");
    lua_pushnumber(L, SDL_SCANCODE_INSERT);
    lua_setfield(L, -2, "KEY_INSERT");
    lua_pushnumber(L, SDL_SCANCODE_HOME);
    lua_setfield(L, -2, "KEY_HOME");
    lua_pushnumber(L, SDL_SCANCODE_PAGEUP);
    lua_setfield(L, -2, "KEY_PAGEUP");
    lua_pushnumber(L, SDL_SCANCODE_DELETE);
    lua_setfield(L, -2, "KEY_DELETE");
    lua_pushnumber(L, SDL_SCANCODE_END);
    lua_setfield(L, -2, "KEY_END");
    lua_pushnumber(L, SDL_SCANCODE_PAGEDOWN);
    lua_setfield(L, -2, "KEY_PAGEDOWN");
    lua_pushnumber(L, SDL_SCANCODE_RIGHT);
    lua_setfield(L, -2, "KEY_RIGHT");
    lua_pushnumber(L, SDL_SCANCODE_LEFT);
    lua_setfield(L, -2, "KEY_LEFT");
    lua_pushnumber(L, SDL_SCANCODE_DOWN);
    lua_setfield(L, -2, "KEY_DOWN");
    lua_pushnumber(L, SDL_SCANCODE_UP);
    lua_setfield(L, -2, "KEY_UP");
    lua_pushnumber(L, SDL_SCANCODE_NUMLOCKCLEAR);
    lua_setfield(L, -2, "KEY_NUMLOCKCLEAR");
    lua_pushnumber(L, SDL_SCANCODE_LCTRL);
    lua_setfield(L, -2, "KEY_LCTRL");
    lua_pushnumber(L, SDL_SCANCODE_LSHIFT);
    lua_setfield(L, -2, "KEY_LSHIFT");
    lua_pushnumber(L, SDL_SCANCODE_LALT);
    lua_setfield(L, -2, "KEY_LALT");
    lua_pushnumber(L, SDL_SCANCODE_LGUI);
    lua_setfield(L, -2, "KEY_LGUI");
    lua_pushnumber(L, SDL_SCANCODE_RCTRL);
    lua_setfield(L, -2, "KEY_RCTRL");
    lua_pushnumber(L, SDL_SCANCODE_RSHIFT);
    lua_setfield(L, -2, "KEY_RSHIFT");
    lua_pushnumber(L, SDL_SCANCODE_RALT);
    lua_setfield(L, -2, "KEY_RALT");
    lua_pushnumber(L, SDL_SCANCODE_RGUI);
    lua_setfield(L, -2, "KEY_RGUI");
#pragma endregion
    lua_setfield(L, -2, "input");

    lua_newtable(L);
    luaL_setfuncs(L, lua_pomegranate_time, 0);
    lua_setfield(L, -2, "time");


    return 1;
}

void LuaSystem::tick(Entity *e)
{
    if(script != previous_script)
    {
        previous_script = script;
        script->run_script();
    }

    if(script == nullptr)
    {
        return;
    }

    lua_getglobal(lua_state, "tick");

    //Check if the function exists
    if(lua_isnil(lua_state, -1))
    {
        return;
    }

    //Create the entity
    lua_newuserdata(lua_state, sizeof(Entity*));
    *(Entity**)lua_touserdata(lua_state, -1) = e;
    //Set the metatable
    lua_newtable(lua_state);
    lua_pushcfunction(lua_state, lua_entity_index);
    lua_setfield(lua_state, -2, "__index");
    lua_pushcfunction(lua_state, lua_entity_set_index);
    lua_setfield(lua_state, -2, "__newindex");
    lua_setmetatable(lua_state, -2);
    lua_call(lua_state, 1, 0);
}

void LuaSystem::draw(Entity *e)
{
    if(script != previous_script)
    {
        previous_script = script;
        script->run_script();
    }

    if(script == nullptr)
    {
        return;
    }

    

    lua_getglobal(lua_state, "draw");

    //Check if the function exists
    if(lua_isnil(lua_state, -1))
    {
        return;
    }

    //Create the entity
    lua_newuserdata(lua_state, sizeof(Entity*));
    *(Entity**)lua_touserdata(lua_state, -1) = e;
    //Set the metatable
    lua_newtable(lua_state);
    lua_pushcfunction(lua_state, lua_entity_index);
    lua_setfield(lua_state, -2, "__index");
    lua_pushcfunction(lua_state, lua_entity_set_index);
    lua_setfield(lua_state, -2, "__newindex");
    lua_setmetatable(lua_state, -2);
    lua_call(lua_state, 1, 0);
}

void LuaSystem::pre_tick()
{
    if(script != previous_script)
    {
        previous_script = script;
        script->run_script();
    }

    if(script == nullptr)
    {
        return;
    }

    

    lua_getglobal(lua_state, "preTick");

    //Check if the function exists
    if(lua_isnil(lua_state, -1))
    {
        return;
    }

    lua_call(lua_state, 0, 0);
}

void LuaSystem::post_tick() {
    if(script != previous_script)
    {
        previous_script = script;
        script->run_script();
    }

    if(script == nullptr)
    {
        return;
    }

    

    lua_getglobal(lua_state, "postTick");

    //Check if the function exists
    if(lua_isnil(lua_state, -1))
    {
        return;
    }

    lua_call(lua_state, 0, 0);
}

void LuaSystem::pre_draw() {
    if(script != previous_script)
    {
        previous_script = script;
        script->run_script();
    }

    if(script == nullptr)
    {
        return;
    }

    

    lua_getglobal(lua_state, "preDraw");

    //Check if the function exists
    if(lua_isnil(lua_state, -1))
    {
        return;
    }

    lua_call(lua_state, 0, 0);
}

void LuaSystem::post_draw() {
    if(script != previous_script)
    {
        previous_script = script;
        script->run_script();
    }

    if(script == nullptr)
    {
        return;
    }

    

    lua_getglobal(lua_state, "postDraw");

    //Check if the function exists
    if(lua_isnil(lua_state, -1))
    {
        return;
    }

    lua_call(lua_state, 0, 0);
}

LuaSystem::LuaSystem()
{
    script = nullptr;
}