#ifndef POMEGRANATE_LIVESHARESERVER_LIVE_SHARE_H
#define POMEGRANATE_LIVESHARESERVER_LIVE_SHARE_H
#include<enet/enet.h>
#include"notifications.h"

#ifdef __WIN32__
#include<Windows.h>
#endif

class LiveShare
{
public:
    static ENetAddress address;
    static ENetHost* client;
    static ENetPeer* peer;
    static void live_share_start_server();
    static void live_share_stop_server();
};

#endif //POMEGRANATE_LIVESHARESERVER_LIVE_SHARE_H
