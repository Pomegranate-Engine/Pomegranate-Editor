#include "lua_wrapper.h"
namespace Pomegranate
{
    std::unordered_map<Component*,int> ref_map;

    void lua_push_vec2(Vec2* vec2,lua_State* l)
    {
        lua_newtable(l);
        lua_pushstring(l, "x");
        lua_pushnumber(l, vec2->x);
        lua_settable(l, -3);
        lua_pushstring(l, "y");
        lua_pushnumber(l, vec2->y);
        lua_settable(l, -3);
    }

    Vec2 lua_get_vec2(lua_State* l, int idx)
    {
        Vec2 vec2;
        lua_pushstring(l, "x");
        lua_gettable(l, idx);
        vec2.x = (float)lua_tonumber(l, -1);
        lua_pushstring(l, "y");
        lua_gettable(l, idx);
        vec2.y = (float)lua_tonumber(l, -1);
        return vec2;
    }

    Color lua_get_color(lua_State* l, int idx)
    {
        Color color;
        lua_pushstring(l, "r");
        lua_gettable(l, idx);
        color.r = (uint8_t)lua_tonumber(l, -1);
        lua_pushstring(l, "g");
        lua_gettable(l, idx);
        color.g = (uint8_t)lua_tonumber(l, -1);
        lua_pushstring(l, "b");
        lua_gettable(l, idx);
        color.b = (uint8_t)lua_tonumber(l, -1);
        lua_pushstring(l, "a");
        lua_gettable(l, idx);
        color.a = (uint8_t)lua_tonumber(l, -1);
        return color;
    }

    void lua_push_color(Color* color, lua_State* l)
    {
        lua_newtable(l);
        lua_pushstring(l, "r");
        lua_pushnumber(l, color->r);
        lua_settable(l, -3);
        lua_pushstring(l, "g");
        lua_pushnumber(l, color->g);
        lua_settable(l, -3);
        lua_pushstring(l, "b");
        lua_pushnumber(l, color->b);
        lua_settable(l, -3);
        lua_pushstring(l, "a");
        lua_pushnumber(l, color->a);
        lua_settable(l, -3);
    }

    void lua_push_component(Component* component,lua_State* l)
    {
        if(ref_map.find(component) != ref_map.end())
        {
            lua_rawgeti(l, LUA_REGISTRYINDEX, ref_map[component]);
            return;
        }
        //Create a table
        lua_newtable(l);
        for (const auto& d : component->component_data)
        {
            //Push the name of the variable
            lua_pushstring(l, d.first.c_str());
            //Push the value of the variable
            if (d.second.first == &typeid(int))
            {
                lua_pushnumber(l, *(int*)d.second.second);
            }
            else if (d.second.first == &typeid(float))
            {
                lua_pushnumber(l, *(float*)d.second.second);
            }
            else if (d.second.first == &typeid(double))
            {
                lua_pushnumber(l, *(double*)d.second.second);
            }
            else if (d.second.first == &typeid(bool))
            {
                lua_pushboolean(l, *(bool*)d.second.second);
            }
            else if (d.second.first == &typeid(std::string))
            {
                lua_pushstring(l, ((std::string*)d.second.second)->c_str());
            }
            else if(d.second.first == &typeid(Vec2) || d.second.first == &typeid(Vec2i)) //TODO: These should be pushed separately
            {
                lua_push_vec2((Vec2*)d.second.second, l);
            }
            else if(d.second.first == &typeid(Color))
            {
                lua_push_color((Color*)d.second.second, l);
            }
            else
            {
                lua_pushnil(l);
            }
            //Set the value of the variable
            lua_settable(l, -3);
        }
        int ref = luaL_ref(l, LUA_REGISTRYINDEX);
        ref_map[component] = ref;
        lua_rawgeti(l, LUA_REGISTRYINDEX, ref);
    }

    int lua_get_component(lua_State* l)
    {
        double d = lua_tonumber(l, 1);
        Entity* e = Entity::entities[(int)d];
        const char* name = lua_tostring(l, 2);

        if(LuaComponent::lua_component_types.find(std::string(name)) != LuaComponent::lua_component_types.end())
        {
            Component *c = e->get_component<LuaComponent>(name);
            lua_push_component(c, l);
        }
        else
        {
            Component *c = e->get_component(name);
            lua_push_component(c, l);
        }
        return 1;
    }

