#include "live_share.h"

int start_process(const std::string& app_path, const std::vector<std::string>& args)
{
#ifdef _WIN32
    std::string command = "\"" + app_path + "\"";
    for (const auto& arg : args) {
        command += " " + arg;
    }

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;

    if (!CreateProcess(NULL, &command[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "CreateProcess failed: " << GetLastError() << std::endl;
        return 1;
    }

    // Close process and thread handles immediately to not pause the current application
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
#else
    pid_t pid = fork();

    if (pid == -1) {
        std::cerr << "Fork failed" << std::endl;
        return 1;
    } else if (pid == 0) {
        // Child process
        std::vector<const char*> c_args;
        c_args.push_back(app_path.c_str());
        for (const auto& arg : args) {
            c_args.push_back(arg.c_str());
        }
        c_args.push_back(nullptr);

        execv(app_path.c_str(), const_cast<char* const*>(c_args.data()));

        // If execv returns, it must have failed
        std::cerr << "Execv failed" << std::endl;
        return 1;
    } else {
        // Parent process does not wait for the child process
        return 0;
    }
#endif
}

bool LiveShare::is_verified;
char LiveShare::user_id = 0;
bool LiveShare::live_sharing;
bool LiveShare::verified_password = false;
std::string LiveShare::join_address;
std::string LiveShare::join_port;
std::string LiveShare::join_password = "";
ENetAddress LiveShare::address;
ENetHost* LiveShare::client;
ENetPeer* LiveShare::peer;

const size_t INT_HASH = typeid(int).hash_code();
const size_t FLOAT_HASH = typeid(float).hash_code();
const size_t STRING_HASH = typeid(std::string).hash_code();
const size_t BOOL_HASH = typeid(bool).hash_code();
const size_t COLOR_HASH = typeid(Color).hash_code();
const size_t VEC2_HASH = typeid(Vec2).hash_code();
const size_t VEC3_HASH = typeid(Vec3).hash_code();
const size_t VEC4_HASH = typeid(Vec4).hash_code();
const size_t TEXTURE_HASH = typeid(Texture*).hash_code();
const size_t SIZE_T_HASH = typeid(size_t).hash_code();
const size_t CHAR_HASH = typeid(char).hash_code();

std::vector<std::string> split(std::string str, char delimiter)
{
    std::vector<std::string> parts;
    std::string part;
    for (char c : str)
    {
        if (c == delimiter)
        {
            parts.push_back(part);
            part = "";
        }
        else
        {
            part += c;
        }
    }
    parts.push_back(part);
    return parts;
}

void LiveShare::start_server()
{
    std::cout << "Starting server..." << std::endl;
    //Start server
    std::vector<std::string> args;
    args.push_back(join_port);
    args.push_back(join_password);
    start_process("Pomegranate_LiveShareServer", args);
    //Give the server time to start
    SDL_Delay(1000);
    join_address = "localhost";
    //Join server
    join_server();
}

void LiveShare::stop_server()
{

}

void LiveShare::join_server()
{
    std::cout << "Initializing client..." << std::endl;
    //Initialize ENet

    if(enet_initialize() != 0)
    {
        std::cout << "ENet initialization failed" << std::endl;
    }


    client = enet_host_create(nullptr,1,2,0,0);

    enet_address_set_host(&address,LiveShare::join_address.c_str());
    address.port = std::stoi(join_port);

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

void LiveShare::leave_server()
{
    if(!live_sharing)
    {
        return;
    }
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
                    //Send password packet
                    std::string message;
                    message += join_password;
                    std::cout << "Sending password packet: " << message << std::endl;
                    send(LIVE_SHARE_PACKET_TYPE_PASSWORD, message);
                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE: {
                    //Decrypt message
                    if(verified_password)
                    {
                        //Decrypt message into the
                        event.packet->data = decrypt_message(event.packet->data,event.packet->dataLength,join_password);
                    }
                    std::cout << "Packet received: " << (int)event.packet->data[0] << std::endl;
                    LiveSharePacketType type = (LiveSharePacketType) event.packet->data[0];
                    switch (type) {
                        case LIVE_SHARE_PACKET_TYPE_PASSWORD_CORRECT: {
                            std::cout << "Recieved password correct packet" << std::endl;
                            verified_password = true;
                            break;
                        }
                        case LIVE_SHARE_PACKET_TYPE_CREATE_ENTITY: {
                            std::cout << "Recieved create entity packet" << std::endl;
                            char from = event.packet->data[1];
                            if (from == user_id) {
                                break;
                            }

                            int id = read_int_from_bytes(event.packet->data + 2);
                            int group = read_int_from_bytes(event.packet->data + 6);
                            std::cout << "Creating entity with ID: " << id << " as child of Group: " << group << std::endl;
                            EntityRef entity = Entity::create("New Entity");
                            entity->set_id(id);
                            entity->name = "Entity " + std::to_string(id);
                            Group::groups_id[group]->add_entity(entity);
                            break;
                        }
                        case LIVE_SHARE_PACKET_TYPE_ADD_COMPONENT: {
                            std::cout << "Recieved add component packet" << std::endl;
                            char from = event.packet->data[1];
                            if (from == user_id) {
                                break;
                            }
                            int id = read_int_from_bytes((unsigned char *) &event.packet->data[2]);
                            EntityRef entity = EntityRef(Entity::entities[id]);
                            if(entity == nullptr)
                            {
                                std::cout << "Entity not found" << std::endl;
                                break;
                            }
                            std::string message = std::string((char *) event.packet->data + 6, event.packet->dataLength - 6);
                            std::vector<std::string> parts = split(message, '/');
                            std::string component = parts[0];
                            std::cout << "User: " << (int)event.packet->data[1] << " editor on entity: " << id << " adding component: " << component << std::endl;
                            entity->add_component(component.c_str());
                            break;
                        }
                        case LIVE_SHARE_PACKET_TYPE_DELETE_COMPONENT:
                        {
                            std::cout << "Recieved delete component packet" << std::endl;
                            char from = event.packet->data[1];
                            if (from == user_id) {
                                break;
                            }
                            int id = read_int_from_bytes((unsigned char *) &event.packet->data[2]);
                            EntityRef entity = EntityRef(Entity::entities[id]);
                            if(entity == nullptr)
                            {
                                std::cout << "Entity not found" << std::endl;
                                break;
                            }
                            std::string message = std::string((char *) event.packet->data + 6, event.packet->dataLength - 6);
                            std::vector<std::string> parts = split(message, '/');
                            std::string component = parts[0];
                            std::cout << "User: " << (int)event.packet->data[1] << " editor on entity: " << id << " deleting component: " << component << std::endl;
                            entity->remove_component(entity->get_component(component.c_str()));
                            break;
                        }
                        case LIVE_SHARE_PACKET_TYPE_DELETE_ENTITY: {
                            std::cout << "Recieved delete entity packet" << std::endl;
                            char from = event.packet->data[1];
                            if (from == user_id) {
                                break;
                            }
                            int id = read_int_from_bytes((unsigned char *) &event.packet->data[2]);
                            EntityRef entity = EntityRef(Entity::entities[id]);
                            if(entity == nullptr)
                            {
                                std::cout << "Entity not found" << std::endl;
                                break;
                            }
                            std::cout << "User: " << (int)event.packet->data[1] << " editor deleting entity: " << id << std::endl;
                            entity->force_destroy();
                            break;
                        }
                        case LIVE_SHARE_PACKET_TYPE_CHANGE_PROPERTY: {
                            std::cout << "Recieved change property packet" << std::endl;
                            char from = event.packet->data[1];
                            if (from == user_id) {
                                break;
                            }
                            std::string data = std::string((char *) event.packet->data, event.packet->dataLength);
                            int id = read_int_from_bytes((unsigned char *) event.packet->data + 2);
                            EntityRef entity = EntityRef(Entity::entities[id]);
                            if(entity == nullptr)
                            {
                                std::cout << "Entity not found" << std::endl;
                                break;
                            }
                            std::string message = std::string((char *) event.packet->data + 6, event.packet->dataLength - 6);
                            std::vector<std::string> parts = split(message, (char)255);
                            std::string component = parts[0];
                            Component* comp = entity->get_component(component.c_str());
                            std::string property = parts[1];
                            LiveShareDataType type = (LiveShareDataType)parts[2][0];
                            char* value = (char*)parts[3].c_str();
                            std::cout << "User: " << (int)event.packet->data[1] << " editor on entity: " << id << " changing: " << component << "/" << property << " to: " << message << std::endl;

                            if(type == LIVE_SHARE_DATA_TYPE_INT)
                            {
                                int v = read_int_from_bytes((unsigned char*)value);
                                comp->set(property.c_str(),v);
                            }
                            else if(type == LIVE_SHARE_DATA_TYPE_FLOAT)
                            {
                                float v = read_float_from_bytes((unsigned char*)value);
                                comp->set(property.c_str(),v);
                            }
                            else if(type == LIVE_SHARE_DATA_TYPE_STRING)
                            {
                                std::string v = std::string(value);
                                comp->set(property.c_str(),v);
                            }
                            else if(type == LIVE_SHARE_DATA_TYPE_BOOL)
                            {
                                bool v = (bool)value[0];
                                comp->set(property.c_str(),v);
                            }
                            else if(type == LIVE_SHARE_DATA_TYPE_COLOR)
                            {
                                Color v;
                                v.r = read_int_from_bytes((unsigned char*)value);
                                v.g = read_int_from_bytes((unsigned char*)value + 4);
                                v.b = read_int_from_bytes((unsigned char*)value + 8);
                                v.a = read_int_from_bytes((unsigned char*)value + 12);
                                comp->set(property.c_str(),v);
                            }
                            else if(type == LIVE_SHARE_DATA_TYPE_VEC2)
                            {
                                Vec2 v;
                                v.x = read_float_from_bytes((unsigned char*)value);
                                v.y = read_float_from_bytes((unsigned char*)value + 4);
                                comp->set(property.c_str(),v);
                            }
                            else if(type == LIVE_SHARE_DATA_TYPE_VEC3)
                            {
                                Vec3 v;
                                v.x = read_float_from_bytes((unsigned char*)value);
                                v.y = read_float_from_bytes((unsigned char*)value + 4);
                                v.z = read_float_from_bytes((unsigned char*)value + 8);
                                comp->set(property.c_str(),v);
                            }
                            else if(type == LIVE_SHARE_DATA_TYPE_VEC4)
                            {
                                Vec4 v;
                                v.x = read_float_from_bytes((unsigned char*)value);
                                v.y = read_float_from_bytes((unsigned char*)value + 4);
                                v.z = read_float_from_bytes((unsigned char*)value + 8);
                                v.w = read_float_from_bytes((unsigned char*)value + 12);
                                comp->set(property.c_str(),v);
                            }
                            else if(type == LIVE_SHARE_DATA_TYPE_SIZE_T)
                            {
                                size_t v = read_size_t_from_bytes((unsigned char*)value);
                                comp->set(property.c_str(),v);
                            }
                            else if(type == LIVE_SHARE_DATA_TYPE_CHAR)
                            {
                                char v = value[0];
                                comp->set(property.c_str(),v);
                            }
                            else if(type == LIVE_SHARE_DATA_TYPE_TEXTURE)
                            {
                                //Load texture
                                Texture* v = ResourceManager::load<Texture>(value);
                                comp->set(property.c_str(),v);
                            }
                            break;
                        }
                        case LIVE_SHARE_PACKET_TYPE_CHANGE_ENTITY_NAME:
                        {
                            std::cout << "Recieved change entity name packet" << std::endl;
                            char from = event.packet->data[1];
                            if (from == user_id) {
                                break;
                            }
                            int id = read_int_from_bytes((unsigned char *) &event.packet->data[2]);
                            EntityRef entity = EntityRef(Entity::entities[id]);
                            if(entity == nullptr)
                            {
                                std::cout << "Entity not found" << std::endl;
                                break;
                            }
                            std::string message = std::string((char *) event.packet->data + 6, event.packet->dataLength - 6);
                            std::cout << "User: " << (int)event.packet->data[1] << " editor on entity: " << id << " changing name to: " << message << std::endl;
                            entity->name = message;
                            break;
                        }
                        case LIVE_SHARE_PACKET_TYPE_CREATE_GROUP:
                        {
                            std::cout << "Recieved create group packet" << std::endl;
                            char from = event.packet->data[1];
                            if (from == user_id) {
                                break;
                            }
                            int id = read_int_from_bytes(event.packet->data + 2);
                            int parent_group = read_int_from_bytes(event.packet->data + 6);
                            std::cout << "Creating group with ID: " << id << std::endl;
                            GroupRef group = new Group("New Group");
                            group->set_id(id);
                            Group::groups_id[parent_group]->add_group(group);
                            break;
                        }
                        case LIVE_SHARE_PACKET_TYPE_DELETE_GROUP:
                        {
                            std::cout << "Recieved delete group packet" << std::endl;
                            char from = event.packet->data[1];
                            if (from == user_id) {
                                break;
                            }
                            int id = read_int_from_bytes(event.packet->data + 2);
                            Group::groups_id[id]->force_destroy();
                            break;
                        }
                        case LIVE_SHARE_PACKET_TYPE_CREATE_SYSTEM:
                        {
                            std::cout << "Recieved create system packet" << std::endl;
                            char from = event.packet->data[1];
                            if (from == user_id) {
                                break;
                            }
                            std::string message = std::string((char*)event.packet->data + 2,event.packet->dataLength - 2);
                            std::cout << "User: " << (int)event.packet->data[1] << " editor creating system: " << message << std::endl;
                            if(System::system_types.find(message) != System::system_types.end()) {
                                SystemRef system = System::system_types[message]();
                            }
                            else
                            {
                                std::cout << "System type not found" << std::endl;
                            }
                            break;
                        }
                        case LIVE_SHARE_PACKET_TYPE_RESOURCE_EXISTS:
                        {
                            std::cout << "Recieved resource exists packet" << std::endl;
                            char from = event.packet->data[1];
                            if (from == user_id) {
                                break;
                            }
                            std::string file = std::string((char*)event.packet->data + 2,event.packet->dataLength - 2);
                            std::cout << "User: " << (int)event.packet->data[1] << " editor checking if resource exists: " << file << std::endl;
                            std::replace(file.begin(),file.end(),'\\','/');
                            if(ResourceManager::exists(file))
                            {

                            }
                            else
                            {
                                std::string message;
                                //Add original sender
                                message += (char)from;
                                //Add file
                                message += file;
                                send(LIVE_SHARE_PACKET_TYPE_RESOURCE_DOES_NOT_EXIST,message);
                            }
                            break;
                        }
                        case LIVE_SHARE_PACKET_TYPE_RESOURCE_DOES_NOT_EXIST:
                        {
                            std::cout << "Recieved resource does not exist packet" << std::endl;
                            char from = event.packet->data[1];
                            if (from == user_id) {
                                break;
                            }
                            //Get original sender
                            char original_sender = event.packet->data[2];
                            if(original_sender == user_id)
                            {
                                print_info("I was the original sender");
                                Notify::notify({ResourceManager::load<Texture>("engine/warning.png"),
                                                EditorTheme::color_palette_red, "Live Share", "User: " + std::to_string((int)event.packet->data[1]) + " editor requested resource that does not exist, sending file"});
                                //Send the resource
                                std::string file = std::string((char*)event.packet->data + 3,event.packet->dataLength - 3);
                                std::replace(file.begin(),file.end(),'\\','/');
                                std::cout << "User: " << (int)event.packet->data[1] << " editor requesting resource: " << file << std::endl;
                                //Load bytes from file
                                std::ifstream file_stream(file,std::ios::binary);
                                std::vector<char> bytes((std::istreambuf_iterator<char>(file_stream)),std::istreambuf_iterator<char>());
                                std::string message;
                                //Add original sender
                                message += (char)user_id;
                                //Add file path length
                                message += (char)file.length();
                                //Add file
                                message += file;
                                //Add bytes
                                message += std::string(bytes.begin(),bytes.end());
                                send(LIVE_SHARE_PACKET_TYPE_RESOURCE,message);
                                break;
                            }
                            break;
                        }
                        case LIVE_SHARE_PACKET_TYPE_RESOURCE:
                        {
                            std::cout << "Recieved resource packet" << std::endl;
                            char from = event.packet->data[1];
                            if (from == user_id) {
                                break;
                            }
                            //Get original sender
                            //Get file path length
                            char file_path_length = event.packet->data[3];
                            //Get file path
                            std::string file = std::string((char*)event.packet->data + 4,file_path_length);
                            //Replace \ with / on m*c
                            std::replace(file.begin(),file.end(),'\\','/');
                            //Get bytes
                            std::vector<char> bytes(event.packet->data + 4 + file_path_length,event.packet->data + event.packet->dataLength);
                            std::cout << "User: " << (int)event.packet->data[1] << " editor sending resource: " << file << std::endl;
                            //Write bytes to file
                            std::ofstream file_stream(file,std::ios::binary);
                            file_stream.write(bytes.data(),bytes.size());
                            break;
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
    char* group_id = reinterpret_cast<char*>(&entity->get_parent_groups()[0]->id);
    send(LIVE_SHARE_PACKET_TYPE_CREATE_ENTITY,std::string(id,4) + std::string(group_id,4));
}

void LiveShare::send_change_property(EntityRef entity, std::string component, std::string property, size_t type, void* value)
{
    char* id = static_cast<char*>(static_cast<void*>(&entity->id));

    LiveShareDataType t = LIVE_SHARE_DATA_TYPE_NONE;
    if(type == INT_HASH)
    {
        t = LIVE_SHARE_DATA_TYPE_INT;
    }
    else if(type == FLOAT_HASH)
    {
        t = LIVE_SHARE_DATA_TYPE_FLOAT;
    }
    else if(type == STRING_HASH)
    {
        t = LIVE_SHARE_DATA_TYPE_STRING;
    }
    else if(type == BOOL_HASH)
    {
        t = LIVE_SHARE_DATA_TYPE_BOOL;
    }
    else if(type == COLOR_HASH)
    {
        t = LIVE_SHARE_DATA_TYPE_COLOR;
    }
    else if(type == VEC2_HASH)
    {
        t = LIVE_SHARE_DATA_TYPE_VEC2;
    }
    else if(type == VEC3_HASH)
    {
        t = LIVE_SHARE_DATA_TYPE_VEC3;
    }
    else if(type == VEC4_HASH)
    {
        t = LIVE_SHARE_DATA_TYPE_VEC4;
    }
    else if(type == TEXTURE_HASH)
    {
        t = LIVE_SHARE_DATA_TYPE_TEXTURE;
    }
    else if(type == SIZE_T_HASH)
    {
        t = LIVE_SHARE_DATA_TYPE_SIZE_T;
    }
    else if(type == CHAR_HASH)
    {
        t = LIVE_SHARE_DATA_TYPE_CHAR;
    }
    char ty = (char)t;

    std::string message;
    message += std::string(id,sizeof(int));

    message += scuffy_demangle(component.c_str());
    message += (char)255;
    message += property;
    message += (char)255;
    message += ty;
    message += (char)255;

    if(type == INT_HASH)
    {
        int* v = static_cast<int*>(value);
        message += std::string(static_cast<char*>(static_cast<void*>(v)),sizeof(int));
    }
    else if(type == FLOAT_HASH)
    {
        float* v = static_cast<float*>(value);
        message += std::string(static_cast<char*>(static_cast<void*>(v)),sizeof(float));
    }
    else if(type == STRING_HASH)
    {
        std::string* v = static_cast<std::string*>(value);
        message += *v;
    }
    else if(type == BOOL_HASH)
    {
        bool* v = static_cast<bool*>(value);
        message += std::string(static_cast<char*>(static_cast<void*>(v)),sizeof(bool));
    }
    else if(type == COLOR_HASH)
    {
        Color* v = static_cast<Color*>(value);
        message += std::string(static_cast<char*>(static_cast<void*>(&v->r)),sizeof(int));
        message += std::string(static_cast<char*>(static_cast<void*>(&v->g)),sizeof(int));
        message += std::string(static_cast<char*>(static_cast<void*>(&v->b)),sizeof(int));
        message += std::string(static_cast<char*>(static_cast<void*>(&v->a)),sizeof(int));
    }
    else if(type == VEC2_HASH)
    {
        Vec2* v = static_cast<Vec2*>(value);
        message += std::string(static_cast<char*>(static_cast<void*>(&v->x)),sizeof(float));
        message += std::string(static_cast<char*>(static_cast<void*>(&v->y)),sizeof(float));
    }
    else if(type == VEC3_HASH)
    {
        Vec3* v = static_cast<Vec3*>(value);
        message += std::string(static_cast<char*>(static_cast<void*>(&v->x)),sizeof(float));
        message += std::string(static_cast<char*>(static_cast<void*>(&v->y)),sizeof(float));
        message += std::string(static_cast<char*>(static_cast<void*>(&v->z)),sizeof(float));
    }
    else if(type == VEC4_HASH)
    {
        Vec4* v = static_cast<Vec4*>(value);
        message += std::string(static_cast<char*>(static_cast<void*>(&v->x)),sizeof(float));
        message += std::string(static_cast<char*>(static_cast<void*>(&v->y)),sizeof(float));
        message += std::string(static_cast<char*>(static_cast<void*>(&v->z)),sizeof(float));
        message += std::string(static_cast<char*>(static_cast<void*>(&v->w)),sizeof(float));
    }
    else if(type == TEXTURE_HASH)
    {
        //Send resource path
        Texture** v = static_cast<Texture**>(value);
        message += (*v)->path;
    }
    send(LIVE_SHARE_PACKET_TYPE_CHANGE_PROPERTY,message);
}

void LiveShare::send_add_component(EntityRef entity, std::string component)
{
    char* id = static_cast<char*>(static_cast<void*>(&entity->id));
    std::string message;
    message += std::string(id,sizeof(int));
    message += scuffy_demangle(component.c_str());
    send(LIVE_SHARE_PACKET_TYPE_ADD_COMPONENT,message);
}

void LiveShare::send_delete_component(EntityRef entity, std::string component)
{
    char* id = static_cast<char*>(static_cast<void*>(&entity->id));
    std::string message;
    message += std::string(id,sizeof(int));
    message += scuffy_demangle(component.c_str());
    send(LIVE_SHARE_PACKET_TYPE_DELETE_COMPONENT,message);
}

void LiveShare::send_delete_entity(EntityRef entity)
{
    char* id = static_cast<char*>(static_cast<void*>(&entity->id));
    send(LIVE_SHARE_PACKET_TYPE_DELETE_ENTITY,std::string(id));
}

void LiveShare::send_create_group(GroupRef entity)
{
    char* id = static_cast<char*>(static_cast<void*>(&entity->id));
    char* parent_id = static_cast<char*>(static_cast<void*>(&entity->get_parent()->id));
    send(LIVE_SHARE_PACKET_TYPE_CREATE_GROUP,std::string(id,4) + std::string(parent_id,4));
}

void LiveShare::send_delete_group(GroupRef entity)
{
    char* id = static_cast<char*>(static_cast<void*>(&entity->id));
    send(LIVE_SHARE_PACKET_TYPE_DELETE_GROUP,std::string(id));
}

void LiveShare::send_entity_full(Pomegranate::EntityRef entity)
{
    send_new_entity(entity);
    auto components = entity->get_components();
    for (auto& component : components)
    {
        send_add_component(entity, component.first->name());
        for (auto& property : component.second->component_data)
        {
            send_change_property(entity, component.first->name(), property.first, property.second.first->hash_code(), property.second.second);
        }
    }
}

void LiveShare::send_query_file(std::string file)
{
    std::replace(file.begin(),file.end(),'\\','/');
    send(LIVE_SHARE_PACKET_TYPE_RESOURCE_EXISTS, file);
}

void LiveShare::send_change_entity_name(Pomegranate::EntityRef entity, std::string name)
{
    char* id = static_cast<char*>(static_cast<void*>(&entity->id));
    std::string message;
    message += std::string(id,sizeof(int));
    message += name;
    send(LIVE_SHARE_PACKET_TYPE_CHANGE_ENTITY_NAME,message);
}

void LiveShare::send_create_system(Pomegranate::SystemRef system)
{
    std::string name = scuffy_demangle(typeid(*(system.get())).name());
    send(LIVE_SHARE_PACKET_TYPE_CREATE_SYSTEM,name);
}