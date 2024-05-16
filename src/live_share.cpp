#include "live_share.h"
bool LiveShare::is_verified;
char LiveShare::user_id = 0;
bool LiveShare::live_sharing;
ENetAddress LiveShare::address;
ENetHost* LiveShare::client;
ENetPeer* LiveShare::peer;

void LiveShare::start_server()
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
        live_sharing = true;
    }
}
void LiveShare::stop_server()
{
    enet_peer_disconnect(peer,0);
    enet_host_destroy(client);
    enet_deinitialize();
    live_sharing = false;
    is_verified = false;
    user_id = 0;
}

void LiveShare::update()
{
    if(live_sharing)
    {
        ENetEvent event;
        if (enet_host_service(client, &event, 8)) {
            std::cout << "Something happened!" << std::endl;
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT: {
                    std::cout << "User connected!" << std::endl;
                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE: {
                    std::cout << "Packet received: " << (int)event.packet->data[0] << std::endl;
                    LiveSharePacketType type = (LiveSharePacketType) event.packet->data[0];
                    switch (type) {
                        case LIVE_SHARE_PACKET_TYPE_CREATE_ENTITY: {
                            std::cout << "Recieved create entity packet" << std::endl;
                            char from = event.packet->data[1];
                            if (from == user_id) {
                                break;
                            }

                            int id = read_int_from_bytes((unsigned char *) &event.packet->data[2]);
                            std::cout << "Creating entity with ID: " << id << std::endl;
                            EntityRef entity = Entity::create("New Entity");
                            entity->id = id;
                            entity->name = "Entity " + std::to_string(id);
                            break;
                        }
                        case LIVE_SHARE_PACKET_TYPE_VERIFY_USER:
                        {
                            if(!is_verified) {
                                is_verified = true;
                                user_id = event.packet->data[1];
                                std::cout << "Connection Verified, User ID: " << (int)user_id << std::endl;
                                Notify::notify({ResourceManager::load<Texture>("engine/check.png"),
                                                EditorTheme::color_palette_green, "Live Share", "Connection Verified, User ID: " + std::to_string((int)user_id)});
                                break;
                            }
                        }
                        default:
                            break;
                    }
                    break;
                }
            }
        }
    }
}

void LiveShare::send(LiveSharePacketType type,std::string message)
{
    if(!live_sharing)
    {
        return;
    }
    //Insert the type at the beginning
    message.insert(0,1,type);
    message.insert(1,1,user_id);
    ENetPacket* packet = enet_packet_create(message.c_str(),message.length(),ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer,0,packet);
}

void LiveShare::send_new_entity(EntityRef entity)
{
    char* id = reinterpret_cast<char*>(&entity->id);
    send(LIVE_SHARE_PACKET_TYPE_CREATE_ENTITY,std::string(id));
}