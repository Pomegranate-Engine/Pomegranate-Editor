#ifndef POMEGRANATE_LIBRARY_ENTITY_HIERARCHY_WINDOW_H
#define POMEGRANATE_LIBRARY_ENTITY_HIERARCHY_WINDOW_H
#include "live_share.h"
#include "editor_window.h"
#include "Imgui/imgui.h"
//stdlib imgui
#include "Imgui/misc/cpp/imgui_stdlib.h"
#include "Pomegranate/pomegranate.h"
#include "Pomegranate/ecs.h"
#include "Pomegranate/resource_manager.h"
#include "Pomegranate/ttf_font.h"
#include "hotkey_manager.h"
#include "notifications.h"

#include"Pomegranate/utils.h"
#include <cstdlib>
#include "scene.h"
#include<memory>
#include "theme.h"
#include <algorithm>

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
    EntityRef entity;
    GroupRef group;
    SystemRef system;

    std::vector<Node*> children;
    bool open;
    Node(EntityRef entity);
    Node(GroupRef group);
    Node(SystemRef system);
};

class Window_EntityHierarchy : public EditorWindow
{
public:
    static Node* currently_linking;
    static bool linking;
    static bool trying_to_link;
    static Node* to_try_link;
    static float linking_distance;
    std::string search;
    static bool searching;
    Vec2 cam_pos;
    float zoom;
    static std::vector<Node*> nodes;
    static Node* selected_node;
    static Node* dragging_node;
    Window_EntityHierarchy();
    std::vector<Node*> find_all_parents(Node* node);
    bool is_node_visible(Node* node);
    void build_graph(GroupRef group, Node* parent = nullptr);
    void draw_node(Node*n);
    void simulate_node(Node* node);
    void render() override;
    void update() override;
    static void focus();
    static void create_entity();
    static void create_group();
    static void create_auto_group();
    static void create_system();
    static void delete_node();
    static void duplicate();
    static void move(Vec2 dir);
    static void move_right();
    static void move_left();
    static void move_up();
    static void move_down();
    static void begin_link();
};

#include "entity_hierarchy_window.inl"
#endif //POMEGRANATE_LIBRARY_ENTITY_HIERARCHY_WINDOW_H
