#pragma once

#include <stdio.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string_view>

namespace Magnus::LibMagnus {

class Encryption {
private:
    // Private functions to handle encryption/decryption
    void encrypt(std::string_view input,
                         unsigned char public_key[32]);

    void decrypt(std::string_view input);

    unsigned char mNonce[32];

    unsigned char mPrivateKey[32];
    unsigned char mPublicKey[32];

    std::string& encrypted_data;
    std::string& decrypted_data;

public:
    Encryption();
    Encryption(std::string_view public_key);

    ~Encryption();
};

namespace Helpers {
    // Functions to read Private/Public keys from the disc
    std::string_view read_private_key();
    std::string_view read_public_key();

    // Functions to write Private/Public keys to the disc
    std::string_view write_private_key();
    std::string_view write_public_key();

}
};
