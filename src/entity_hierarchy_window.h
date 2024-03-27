#ifndef POMEGRANATE_LIBRARY_ENTITY_HIERARCHY_WINDOW_H
#define POMEGRANATE_LIBRARY_ENTITY_HIERARCHY_WINDOW_H
#include "editor_window.h"
#include "Imgui/imgui.h"
#include "Pomegranate/pomegranate.h"
#include "Pomegranate/ecs.h"
#include "Pomegranate/resource_manager.h"
#include "Pomegranate/ttf_font.h"

#include"utils.h"
#include <cstdlib>
#include "scene.h"
#include<memory>
using namespace Pomegranate;
class Node
{
public:
    static Node* selected;
    Pomegranate::Vec2 pos;
    Pomegranate::Vec2 velocity;
    std::vector<Node*> linked;
    float size;
    Color color;
    Texture* texture;
    std::unique_ptr<Entity> entity;
    std::unique_ptr<EntityGroup> group;
    std::unique_ptr<System> system;
    bool open;
    Node(Entity* entity);
    Node(EntityGroup* group);
    Node(System* system);
};

class Window_EntityHierarchy : public EditorWindow
{
public:
    Texture* entity_texture;
    Texture* group_texture;
    SDL_Texture* graph_texture;
    Vec2 cam_pos;
    float zoom;
    static std::vector<Node*> nodes;
    static Node* selected_node;
    static Node* dragging_node;
    Window_EntityHierarchy();
    void build_graph(EntityGroup* group, Node* parent = nullptr);
    void draw_node(Node*n);
    void simulate_node(Node* node);
    void render() override;
    void update() override;
};

#include "entity_hierarchy_window.inl"
#endif //POMEGRANATE_LIBRARY_ENTITY_HIERARCHY_WINDOW_H
