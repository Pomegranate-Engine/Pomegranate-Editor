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

std::string encrypt_message(const std::string& message, const std::string& key) {
    // Ensure the key is exactly 16 characters (128 bits)
    if (key.size() != 16) {
        throw std::invalid_argument("Key must be 16 characters (128 bits) long");
    }

    // Initialize the IV (Initialization Vector) with 16 bytes
    const unsigned char iv[16] = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
    };

    // Convert the key to a vector of unsigned char
    std::vector<unsigned char> keyVec(key.begin(), key.end());

    // Calculate the padded size for the encrypted message
    const unsigned long encrypted_size = plusaes::get_padded_encrypted_size(message.size());

    // Prepare a vector to hold the encrypted data
    std::vector<unsigned char> encrypted(encrypted_size);

    // Perform the encryption
    plusaes::encrypt_cbc(
            reinterpret_cast<const unsigned char*>(message.data()),  // Input data
            message.size(),  // Input data size
            keyVec.data(),  // Key
            keyVec.size(),  // Key size
            &iv,  // IV
            encrypted.data(),  // Output buffer
            encrypted.size(),  // Output buffer size
            true  // Padding enabled
    );

    // Convert the encrypted vector to a string and return it
    return std::string(encrypted.begin(), encrypted.end());
}

std::string decrypt_message(const std::string& message, const std::string& key) {
    //Convert the encrypted message to a vector of unsigned char
    std::vector<unsigned char> encryptedMessage(message.begin(), message.end());
    // Ensure the key is exactly 16 characters (128 bits)
    if (key.size() != 16) {
        throw std::invalid_argument("Key must be 16 characters (128 bits) long");
    }

    // Initialize the IV (Initialization Vector) with 16 bytes
    const unsigned char iv[16] = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
    };

    // Convert the key to a vector of unsigned char
    std::vector<unsigned char> keyVec(key.begin(), key.end());

    // Prepare a vector to hold the decrypted data
    std::vector<unsigned char> decrypted(encryptedMessage.size());

    // Variable to hold the size of the decrypted data
    unsigned long padded_size = 0;

    // Perform the decryption
    plusaes::decrypt_cbc(
            encryptedMessage.data(),  // Encrypted data
            encryptedMessage.size(),  // Encrypted data size
            keyVec.data(),  // Key
            keyVec.size(),  // Key size
            &iv,  // IV
            decrypted.data(),  // Output buffer
            decrypted.size(),  // Output buffer size
            &padded_size  // Actual size of decrypted data
    );

    // Resize the decrypted vector to the actual size of the decrypted data
    decrypted.resize(padded_size);

    // Convert the decrypted vector to a string and return it
    return std::string(decrypted.begin(), decrypted.end());
}