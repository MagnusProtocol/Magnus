#include <encryption/encryption.hpp>

namespace Magnus::LibMagnus {
Encryption::Encryption()
{
    mKeys.dh_key.GenerateKeyPair(mRng, mKeys.private_key, mKeys.public_key);
}

Encryption::~Encryption()
{
}

Keys& Encryption::get_keys()
{
    return mKeys;
}

// param pKeys: person B's Keys.
// variable mKeys: person A's keys
SecByteBlock Encryption::generate_shared_key(Keys& pKeys)
{
    // pKeys = Keys that were provided
    // mKeyss = Keys that were generated in the class

    if (pKeys.public_key.empty())
        throw std::runtime_error("Please provide a public key");

    // Check if both keys have equal lengths
    if (mKeys.dh_key.AgreedValueLength() != mKeys.dh_key.AgreedValueLength())
        throw std::runtime_error("Shared shared size mismatch");

    // Shared key
    mSharedKey = SecByteBlock(mKeys.dh_key.AgreedValueLength());

    // Generate shared secret withe locally stored private key
    // and the newly obtaind public key

    if (!mKeys.dh_key.Agree(mSharedKey, mKeys.private_key, pKeys.public_key))
        throw std::runtime_error("Failed to reach shared secret");

    return mSharedKey;
}

Integer Encryption::get_shared_key(Keys& keys)
{
    SecByteBlock shared_key = generate_shared_key(keys);

    Integer shared_secret;
    shared_secret.Decode(shared_key.BytePtr(), shared_key.SizeInBytes());

    return shared_secret;
}
};
