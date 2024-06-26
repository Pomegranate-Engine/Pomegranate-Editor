#include "scene.h"

#include <Pomegranate/audio.h>
#include <Pomegranate/ttf_font.h>

#include "entity_hierarchy_window.h"

SceneGroup::SceneGroup(std::string name, std::string path) : Group(name)
{
    this->path = path;
}

void SceneGroup::instantiate()
{
    GroupRef scene = open_scene(path.c_str());
    add_group(scene);
    print_info("Instantiated scene: " + name);
}

std::vector<GroupRef> get_all_groups(GroupRef group)
{
	std::vector<GroupRef> groups;
	groups.push_back(group);
	for (auto& child : group->get_child_groups())
	{
		auto child_groups = get_all_groups(child);
		groups.insert(groups.end(), child_groups.begin(), child_groups.end());
	}
	return groups;
}
std::vector<EntityRef> get_all_entities(GroupRef group)
{
	std::vector<EntityRef> entities;
	for (auto& entity : group->get_entities())
	{
		entities.push_back(entity);
	}
	for (auto& child : group->get_child_groups())
	{
		auto child_entities = get_all_entities(child);
		entities.insert(entities.end(), child_entities.begin(), child_entities.end());
	}
	return entities;
}
std::vector<std::pair<SystemRef,uint32_t>> get_all_systems(GroupRef group)
{
	std::vector<std::pair<SystemRef,uint32_t>> systems;
	for (auto& system : group->get_systems())
	{
		systems.push_back(std::make_pair(system,group->id));
	}
	for (auto& child : group->get_child_groups())
	{
		auto child_systems = get_all_systems(child);
		systems.insert(systems.end(), child_systems.begin(), child_systems.end());
	}
	return systems;
}
std::vector<GroupRef> get_all_parents(GroupRef group)
{
    if(group == nullptr)
        return std::vector<GroupRef>();
    GroupRef parent = group->get_parent();
    std::vector<GroupRef> parents;
    if(parent != nullptr)
    {
        parents.push_back(parent);
        //Recursively get parents
        auto parent_parents = get_all_parents(parent);
        parents.insert(parents.end(), parent_parents.begin(), parent_parents.end());
    }
    return parents;
}
std::vector<GroupRef> get_all_parents(EntityRef entity)
{
    std::vector<GroupRef> parents;
    for (auto& parent : entity->get_parent_groups())
    {
        parents.push_back(parent);
        //Recursively get parents
        auto parent_parents = get_all_parents(parent);
        parents.insert(parents.end(), parent_parents.begin(), parent_parents.end());
    }
    return parents;
}
json save_scene_as_json(GroupRef scene)
{
    json j;
    //Get all groups
    auto groups = get_all_groups(scene);
    //Write groups
    j["groups"] = json::object();
    for (auto& group : groups)
    {
        //Make sure it's not a scene group
        bool skip = false;
        auto parents = get_all_parents(group);
        for (auto& parent : parents)
        {
            if(typeid(*parent.get()) == typeid(SceneGroup))
                skip = true;
        }
        if(skip)
        {
            print_info("Skipping group: " + group->name + " because it is in a scene group");
            continue;
        }

        j["groups"][std::to_string(group->id)] = json::object();
        j["groups"][std::to_string(group->id)]["name"] = group->name;
        if(typeid(*group.get()) == typeid(AutoGroup))
        {
            j["groups"][std::to_string(group->id)]["type"] = "auto";
            j["groups"][std::to_string(group->id)]["components"] = json::array();
            for(auto& component : ((AutoGroup*)group.get())->component_types)
            {
                j["groups"][std::to_string(group->id)]["components"].push_back(scuffy_demangle(component->name()));
            }
        }
        else if(typeid(*group.get()) == typeid(SceneGroup))
        {
            j["groups"][std::to_string(group->id)]["type"] = "scene";
            j["groups"][std::to_string(group->id)]["path"] = ((SceneGroup*)group.get())->path;
        }
        else
        {
            j["groups"][std::to_string(group->id)]["type"] = "normal";
        }
        if(group->get_parent() != nullptr)
        {
            j["groups"][std::to_string(group->id)]["parent"] = group->get_parent()->id;
        }
    }
    //Get all entities
    auto entities = get_all_entities(scene);
    //Write entities
    j["entities"] = json::object();
    for (auto& entity : entities)
    {
        auto parents = get_all_parents(entity);
        bool skip = false;
        for (auto& parent : parents)
        {
            if(typeid(*parent.get()) == typeid(SceneGroup))
                skip = true;
        }
        if(skip)
        {
            print_info("Skipping entity: " + entity->name + " because it is in a scene group");
            continue;
        }
        j["entities"][std::to_string(entity->id)] = json::object();
        j["entities"][std::to_string(entity->id)]["name"] = entity->name;
        //write linked parents
        j["entities"][std::to_string(entity->id)]["parents"] = json::array();
        parents = entity->get_parent_groups();
        for (auto& parent : parents)
        {
            j["entities"][std::to_string(entity->id)]["parents"].push_back(parent->id);
        }
        j["entities"][std::to_string(entity->id)]["components"] = json::object();
        auto components = entity->get_components();
        for (auto& [type, component] : components)
        {
            std::string type_name = scuffy_demangle(type->name());
            j["entities"][std::to_string(entity->id)]["components"][type_name] = json::object();
            std::map<std::string, std::pair<const std::type_info*, void*>> data = component->component_data;
            //Write component data
            for (auto& [name, data] : data)
            {
                if (data.second == nullptr)
                {
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["type"] = "null";
                }
                else if (data.first == &typeid(int))
                {

                    j["entities"][std::to_string(entity->id)]["components"][type_name][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["type"] = "int";
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["value"] = *(int*)data.second;
                }
                else if (data.first == &typeid(float))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["type"] = "float";
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["value"] = *(float*)data.second;
                }
                else if (data.first == &typeid(double))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["type"] = "double";
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["value"] = *(double*)data.second;
                }
                else if (data.first == &typeid(bool))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["type"] = "bool";
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["value"] = *(bool*)data.second;
                }
                else if (data.first == &typeid(std::string))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["type"] = "string";
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["value"] = *(std::string*)data.second;
                }
                else if (data.first == &typeid(Vec2))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["type"] = "vec2";
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["x"] = (*(Vec2*)data.second).x;
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["y"] = (*(Vec2*)data.second).y;
                }
                else if (data.first == &typeid(Vec3))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["type"] = "vec3";
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["x"] = (*(Vec3*)data.second).x;
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["y"] = (*(Vec3*)data.second).y;
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["z"] = (*(Vec3*)data.second).z;
                }
                else if (data.first == &typeid(Color))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["type"] = "color";
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["r"] = (*(Color*)data.second).r;
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["g"] = (*(Color*)data.second).g;
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["b"] = (*(Color*)data.second).b;
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["a"] = (*(Color*)data.second).a;
                }
                else if (data.first == &typeid(Texture*) && (*(Texture**)data.second) != nullptr)
                {
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["type"] = "texture";
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["path"] = (*(Texture**)data.second)->path;
                }
                else if (data.first == &typeid(TTFFont*))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["type"] = "ttffont";
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["path"] = (*(TTFFont**)data.second)->path;
                }
                else if (data.first == &typeid(Audio*))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["type"] = "audio";
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["path"] = (*(Audio**)data.second)->path;
                }
                else if(data.first == &typeid(Entity*))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["type"] = "entity";
                    if(*(Entity**)data.second != nullptr)
                        j["entities"][std::to_string(entity->id)]["components"][type_name][name]["value"] = (*(Entity**)data.second)->id;
                    else
                        j["entities"][std::to_string(entity->id)]["components"][type_name][name]["value"] = -1;
                }
                else if(data.first == &typeid(std::vector<LuaComponentData*>))
                {
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["type"] = "vector_lua";
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["value"] = json::object();
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["value"]["length"] = (*(std::vector<LuaComponentScript*>*)data.second).size();
                    j["entities"][std::to_string(entity->id)]["components"][type_name][name]["value"]["data"] = json::array();
                    for(auto& script : (*(std::vector<LuaComponentData*>*)data.second))
                    {
                        if(script != nullptr) {
                            j["entities"][std::to_string(
                                    entity->id)]["components"][type_name][name]["value"]["data"].push_back(
                                    json::object());
                            j["entities"][std::to_string(entity->id)]["components"][type_name][name]["value"]["data"].back()["path"] = script->component->path;
                            j["entities"][std::to_string(entity->id)]["components"][type_name][name]["value"]["data"].back()["data"] = json::object();
                            for (auto [key,value] : script->component_data)
                            {
                                if (value.first == &typeid(int)) {
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key] = json::object();
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["type"] = "int";
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["value"] = *(int*)value.second;
                                }
                                else if (value.first == &typeid(float)) {
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key] = json::object();
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["type"] = "float";
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["value"] = *(float*)value.second;
                                }
                                else if (value.first == &typeid(double)) {
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key] = json::object();
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["type"] = "double";
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["value"] = *(double*)value.second;
                                }
                                else if (value.first == &typeid(bool)) {
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key] = json::object();
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["type"] = "bool";
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["value"] = *(bool*)value.second;
                                }
                                else if (value.first == &typeid(std::string)) {
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key] = json::object();
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["type"] = "string";
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["value"] = *(std::string*)value.second;
                                }
                                else if (value.first == &typeid(Vec2)) {
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key] = json::object();
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["type"] = "vec2";
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["x"] = (*(Vec2*)value.second).x;
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["y"] = (*(Vec2*)value.second).y;
                                }
                                else if (value.first == &typeid(Vec3)) {
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key] = json::object();
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["type"] = "vec3";
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["x"] = (*(Vec3*)value.second).x;
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["y"] = (*(Vec3*)value.second).y;
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["z"] = (*(Vec3*)value.second).z;
                                }
                                else if (value.first == &typeid(Color)) {
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key] = json::object();
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["type"] = "color";
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["r"] = (*(Color*)value.second).r;
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["g"] = (*(Color*)value.second).g;
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["b"] = (*(Color*)value.second).b;
                                    j["entities"][std::to_string(
                                            entity->id)]["components"][type_name][name]["value"]["data"].back()["data"][key]["a"] = (*(Color*)value.second).a;
                                }
                            }
                        }
                        else
                            j["entities"][std::to_string(entity->id)]["components"][type_name][name]["value"]["data"].push_back("");
                    }
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
        GroupRef group = Group::groups_id[linked];
        //Make sure it's not a scene group
        bool skip = false;
        auto parents = get_all_parents(group);
        for (auto& parent : parents)
        {
            if(typeid(*parent.get()) == typeid(SceneGroup))
                skip = true;
        }
        if(skip)
        {
            continue;
        }
        j["systems"][scuffy_demangle(typeid(*system.get()).name())] = json::object();
        j["systems"][scuffy_demangle(typeid(*system.get()).name())]["linked"] = json::array();
        j["systems"][scuffy_demangle(typeid(*system.get()).name())]["linked"].push_back(linked);
    }
    return j;
}
void save_scene(const char* path, GroupRef scene)
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
	for (auto& node : Window_EntityHierarchy::nodes)
	{
		delete node;
	}
	//Set selected to null
	Window_EntityHierarchy::selected_node = nullptr;
	Window_EntityHierarchy::dragging_node = nullptr;
	Node::selected = nullptr;

	//Delete entities
	for (auto& entity : Entity::entities)
	{
        entity.second->destroy();
	}

    //Delete groups
    for (auto& group : Group::groups)
    {
        group.second->destroy();
    }

    Group::apply_destruction_queue();
    Entity::apply_destruction_queue();


	Window_EntityHierarchy::nodes.clear();

	Entity::entities.clear();
	Group::groups_id.clear();
	Group::groups.clear();
	Group::group_count = 0;
	Entity::entity_count = 0;
}

