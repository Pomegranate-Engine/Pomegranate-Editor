template <typename T> T LuaComponentScript::get(std::string name)
{
    return std::any_cast<T>(component_data[name]);
}