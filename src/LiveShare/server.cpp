#include <iostream>
#include "live_share_shared_data.h"
#include "enet/enet.h"
#include <vector>

ENetAddress address;
ENetHost* server;


void send(LiveSharePacketType type, std::string message)
{
    // Insert the type at the beginning
    message.insert(0, 1, type);
    ENetPacket* packet = enet_packet_create(message.c_str(), message.length(), ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(server, 0, packet);
}

int main()
{
    std::string passkey = "passwordpassword";
    char user_count = 1;

    std::cout << "Initializing server..." << std::endl;
    if (enet_initialize() != 0)
    {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
        return 1;
    }

    address.host = ENET_HOST_ANY;
    address.port = 1234;
    server = enet_host_create(&address, 32, 2, 0, 0);
    if (server == nullptr)
    {
        std::cerr << "Failed to create ENet Server" << std::endl;
        return -1;
    }

    std::cout << "Server initialized and listening on port " << address.port << std::endl;
    std::cout << "Starting event listener" << std::endl;

    bool running = true;
    int users_connected = 0;

    while (running)
    {
        ENetEvent event;
        if (enet_host_service(server, &event, 16))
        {
            std::cout << "Something happened!" << std::endl;
            switch (event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                {
                    std::cout << "User connected!" << std::endl;
                    // Send verification packet
                    std::string message;
                    message += user_count;
                    std::cout << "Sending verification packet: " << (int)message[0] << std::endl;
                    send(LIVE_SHARE_PACKET_TYPE_VERIFY_USER, message);
                    user_count++;
                    users_connected++;
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT:
                {
                    std::cout << "User disconnected!" << std::endl;
                    users_connected--;
                    if (users_connected == 0)
                    {
                        running = false;
                    }
                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE:
                {
                    std::cout << "Packet received" << std::endl;
                    LiveSharePacketType type = (LiveSharePacketType)event.packet->data[0];
                    //Get sender
                    int sender = (int)event.packet->data[1];

                    if(type == LIVE_SHARE_PACKET_TYPE_PASSWORD)
                    {
                        std::string password = std::string((char*)event.packet->data + 2, event.packet->dataLength - 2);
                        std::cout << "Password received: " << password << std::endl;
                        if(password == passkey)
                        {
                            std::cout << "Password correct!" << std::endl;
                            //Send confirmation packet
                            std::string message;
                            message += (char)LIVE_SHARE_PACKET_TYPE_PASSWORD_CORRECT;
                            message += (char)sender;
                            send(LIVE_SHARE_PACKET_TYPE_PASSWORD_CORRECT, message);
                        }
                        else
                        {
                            std::cout << "Password incorrect!" << std::endl;
                            //Disconnect the user
                            enet_peer_disconnect(event.peer, 0);
                        }
                        break;
                    }

                    //Encrypt the message
                    std::string decrypted = std::string((char*)event.packet->data + 1);
                    std::string encrypted = encrypt_message(decrypted, passkey);

                    // Broadcast the message to all clients
                    ENetPacket* packet = enet_packet_create(encrypted.c_str(), encrypted.length(), ENET_PACKET_FLAG_RELIABLE);
                    // Broadcast to all clients not in the blacklist
                    enet_host_broadcast(server, 0, packet);
                    break;
                }
            }
        }
    }

    std::cout << "No more users, Shutting down server..." << std::endl;
    enet_host_destroy(server);
    enet_deinitialize();

    return 0;
}