GroupRef open_scene_from_json(json data)
{
    int id_append_entity = Entity::entity_count;
    int id_append_group = Group::group_count;
    std::vector<SceneGroup*> scene_groups;

    GroupRef root = nullptr;
    //Load groups
    for (auto& [id, group] : data["groups"].items())
    {
        //Check if it is a auto group
        if (group["type"] == "auto")
        {
            AutoGroup *g = new AutoGroup(group["name"].get<std::string>());
            g->set_id(std::stoul(id) + id_append_group);
            for (auto& component : group["components"])
            {
                g->add_component_type(component.get<std::string>());
            }
            print_info("Created group: " + g->name);
        }
        else if(group["type"] == "scene")
        {
            SceneGroup *g = new SceneGroup(group["name"].get<std::string>(), group["path"].get<std::string>());
            g->set_id(std::stoul(id) + id_append_group);
            print_info("Created group: " + g->name);
            scene_groups.push_back(g);
        }
        else
        {
            Group *g = new Group(group["name"].get<std::string>());
            g->set_id(std::stoul(id) + id_append_group);
            print_info("Created group: " + g->name);
        }
    }
    //Link the groups together
    for (auto& [id, group] : data["groups"].items())
    {
        //Parse id as uint32_t
        uint32_t d = std::stoul(id) + id_append_group;
        GroupRef g = Group::groups_id[d];
        if (group.contains("parent"))
        {
            uint32_t parent_id = group["parent"].get<uint32_t>();
            GroupRef parent = Group::groups_id[parent_id];
            parent->add_group(g);
        }
        else
        {
            root = g;
        }
    }

    //Load entities
    for (auto& [id, entity] : data["entities"].items())
    {
        EntityRef e = Entity::create(entity["name"].get<std::string>());
        e->set_id(std::stoul(id) + id_append_entity);
        e->name = entity["name"].get<std::string>();
        for (auto& parent : entity["parents"])
        {
            Group::groups_id[parent.get<uint32_t>() + id_append_group]->add_entity(e);
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
                else if(data["type"] == "vector_lua")
                {
                    std::vector<LuaComponentData*> scripts;
                    for(auto& script : data["value"]["data"])
                    {
                        if(script["path"].get<std::string>() != "") {
                            LuaComponentData *s = new LuaComponentData(ResourceManager::load<LuaComponentScript>(script["path"].get<std::string>()));
                            //Set data
                            for (auto& [key, value] : script["data"].items())
                            {
                                if (value["type"] == "int") {
                                    *(int*)s->component_data[key].second = value["value"].get<int>();
                                }
                                else if (value["type"] == "float") {
                                    *(float*)s->component_data[key].second = value["value"].get<float>();
                                }
                                else if (value["type"] == "double") {
                                    *(double*)s->component_data[key].second = value["value"].get<double>();
                                }
                                else if (value["type"] == "bool") {
                                    *(bool*)s->component_data[key].second = value["value"].get<bool>();
                                }
                                else if (value["type"] == "string") {
                                    *(std::string*)s->component_data[key].second = value["value"].get<std::string>();
                                }
                                else if (value["type"] == "vec2") {
                                    *(Vec2*)s->component_data[key].second = Vec2(value["x"].get<float>(), value["y"].get<float>());
                                }
                                else if (value["type"] == "vec3") {
                                    *(Vec3*)s->component_data[key].second = Vec3(value["x"].get<float>(), value["y"].get<float>(), value["z"].get<float>());
                                }
                                else if (value["type"] == "color") {
                                    *(Color*)s->component_data[key].second = Color(value["r"].get<uint8_t>(), value["g"].get<uint8_t>(), value["b"].get<uint8_t>(), value["a"].get<uint8_t>());
                                }
                            }
                            scripts.push_back(s);
                        }
                        else {
                            scripts.push_back(nullptr);
                        }
                    }
                    *(std::vector<LuaComponentData*>*)c->component_data[name].second = scripts;
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
            Group::groups_id[linked.get<uint32_t>() + id_append_group]->add_system(s);
        }
    }
    //Initalize scene groups
    for(auto& group : scene_groups)
    {
        group->instantiate();
    }
    return root;
}

GroupRef open_scene(const char *path)
{
    std::ifstream f(path);
    json data = json::parse(f);
    if (f.is_open()) {
        GroupRef ref = open_scene_from_json(data);
        return ref;
    }
    Notify::notify({ResourceManager::load<Texture>("engine/error.png"),EditorTheme::color_palette_red,"Scene Loading Error", "Failed to open scene file: " + std::string(path)});
    return nullptr;
    try {

    }
    catch (std::exception& e)
    {
        Notify::notify({ResourceManager::load<Texture>("engine/error.png"),EditorTheme::color_palette_red,"Scene Loading Error", "Failed to open scene file: " + std::string(path)});
        Notify::notify({ResourceManager::load<Texture>("engine/error.png"),EditorTheme::color_palette_red,"Scene Loading Error", e.what()});
        return nullptr;
    }
}

GroupRef create_default_scene()
{
    Group* group = new Group("Main");
    AutoGroup* render = new AutoGroup("Render");
    render->add_system(new Render());
    render->add_component_type<Sprite>();
    render->add_component_type<Transform>();
    group->add_group(render);
    return group;
}
