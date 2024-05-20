#ifndef POMEGRANATE_LIVESHARESERVER_LIVE_SHARE_SHARED_DATA_H
#define POMEGRANATE_LIVESHARESERVER_LIVE_SHARE_SHARED_DATA_H
#include<typeinfo>
#include<plusaes/plusaes.hpp>

enum LiveSharePacketType
{
    LIVE_SHARE_PACKET_TYPE_NONE = 0,
    LIVE_SHARE_PACKET_TYPE_CREATE_ENTITY = 1,
    LIVE_SHARE_PACKET_TYPE_VERIFY_USER = 2,
    LIVE_SHARE_PACKET_TYPE_CHANGE_PROPERTY = 3,
    LIVE_SHARE_PACKET_TYPE_ADD_COMPONENT = 4,
    LIVE_SHARE_PACKET_TYPE_DELETE_ENTITY = 5,
    LIVE_SHARE_PACKET_TYPE_CREATE_GROUP = 6,
    LIVE_SHARE_PACKET_TYPE_DELETE_GROUP = 7,
    LIVE_SHARE_PACKET_TYPE_DELETE_COMPONENT = 8,
    LIVE_SHARE_PACKET_TYPE_RESOURCE_EXISTS = 9,
    LIVE_SHARE_PACKET_TYPE_RESOURCE_DOES_NOT_EXIST = 10,
    LIVE_SHARE_PACKET_TYPE_RESOURCE = 11,
    LIVE_SHARE_PACKET_TYPE_PASSWORD = 12,
    LIVE_SHARE_PACKET_TYPE_PASSWORD_CORRECT = 13,
};

enum LiveShareDataType
{
    LIVE_SHARE_DATA_TYPE_NONE = 0,
    LIVE_SHARE_DATA_TYPE_INT = 1,
    LIVE_SHARE_DATA_TYPE_FLOAT = 2,
    LIVE_SHARE_DATA_TYPE_STRING = 3,
    LIVE_SHARE_DATA_TYPE_BOOL = 4,
    LIVE_SHARE_DATA_TYPE_SIZE_T = 5,
    LIVE_SHARE_DATA_TYPE_CHAR = 6,
    LIVE_SHARE_DATA_TYPE_COLOR = 7,
    LIVE_SHARE_DATA_TYPE_VEC2 = 8,
    LIVE_SHARE_DATA_TYPE_VEC3 = 9,
    LIVE_SHARE_DATA_TYPE_VEC4 = 10,
    LIVE_SHARE_DATA_TYPE_TEXTURE = 11,
};

int read_int_from_bytes(unsigned char* bytes);
float read_float_from_bytes(unsigned char* bytes);
size_t read_size_t_from_bytes(unsigned char* bytes);

char* write_int_to_bytes(int value);
char* write_float_to_bytes(float value);

std::string encrypt_message(const std::string& message, const std::string& key);
std::string decrypt_message(const std::string& encryptedMessage, const std::string& key);

#endif
