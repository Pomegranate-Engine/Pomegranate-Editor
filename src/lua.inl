template <typename T> T LuaComponentData::get(std::string name)
{
    return std::any_cast<T>(component_data[name]);
}

template <typename T> void LuaComponentData::set(std::string name, void* value)
{
    component_data[name] = {&typeid(T),value};
}