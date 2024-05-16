#ifndef POMEGRANATE_LIVESHARESERVER_LIVE_SHARE_SHARED_DATA_H
#define POMEGRANATE_LIVESHARESERVER_LIVE_SHARE_SHARED_DATA_H

enum LiveSharePacketType
{
    LIVE_SHARE_PACKET_TYPE_NONE = 0,
    LIVE_SHARE_PACKET_TYPE_CREATE_ENTITY = 1,
    LIVE_SHARE_PACKET_TYPE_VERIFY_USER = 2,
};

int read_int_from_bytes(unsigned char* bytes);
float read_float_from_bytes(unsigned char* bytes);

char* write_int_to_bytes(int value);
char* write_float_to_bytes(float value);

#endif
