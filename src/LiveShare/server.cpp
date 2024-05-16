#include<iostream>
#include"live_share_shared_data.h"
#include"enet/enet.h"

int main()
{
    std::cout << "Initializing server..." << std::endl;
    if (enet_initialize() != 0)
    {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
        return 1;
    }

    ENetAddress address;
    ENetHost* server;

    address.host = ENET_HOST_ANY;
    address.port = 1234;
    server = enet_host_create(&address, 32, 2,0,0);
    if(server == nullptr)
    {
        std::cerr << "Failed to create ENet Server" << std::endl;
        return -1;
    }

    std::cout << "Starting event listener" << std::endl;
    while(true)
    {
        ENetEvent event;
        if(enet_host_service(server, &event, 16))
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

    enet_host_destroy(server);

    return 0;
}