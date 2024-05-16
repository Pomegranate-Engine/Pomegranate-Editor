#ifndef POMEGRANATE_LIVESHARESERVER_LIVE_SHARE_H
#define POMEGRANATE_LIVESHARESERVER_LIVE_SHARE_H
#include<enet/enet.h>
#include"LiveShare/live_share_shared_data.h"
#include"notifications.h"

#ifdef __WIN32__
#include<Windows.h>
#endif

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
};

#endif //POMEGRANATE_LIVESHARESERVER_LIVE_SHARE_H