    int lua_has_component(lua_State* l)
    {
        double d = lua_tonumber(l, 1);
        Entity* e = Entity::entities[(int)d];
        const char* name = lua_tostring(l, 2);
        if(LuaComponent::lua_component_types.find(std::string(name)) != LuaComponent::lua_component_types.end())
        {
            bool has = e->has_component<LuaComponent>(name);
            lua_pushboolean(l, has);
        }
        else
        {
            bool has = e->has_component(name);
            lua_pushboolean(l, has);
        }
        return 1;
    }

    int lua_require_component(lua_State* l)
    {
        double d = lua_tonumber(l, 1);
        Entity* e = Entity::entities[(int)d];
        const char* name = lua_tostring(l, 2);
        bool has;
        if(LuaComponent::lua_component_types.find(std::string(name)) != LuaComponent::lua_component_types.end())
        {
            has = e->has_component<LuaComponent>(name);
        }
        else
        {
            has = e->has_component(name);
        }

        if(!has)
        {
            if(LuaComponent::lua_component_types.find(std::string(name)) != LuaComponent::lua_component_types.end())
            {
                e->add_component<LuaComponent>(name);
            }
            else
            {
                e->add_component(name);
            }
        }
        return 0;
    }

    int lua_get_key(lua_State* l)
    {
        const char* name = lua_tostring(l, 1);
        auto key = static_cast<SDL_KeyCode>(SDL_GetKeyFromName(name));
        lua_pushboolean(l, InputManager::get_key(key));
        return 1;
    }

    int lua_get_axis(lua_State* l)
    {
        const char* name1 = lua_tostring(l, 1);
        auto key1 = static_cast<SDL_Scancode>(SDL_GetScancodeFromName(name1));
        const char* name2 = lua_tostring(l, 2);
        auto key2 = static_cast<SDL_Scancode>(SDL_GetScancodeFromName(name2));
        float axis = 0;
        if(InputManager::get_key(key1))
        {
            axis -= 1;
        }
        if(InputManager::get_key(key2))
        {
            axis += 1;
        }
        lua_pushnumber(l, axis);
        return 1;
    }

    int lua_get_mouse(lua_State* l)
    {
        int i = (int)lua_tonumber(l, 1);
        lua_pushboolean(l, InputManager::get_mouse_button(i));
        return 1;
    }

    int lua_get_mouse_pos(lua_State* l)
    {
        Vec2 pos = InputManager::get_mouse_position();
        lua_push_vec2(&pos, l);
        return 1;
    }

    int lia_print_pass(lua_State* l)
    {
        const char* str = lua_tostring(l, 1);
        print_pass(str);
        return 0;
    }

    int lia_print_fail(lua_State* l)
    {
        const char* str = lua_tostring(l, 1);
        print_fail(str);
        return 0;
    }

    int lia_print_error(lua_State* l)
    {
        const char* str = lua_tostring(l, 1);
        print_error(str);
        return 0;
    }

    int lia_print_warn(lua_State* l)
    {
        const char* str = lua_tostring(l, 1);
        print_warn(str);
        return 0;
    }

    int lia_print_info(lua_State* l)
    {
        const char* str = lua_tostring(l, 1);
        print_info(str);
        return 0;
    }

    int lia_print_debug(lua_State* l)
    {
        const char* str = lua_tostring(l, 1);
        print_debug(str);
        return 0;
    }

    int lia_print_notice(lua_State* l)
    {
        const char* str = lua_tostring(l, 1);
        print_notice(str);
        return 0;
    }

    int lia_print_log(lua_State* l)
    {
        const char* str = lua_tostring(l, 1);
        print_log(str);
        return 0;
    }

    int lia_print_ready(lua_State* l)
    {
        const char* str = lua_tostring(l, 1);
        print_ready(str);
        return 0;
    }

    int lia_print_assert(lua_State* l)
    {
        const char* str = lua_tostring(l, 1);
        print_assert(str);
        return 0;
    }

