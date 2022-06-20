#pragma once

#include <iostream>
#include <spdlog/spdlog.h>
#include <stdio.h>
#include <string_view>

// Crypto++
#include <eccrypto.h>
#include <ecp.h>
#include <magnus.hpp>
#include <osrng.h>
#include <xed25519.h>
using namespace CryptoPP;

namespace Magnus::LibMagnus {
struct Keys {
    // Diffie-hellman key, initalized by class constructor
    ECDH<ECP>::Domain dh_key;
    // Private/public keys
    SecByteBlock private_key, public_key;
    // Initialize the values
    Keys()
    {
        dh_key = ECDH<ECP>::Domain(ASN1::brainpoolP512r1());
        private_key.New(dh_key.PrivateKeyLength());
        public_key.New(dh_key.PublicKeyLength());
    }
};

class Encryption {
private:
    // Private/Public keys
    Keys mKeys;
    SecByteBlock mSharedKey;
    OID mCurve;
    AutoSeededRandomPool mRng;

    // Called in every constructor, sets common values.
    void setup();

    // Generate a shared key
    SecByteBlock generate_shared_key(Keys& pKeys);

public:
    /*
     * @brief: return the keys stored in the private class variable.
     * @returns: returns a fully populated object of type, "Keys"
     */
    Keys& get_keys();

    /*
     * @breif: Generate a shared key from a (sender's) private and a (reciever's) public key.
     * @param keys: The reciever's keys.
     * @returns: The shared key, in printable form.
     */
    Integer get_shared_key(Keys& keys);

    Encryption();

    ~Encryption();
};
};
