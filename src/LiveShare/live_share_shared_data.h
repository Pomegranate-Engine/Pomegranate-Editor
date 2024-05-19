#ifndef POMEGRANATE_LIVESHARESERVER_LIVE_SHARE_SHARED_DATA_H
#define POMEGRANATE_LIVESHARESERVER_LIVE_SHARE_SHARED_DATA_H
#include<typeinfo>

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

};

int read_int_from_bytes(unsigned char* bytes);
float read_float_from_bytes(unsigned char* bytes);
size_t read_size_t_from_bytes(unsigned char* bytes);

char* write_int_to_bytes(int value);
char* write_float_to_bytes(float value);


#endif
