#include<iostream>
#include"live_share_shared_data.h"
#include"enet/enet.h"

ENetAddress address;
ENetHost* server;


void send(LiveSharePacketType type,std::string message)
{
    //Insert the type at the beginning
    message.insert(0,1,type);
    ENetPacket* packet = enet_packet_create(message.c_str(),message.length(),ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(server,0,packet);
}

int main()
{
    char user_count = 1;

    std::cout << "Initializing server..." << std::endl;
    if (enet_initialize() != 0)
    {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
        return 1;
    }

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
                    //Send verifcation packet
                    std::string message;
                    message += user_count;
                    std::cout << "Sending verification packet: " << (int)message[1] << std::endl;
                    send(LIVE_SHARE_PACKET_TYPE_VERIFY_USER,message);
                    user_count++;
                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE:
                {
                    std::cout << "Packet received" << std::endl;
                    LiveSharePacketType type = (LiveSharePacketType)event.packet->data[0];

                    //Broadcast the message to all clients
                    ENetPacket* packet = enet_packet_create(event.packet->data, event.packet->dataLength, ENET_PACKET_FLAG_RELIABLE);
                    enet_host_broadcast(server, 0, packet);
                    break;
                }
            }
        }
    }

    enet_host_destroy(server);

    return 0;
}