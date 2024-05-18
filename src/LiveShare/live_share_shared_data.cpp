#include "live_share_shared_data.h"
int read_int_from_bytes(unsigned char* bytes)
{
    return reinterpret_cast<int&>(*bytes);
}

float read_float_from_bytes(unsigned char* bytes)
{
    return reinterpret_cast<float&>(*bytes);
}

size_t read_size_t_from_bytes(unsigned char* bytes)
{
    return reinterpret_cast<size_t&>(*bytes);
}

char* write_int_to_bytes(int value)
{
    char* bytes = new char[4];
    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = value & 0xFF;
    return bytes;
}

char* write_float_to_bytes(float value)
{
    char* bytes = new char[4];
    *((float*)bytes) = value;
    return bytes;
}