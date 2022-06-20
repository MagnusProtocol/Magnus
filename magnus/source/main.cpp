#include <magnus.hpp>

int main()
{
    auto encryptA = Magnus::LibMagnus::Encryption();
    auto keysA = encryptA.get_keys();

    auto encryptB = Magnus::LibMagnus::Encryption();
    auto keysB = encryptB.get_keys();

    auto sharedA = encryptA.get_shared_key(keysB);
    auto sharedB = encryptB.get_shared_key(keysA);

    // Decode the shared secrets and print them
    std::cout << "(A): " << std::hex << sharedA << std::endl;
    std::cout << "(B): " << std::hex << sharedB << std::endl;

    // Check if the shared secrets are actually equal
    if (sharedA != sharedB)
        throw std::runtime_error("Failed to reach shared secret (C)");

    std::cout << "Agreed to shared secret" << std::endl;
}
