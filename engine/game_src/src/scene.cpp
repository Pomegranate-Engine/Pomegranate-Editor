#include "scene.h"

#ifdef __WINDOWS__
#include <windows.h>
#endif
#include <Pomegranate/audio.h>
#include <Pomegranate/ttf_font.h>
std::vector<EntityGroup*> get_all_groups(EntityGroup* group)
{
    std::vector<EntityGroup*> groups;
    groups.push_back(group);
    for (auto& child : *group->get_child_groups())
    {
        auto child_groups = get_all_groups(child);
        groups.insert(groups.end(), child_groups.begin(), child_groups.end());
    }
    return groups;
}
std::vector<Entity*> get_all_entities(EntityGroup* group)
{
    std::vector<Entity*> entities;
    for (auto& entity : *group->get_entities())
    {
        entities.push_back(entity);
    }
    for (auto& child : *group->get_child_groups())
    {
        auto child_entities = get_all_entities(child);
        entities.insert(entities.end(), child_entities.begin(), child_entities.end());
    }
    return entities;
}
std::vector<std::pair<System*,uint32_t>> get_all_systems(EntityGroup* group)
{
    std::vector<std::pair<System*,uint32_t>> systems;
    for (auto& system : *group->get_systems())
    {
        systems.push_back(std::make_pair(system,group->id));
    }
    for (auto& child : *group->get_child_groups())
    {
        auto child_systems = get_all_systems(child);
        systems.insert(systems.end(), child_systems.begin(), child_systems.end());
    }
    return systems;
}
json save_scene_as_json(EntityGroup* scene)
{
    json j;
    //Get all groups
    auto groups = get_all_groups(scene);
    //Write groups
    j["groups"] = json::object();
    for (auto& group : groups)
    {
        j["groups"][std::to_string(group->id)] = json::object();
        j["groups"][std::to_string(group->id)]["name"] = group->name;
        if(group->get_parent() != nullptr)
            j["groups"][std::to_string(group->id)]["parent"] = group->get_parent()->id;
    }
    //Get all entities
    auto entities = get_all_entities(scene);
    //Write entities
    j["entities"] = json::object();
    for (auto& entity : entities)
    {
        j["entities"][std::to_string(entity->id)] = json::object();
        j["entities"][std::to_string(entity->id)]["name"] = entity->name;
        //write linked parents
        j["entities"][std::to_string(entity->id)]["parents"] = json::array();
        auto parents = entity->get_parent_groups();
        for (auto& parent : parents)
        {
            j["entities"][std::to_string(entity->id)]["parents"].push_back(parent->id);
        }
        j["entities"][std::to_string(entity->id)]["components"] = json::object();
        auto components = entity->get_components();
        for (auto& [type, component] : components)
        {
            j["entities"][std::to_string(entity->id)]["components"][type->name()] = json::object();
            std::unordered_map<std::string, std::pair<const std::type_info*, void*>> data = component->component_data;
            //Write component data
            for (auto& [name, data] : data)
            {
                if (data.second == nullptr)
                {
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["type"] = "null";
                }
                else if (data.first == &typeid(int))
                {

                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["type"] = "int";
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["value"] = *(int*)data.second;
                }
                else if (data.first == &typeid(float))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["type"] = "float";
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["value"] = *(float*)data.second;
                }
                else if (data.first == &typeid(double))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["type"] = "double";
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["value"] = *(double*)data.second;
                }
                else if (data.first == &typeid(bool))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["type"] = "bool";
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["value"] = *(bool*)data.second;
                }
                else if (data.first == &typeid(std::string))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["type"] = "string";
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["value"] = *(std::string*)data.second;
                }
                else if (data.first == &typeid(Vec2))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["type"] = "vec2";
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["x"] = (*(Vec2*)data.second).x;
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["y"] = (*(Vec2*)data.second).y;
                }
                else if (data.first == &typeid(Vec3))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["type"] = "vec3";
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["x"] = (*(Vec3*)data.second).x;
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["y"] = (*(Vec3*)data.second).y;
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["z"] = (*(Vec3*)data.second).z;
                }
                else if (data.first == &typeid(Color))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["type"] = "color";
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["r"] = (*(Color*)data.second).r;
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["g"] = (*(Color*)data.second).g;
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["b"] = (*(Color*)data.second).b;
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["a"] = (*(Color*)data.second).a;
                }
                else if (data.first == &typeid(Texture*))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["type"] = "texture";
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["path"] = (*(Texture**)data.second)->path;
                }
                else if (data.first == &typeid(TTFFont*))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["type"] = "ttffont";
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["path"] = (*(TTFFont**)data.second)->path;
                }
                else if (data.first == &typeid(Audio*))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["type"] = "audio";
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["path"] = (*(Audio**)data.second)->path;
                }
                else if(data.first == &typeid(Entity*))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["type"] = "entity";
                    if(*(Entity**)data.second != nullptr)
                        j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["value"] = (*(Entity**)data.second)->id;
                    else
                        j["entities"][std::to_string(entity->id)]["components"][type->name()][name]["value"] = -1;
                }
            }
        }
    }

    //Get all systems
    auto systems = get_all_systems(scene);
    //Write systems
    j["systems"] = json::object();
    for (auto& [system, linked] : systems)
    {
        j["systems"][typeid(*system).name()] = json::object();
        j["systems"][typeid(*system).name()]["linked"] = json::array();
        j["systems"][typeid(*system).name()]["linked"].push_back(linked);
    }
    return j;
}
void save_scene(const char* path, EntityGroup* scene)
{
    std::ofstream file(path);
    if (file.is_open())
    {
        json j = save_scene_as_json(scene);
        file << j.dump(4);
    }
    file.close();
}

