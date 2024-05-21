#ifndef POMEGRANATE_LIVESHARESERVER_LIVE_SHARE_H
#define POMEGRANATE_LIVESHARESERVER_LIVE_SHARE_H
#include<enet/enet.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif
#include"LiveShare/live_share_shared_data.h"
#include"notifications.h"

int start_process(const std::string& app_path, const std::vector<std::string>& args);

class LiveShare
{
public:
    static bool is_verified;
    static char user_id;
    static bool live_sharing;
    static bool verified_password;
    static std::string join_address;
    static std::string join_password;
    static std::string join_port;
    static ENetAddress address;
    static ENetHost* client;
    static ENetPeer* peer;
    static void start_server();
    static void stop_server();
    static void join_server();
    static void leave_server();
    static void update();
    static void send(LiveSharePacketType type, std::string message);
    static void send_new_entity(EntityRef entity);
    static void send_change_property(EntityRef entity, std::string component, std::string property, size_t type, void* value);
    static void send_add_component(EntityRef entity, std::string component);
    static void send_delete_component(EntityRef entity, std::string component);
    static void send_delete_entity(EntityRef entity);
    static void send_create_group(GroupRef group);
    static void send_delete_group(GroupRef group);
    static void send_entity_full(EntityRef entity);
    static void send_query_file(std::string file);
};

#endif //POMEGRANATE_LIVESHARESERVER_LIVE_SHARE_H
