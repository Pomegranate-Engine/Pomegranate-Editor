#include "lua.h"

LuaComponentScript::LuaComponentScript(std::string path)
{
    print_info("Loading Lua script: " + path);
    this->path = path;
}

void LuaComponentScript::run_script()
{
    L = sol::state();
    sol::state* sol_L = &L;
    sol_L->open_libraries();
    luaopen_pomegranate(sol_L);

    try
    {
        sol_L->safe_script_file(path);
    }
    catch(sol::error e)
    {
        print_error(e.what());
    }

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

LuaSystemScript::LuaSystemScript(std::string path)
{
    print_info("Loading Lua script: " + path);
    this->path = path;
}

LuaSystemScript::~LuaSystemScript()
{
    L.collect_garbage();
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

int luaopen_pomegranate(sol::state *L)
{
    (*L)["print_info"] = [](std::string s){print_info(s);};
    (*L)["print_error"] = [](std::string s){print_error(s);};
    (*L)["print_warn"] = [](std::string s){print_warn(s);};
    (*L)["print_debug"] = [](std::string s){print_debug(s);};
    (*L)["print_assert"] = [](std::string s){print_assert(s);};
    (*L)["print_pass"] = [](std::string s){print_pass(s);};
    (*L)["print_fail"] = [](std::string s){print_fail(s);};
    (*L)["print_ready"] = [](std::string s){print_ready(s);};
    (*L)["print_log"] = [](std::string s){print_log(s);};
    (*L)["print_notice"] = [](std::string s){print_notice(s);};

    L->new_usertype<Vec2>("Vec2", sol::constructors<Vec2(float, float),Vec2()>(),
                          "x", sol::property(&Vec2::x,&Vec2::x),
                          "y", sol::property(&Vec2::y,&Vec2::y),
                          "normalize", &Vec2::normalized,
                          "magnitude", &Vec2::length,
                          "angle_to", &Vec2::angle_to,
                          "distance_to", &Vec2::distance_to,
                          "dot", &Vec2::dot,
                          "cross", &Vec2::cross,
                          "rotate", &Vec2::rotate,
                          "linear_interpolate", &Vec2::lerp
    );

    L->new_usertype<Vec3>("Vec3", sol::constructors<Vec3(float, float, float),Vec3()>(),
                          "x", sol::property(&Vec3::x,&Vec3::x),
                          "y", sol::property(&Vec3::y,&Vec3::y),
                          "z", sol::property(&Vec3::z,&Vec3::z),
                          "normalize", &Vec3::normalized,
                          "magnitude", &Vec3::length,
                          "angle_to", &Vec3::angle_to,
                          "distance_to", &Vec3::distance_to,
                          "dot", &Vec3::dot,
                          "cross", &Vec3::cross,
                          "rotate", &Vec3::rotate,
                          "linear_interpolate", &Vec3::lerp
    );

    L->new_usertype<Color>("Color", sol::constructors<Color(float, float, float, float),Color()>(),
                           "r", sol::property(&Color::r,&Color::r),
                           "g", sol::property(&Color::g,&Color::g),
                           "b", sol::property(&Color::b,&Color::b),
                           "a", sol::property(&Color::a,&Color::a)
    );
    return 1;
}

void LuaSystemScript::run_script()
{
    L = sol::state();
    L.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table, sol::lib::string, sol::lib::math, sol::lib::os, sol::lib::io);
    luaopen_pomegranate(&L);

    try
    {
        L.safe_script_file(path);
    }
    catch(sol::error e)
    {
        print_error(e.what());
    }

    print_info("Lua script loaded: " + path);
}