void unload_all()
{

    //Delete entities
    for (auto& entity : Entity::entities)
    {
        delete entity.second;
    }
    //Delete groups
    for (auto& group : EntityGroup::groups)
    {
        delete group.second;
    }
    //TODO: For some reason can't delete entities or groups, causes crash

    Entity::entities.clear();
    EntityGroup::groups_id.clear();
    EntityGroup::groups.clear();
    EntityGroup::group_count = 0;
    Entity::entity_count = 0;
}

EntityGroup* open_scene_from_json(json data)
{
    int id_append_entity = Entity::entity_count;
    int id_append_group = EntityGroup::group_count;
    //Load groups
    for (auto& [id, group] : data["groups"].items())
    {
        EntityGroup* g = new EntityGroup(group["name"].get<std::string>());
        print_info("Created group: " + g->name);
    }
    //Link the groups together
    for (auto& [id, group] : data["groups"].items())
    {
        //Parse id as uint32_t
        uint32_t d = std::stoul(id) + id_append_group;
        EntityGroup* g = EntityGroup::groups_id[d];
        if (group.contains("parent"))
        {
            uint32_t parent_id = group["parent"].get<uint32_t>();
            EntityGroup* parent = EntityGroup::groups_id[parent_id];
            parent->add_group(g);
        }
    }

    //Load entities
    for (auto& [id, entity] : data["entities"].items())
    {
        Entity* e = new Entity();
        e->name = entity["name"].get<std::string>();
        for (auto& parent : entity["parents"])
        {
            EntityGroup::groups_id[parent.get<uint32_t>() + id_append_group]->add_entity(e);
        }
    }
    //Load components
    for (auto& [id, entity] : data["entities"].items()) {
        Entity *e = Entity::entities[std::stoi(id) + id_append_entity];
        for (auto& [type, component] : entity["components"].items())
        {
            if(!e->has_component(type.c_str()))
            {
                e->add_component(type.c_str());
            }
            Component* c = e->get_component(type.c_str());

            for (auto& [name, data] : component.items())
            {
                if (data["type"] == "int")
                {
                    *(int*)c->component_data[name].second = data["value"].get<int>();
                }
                else if (data["type"] == "float")
                {
                    *(float*)c->component_data[name].second = data["value"].get<float>();
                }
                else if (data["type"] == "double")
                {
                    *(double*)c->component_data[name].second = data["value"].get<double>();
                }
                else if (data["type"] == "bool")
                {
                    *(bool*)c->component_data[name].second = data["value"].get<bool>();
                }
                else if (data["type"] == "string")
                {
                    *(std::string*)c->component_data[name].second = data["value"].get<std::string>();
                }
                else if (data["type"] == "vec2")
                {
                    *(Vec2*)c->component_data[name].second = Vec2(data["x"].get<float>(), data["y"].get<float>());
                }
                else if (data["type"] == "vec3")
                {
                    *(Vec3*)c->component_data[name].second = Vec3(data["x"].get<float>(), data["y"].get<float>(), data["z"].get<float>());
                }
                else if (data["type"] == "color")
                {
                    *(Color*)c->component_data[name].second = Color(data["r"].get<uint8_t>(), data["g"].get<uint8_t>(), data["b"].get<uint8_t>(), data["a"].get<uint8_t>());
                }
                else if (data["type"] == "texture")
                {
                    *(Texture**)c->component_data[name].second = ResourceManager::load<Texture>(data["path"].get<std::string>());
                }
                else if (data["type"] == "ttffont")
                {
                    *(TTFFont**)c->component_data[name].second = ResourceManager::load<TTFFont>(data["path"].get<std::string>());
                }
                else if (data["type"] == "audio")
                {
                    *(Audio**)c->component_data[name].second = ResourceManager::load<Audio>(data["path"].get<std::string>());
                }
                else if(data["type"] == "entity")
                {
                    if(data["value"].get<uint32_t>() != -1)
                        *(Entity**)c->component_data[name].second = Entity::entities[data["value"].get<uint32_t>() + id_append_entity];
                }
            }
        }
    }
    //Load systems
    for (auto& [type, system] : data["systems"].items())
    {
        System* s = System::system_types[type.c_str()]();
        for (auto& linked : system["linked"])
        {
            EntityGroup::groups_id[linked.get<uint32_t>() + id_append_group]->add_system(s);
        }
    }
    std::unordered_map<uint32_t,EntityGroup*> groups = EntityGroup::groups_id;
    std::unordered_map<std::string, EntityGroup*> groups_name = EntityGroup::groups;
    EntityGroup *root = EntityGroup::groups_id[0];
    return root;
}

EntityGroup *open_scene(const char *path)
{
    std::ifstream f(path);
    json data = json::parse(f);
    if (f.is_open())
    {
        return open_scene_from_json(data);
    }
    return nullptr;
}
