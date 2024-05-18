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

int read_int_from_bytes(unsigned char* bytes);
float read_float_from_bytes(unsigned char* bytes);
size_t read_size_t_from_bytes(unsigned char* bytes);

char* write_int_to_bytes(int value);
char* write_float_to_bytes(float value);


#endif
