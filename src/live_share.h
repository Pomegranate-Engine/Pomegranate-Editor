#ifndef POMEGRANATE_LIVESHARESERVER_LIVE_SHARE_H
#define POMEGRANATE_LIVESHARESERVER_LIVE_SHARE_H
#include<enet/enet.h>
#include"LiveShare/live_share_shared_data.h"
#include"notifications.h"

class LiveShare
{
public:
    static bool is_verified;
    static char user_id;
    static bool live_sharing;
    static ENetAddress address;
    static ENetHost* client;
    static ENetPeer* peer;
    static void start_server();
    static void stop_server();
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
