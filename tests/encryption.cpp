#include <fstream>
#include <gtest/gtest.h>
#include <magnus.hpp>

// Demonstrate some basic assertions.
TEST(EncryptionTest, ECDHCryptoPP)
{
    auto EncryptA = Magnus::LibMagnus::Encryption();
    auto KeysA = EncryptA.get_keys();

    auto EncryptB = Magnus::LibMagnus::Encryption();
    auto KeysB = EncryptB.get_keys();

    // These are interchangable
    auto SharedA = EncryptA.generate_shared_key(KeysB);
    auto SharedB = EncryptB.generate_shared_key(KeysA);

    auto SharedDecodedA = EncryptA.decode_shared_key(SharedA);
    auto SharedDecodedB = EncryptB.decode_shared_key(SharedB);

    // Expect equality.
    EXPECT_EQ(SharedA, SharedB);
    EXPECT_EQ(SharedDecodedA, SharedDecodedB);
}