    int lua_draw_pixel(lua_State* l)
    {
        Vec2 p = lua_get_vec2(l, 1);
        Color c = lua_get_color(l, 2);
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), c.r, c.g, c.b, c.a);
        SDL_RenderPoint(Window::current->get_sdl_renderer(), p.x, p.y);
        return 0;
    }

    int lua_register_component(lua_State* l)
    {
        const char* name = lua_tostring(l, 1);
        // Check if the second argument is a table
        if (lua_istable(l, 2)) {
            // Iterate through the table
            lua_pushnil(l);  // Push a nil key to start the iteration
            while (lua_next(l, 2) != 0) {
                // Key is at index -2, value is at index -1
                const char* key = lua_tostring(l, -2);

                // Process the key-value pair (replace this with your logic)
                if(lua_isnumber(l,-1))
                {
                    LuaComponent::current->push_data<double>(key, new double(lua_tonumber(l, -1)));
                }
                else if(lua_isstring(l,-1))
                {
                    LuaComponent::current->push_data<std::string>(key, new std::string(lua_tostring(l, -1)));
                }
                else if(lua_isboolean(l,-1))
                {
                    LuaComponent::current->push_data<bool>(key, new bool(lua_toboolean(l, -1)));
                }
                else if(lua_istable(l,-1))
                {
                    //TODO: Add support for vec2s and stuff
                }
                else
                {
                    print_error("Unknown type");
                }

                // Pop the value, leaving the key on top for the next iteration
                lua_pop(l, 1);
            }
        }

        //Debug all current component data
        //region Debug
        /*
            for (auto d : LuaComponent::current->component_data)
            {
                print_debug(d.first);
                if (d.second.first == &typeid(int))
                {
                    print_debug(std::to_string(*(int*)d.second.second));
                }
                else if (d.second.first == &typeid(float))
                {
                    print_debug(std::to_string(*(float*)d.second.second));
                }
                else if (d.second.first == &typeid(double))
                {
                    print_debug(std::to_string(*(double*)d.second.second));
                }
                else if (d.second.first == &typeid(bool))
                {
                    print_debug(std::to_string(*(bool*)d.second.second));
                }
                else if (d.second.first == &typeid(std::string))
                {
                    print_debug(*(std::string*)d.second.second);
                }
                else if(d.second.first == &typeid(Vec2))
                {
                    Vec2* vec2 = (Vec2*)d.second.second;
                    print_debug(std::to_string(vec2->x));
                    print_debug(std::to_string(vec2->y));
                }
                else if(d.second.first == &typeid(Vec2i*))
                {
                    Vec2i* vec2 = (Vec2i*)d.second.second;
                    print_debug(std::to_string(vec2->x));
                    print_debug(std::to_string(vec2->y));
                }
                else if(d.second.first == &typeid(Color*))
                {
                    Color* color = (Color*)d.second.second;
                    print_debug(std::to_string(color->r));
                    print_debug(std::to_string(color->g));
                    print_debug(std::to_string(color->b));
                    print_debug(std::to_string(color->a));
                }
                else
                {
                    print_error("Unknown type");
                }
            }
        */
        //endregion

        LuaComponent::lua_component_types[name] = 0;
        return 0;
    }

    void clean_refs(lua_State* l)
    {
        for(auto & ref : ref_map)
        {
            Component* c = ref.first;
            for (const auto& d : c->component_data)
            {
                lua_rawgeti(l, LUA_REGISTRYINDEX, ref.second);
                //Push the name of the variable
                lua_pushstring(l, d.first.c_str());
                //Get the variable
                lua_gettable(l, -2);
                //Check type
                if(d.second.first == &typeid(int))
                {
                    *(int*)d.second.second = (int)lua_tonumber(l, -1);
                }
                else if(d.second.first == &typeid(float))
                {
                    *(float*)d.second.second = (float)lua_tonumber(l, -1);
                }
                else if(d.second.first == &typeid(double))
                {
                    *(double*)d.second.second = (double)lua_tonumber(l, -1);
                }
                else if(d.second.first == &typeid(bool))
                {
                    *(bool*)d.second.second = (bool)lua_toboolean(l, -1);
                }
                else if(d.second.first == &typeid(std::string))
                {
                    *(std::string*)d.second.second = (std::string)lua_tostring(l, -1);
                }
                else if(d.second.first == &typeid(Vec2))
                {
                    auto* vec2 = (Vec2*)d.second.second;
                    lua_pushstring(l, "x");
                    lua_gettable(l, -2);
                    vec2->x = (float)lua_tonumber(l, -1);
                    lua_pushstring(l, "y");
                    lua_gettable(l, -3);
                    vec2->y = (float)lua_tonumber(l, -1);
                }
                else if(d.second.first == &typeid(Vec2i))
                {
                    auto* vec2 = (Vec2i*)d.second.second;
                    lua_pushstring(l, "x");
                    lua_gettable(l, -2);
                    vec2->x = (int)lua_tonumber(l, -1);
                    lua_pushstring(l, "y");
                    lua_gettable(l, -3);
                    vec2->y = (int)lua_tonumber(l, -1);
                }
                else if(d.second.first == &typeid(Color))
                {
                    auto* color = (Color*)d.second.second;
                    lua_pushstring(l, "r");
                    lua_gettable(l, -2);
                    color->r = (int)lua_tonumber(l, -1);
                    lua_pushstring(l, "g");
                    lua_gettable(l, -3);
                    color->g = (int)lua_tonumber(l, -1);
                    lua_pushstring(l, "b");
                    lua_gettable(l, -4);
                    color->b = (int)lua_tonumber(l, -1);
                    lua_pushstring(l, "a");
                    lua_gettable(l, -5);
                    color->a = (int)lua_tonumber(l, -1);
                }
                else
                {
                    print_error("Unknown type: " + d.first + " " + std::string(d.second.first->name()));
                }
                lua_pop(l, 1);
            }
            luaL_unref(l, LUA_REGISTRYINDEX, ref.second);

        }
        ref_map.clear();
    }

    void LuaSystem::init(Entity *entity)
    {

    }

    void LuaSystem::tick(Entity *entity)
    {
        if (!this->loaded) return;

        lua_State *l = this->state;
        lua_pushnumber(l, 0.016);
        lua_setglobal(l, "delta_time");

        lua_getglobal(l, "tick");
        if (!lua_isfunction(l, -1)) return;

        // Push entity as argument
        lua_pushnumber(l, (double)entity->get_id());

        int arguments = 1;  // Number of arguments pushed onto the stack

        int res = lua_pcall(l, arguments, 0, 0);

        // Print error if there is one
        if (res != LUA_OK)
        {
            print_error(std::string("Lua Error: ") + lua_tostring(l, -1));
        }

        // Clear the stack
        lua_pop(l, lua_gettop(l));

        // Clean up references
        clean_refs(l);
    }


    void LuaSystem::draw(Entity *entity)
    {
        if (!this->loaded) return;

        lua_State *l = this->state;
        lua_pushnumber(l, delta_time);
        lua_setglobal(l, "delta_time");

        lua_getglobal(l, "draw");
        if (!lua_isfunction(l, -1)) return;

        // Push entity as argument
        lua_pushnumber(l, (double)entity->get_id());

        int arguments = 1;  // Number of arguments pushed onto the stack

        int res = lua_pcall(l, arguments, 0, 0);

        // Print error if there is one
        if (res != LUA_OK)
        {
            print_error(std::string("Lua Error: ") + lua_tostring(l, -1));
        }

        // Clear the stack
        lua_pop(l, lua_gettop(l));

        // Clean up references
        clean_refs(l);
    }

    void LuaSystem::pre_tick()
    {
        if (!this->loaded) return;

        lua_State *l = this->state;
        lua_pushnumber(l, 0.016);
        lua_setglobal(l, "delta_time");

        lua_getglobal(l, "pre_tick");
        if (!lua_isfunction(l, -1)) return;

        int arguments = 0;  // Number of arguments pushed onto the stack

        int res = lua_pcall(l, arguments, 0, 0);

        // Print error if there is one
        if (res != LUA_OK)
        {
            print_error(std::string("Lua Error: ") + lua_tostring(l, -1));
        }

        // Clear the stack
        lua_pop(l, lua_gettop(l));

        // Clean up references
        clean_refs(l);
    }

    void LuaSystem::post_tick()
    {
        if (!this->loaded) return;

        lua_State *l = this->state;
        lua_pushnumber(l, 0.016);
        lua_setglobal(l, "delta_time");

        lua_getglobal(l, "post_tick");
        if (!lua_isfunction(l, -1)) return;

        int arguments = 0;  // Number of arguments pushed onto the stack

        int res = lua_pcall(l, arguments, 0, 0);

        // Print error if there is one
        if (res != LUA_OK)
        {
            print_error(std::string("Lua Error: ") + lua_tostring(l, -1));
        }

        // Clear the stack
        lua_pop(l, lua_gettop(l));

        // Clean up references
        clean_refs(l);
    }

    void LuaSystem::pre_draw()
    {
        if (!this->loaded) return;

        lua_State *l = this->state;
        lua_pushnumber(l, delta_time);
        lua_setglobal(l, "delta_time");

        lua_getglobal(l, "pre_draw");
        if (!lua_isfunction(l, -1)) return;

        int arguments = 0;  // Number of arguments pushed onto the stack

        int res = lua_pcall(l, arguments, 0, 0);

        // Print error if there is one
        if (res != LUA_OK)
        {
            print_error(std::string("Lua Error: ") + lua_tostring(l, -1));
        }

        // Clear the stack
        lua_pop(l, lua_gettop(l));

        // Clean up references
        clean_refs(l);
    }

    void LuaSystem::post_draw()
    {
        if (!this->loaded) return;

        lua_State *l = this->state;
        lua_pushnumber(l, delta_time);
        lua_setglobal(l, "delta_time");

        lua_getglobal(l, "post_draw");
        if (!lua_isfunction(l, -1)) return;

        int arguments = 0;  // Number of arguments pushed onto the stack

        int res = lua_pcall(l, arguments, 0, 0);

        // Print error if there is one
        if (res != LUA_OK)
        {
            print_error(std::string("Lua Error: ") + lua_tostring(l, -1));
        }

        // Clear the stack
        lua_pop(l, lua_gettop(l));

        // Clean up references
        clean_refs(l);
    }

    void add_wrapper_functions(lua_State* l)
    {
        //Add functions
        lua_register(l, "require_component", lua_require_component);
        lua_register(l, "register_component", lua_register_component);
        lua_register(l, "get_component", lua_get_component);
        lua_register(l, "has_component", lua_has_component);
        lua_register(l, "input_get_key", lua_get_key);
        lua_register(l, "input_get_axis", lua_get_axis);
        lua_register(l, "input_get_mouse", lua_get_mouse);
        lua_register(l, "input_get_mouse_pos", lua_get_mouse_pos);
        lua_register(l, "print_pass", lia_print_pass);
        lua_register(l, "print_fail", lia_print_fail);
        lua_register(l, "print_error", lia_print_error);
        lua_register(l, "print_warn", lia_print_warn);
        lua_register(l, "print_info", lia_print_info);
        lua_register(l, "print_debug", lia_print_debug);
        lua_register(l, "print_notice", lia_print_notice);
        lua_register(l, "print_log", lia_print_log);
        lua_register(l, "print_ready", lia_print_ready);
        lua_register(l, "print_assert", lia_print_assert);
        lua_register(l, "draw_pixel", lua_draw_pixel);

        //Add constants
        //Physics constants
        lua_pushnumber(l, 0);
        lua_setglobal(l, "PHYSICS_BODY_TYPE_KINEMATIC");
        lua_pushnumber(l, 1);
        lua_setglobal(l, "PHYSICS_BODY_TYPE_RIGID");
        lua_pushnumber(l, 2);
        lua_setglobal(l, "PHYSICS_BODY_TYPE_STATIC");
        lua_pushnumber(l, 0);
        lua_setglobal(l, "COLLISION_SHAPE_TYPE_RECTANGLE");
        lua_pushnumber(l, 1);
        lua_setglobal(l, "COLLISION_SHAPE_TYPE_CIRCLE");
        //Time
        lua_pushnumber(l, delta_time);
        lua_setglobal(l, "delta_time");
    }

    LuaSystem::LuaSystem()
    {
        this->state = luaL_newstate();
        luaL_openlibs(state);
        add_wrapper_functions(state);
    }

    void LuaSystem::load_script(const char *path)
    {
        luaL_dofile(this->state, path);
        this->loaded = true;
    }

    LuaComponent::LuaComponent()
    {
        this->real_type = 0;
        this->loaded = false;
        this->state = luaL_newstate();
        luaL_openlibs(this->state);
        add_wrapper_functions(state);
    }

    void LuaComponent::load_script(const char *path)
    {
        current = this;
        luaL_dofile(this->state, path);
        this->loaded = true;
    }

    void LuaComponent::init(Entity *entity)
    {
        if (!this->loaded) return;

        lua_State *l = this->state;
        lua_getglobal(l, "init");
        if (!lua_isfunction(l, -1)) return;

        // Push entity as argument
        lua_pushnumber(l, (double)entity->get_id());

        int arguments = 1;  // Number of arguments pushed onto the stack

        int res = lua_pcall(l, arguments, 0, 0);

        // Print error if there is one
        if (res != LUA_OK)
        {
            print_error(std::string("Lua Error: ") + lua_tostring(l, -1));
        }

        // Clear the stack
        lua_pop(l, lua_gettop(l));

        // Clean up references
        clean_refs(l);

    }

    LuaComponent::~LuaComponent()
    {
        lua_close(this->state);
    }
}