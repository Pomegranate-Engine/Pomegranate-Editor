#include "live_share.h"

ENetAddress LiveShare::address;
ENetHost* LiveShare::client;
ENetPeer* LiveShare::peer;

void LiveShare::live_share_start_server()
{
    std::cout << "Initializing client..." << std::endl;
    //Initialize ENet

    if(enet_initialize() != 0)
    {
        std::cout << "ENet initialization failed" << std::endl;
    }


    client = enet_host_create(nullptr,1,2,0,0);

    enet_address_set_host(&address,"localhost");
    address.port = 1234;

    peer = enet_host_connect(client,&address,2,0);
    if(peer == nullptr)
    {
        std::cout << "No available peers for initiating an ENet connection" << std::endl;
    }
    else
    {
        std::cout << "Connected to server" << std::endl;
    }

    while(true)
    {
        ENetEvent event;
        if(enet_host_service(client, &event, 16))
        {
            std::cout << "Something happened!" << std::endl;
            switch(event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                {
                    std::cout << "User connected!" << std::endl;
                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE:
                {
                    std::cout << "Packet received" << std::endl;
                    break;
                }
            }
        }
    }

}
void LiveShare::live_share_stop_server()
{

}