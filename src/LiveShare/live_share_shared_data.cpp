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

unsigned char* encrypt_message(const unsigned char* message, size_t length, const std::string& key) {
    if (key.empty()) return nullptr;

    unsigned char* encryptedMessage = new unsigned char[length];
    for (size_t i = 0; i < length; ++i) {
        encryptedMessage[i] = message[i] ^ key[i % key.length()];
    }
    return encryptedMessage;
}

unsigned char* decrypt_message(const unsigned char* message, size_t length, const std::string& key) {
    if (key.empty()) return nullptr;

    unsigned char* decryptedMessage = new unsigned char[length];
    for (size_t i = 0; i < length; ++i) {
        decryptedMessage[i] = message[i] ^ key[i % key.length()];
    }
    return decryptedMessage;
}