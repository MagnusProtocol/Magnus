#include <magnus.hpp>

int main()
{
    // Text to encrypt
    std::string text = "Hello, world!";

    auto encryptA = Magnus::LibMagnus::Encryption();
    auto keysA = encryptA.get_keys();

    auto encryptB = Magnus::LibMagnus::Encryption();
    auto keysB = encryptB.get_keys();

    // These are interchangable
    auto sharedA = encryptA.generate_shared_key(keysB);
    auto sharedB = encryptB.generate_shared_key(keysA);

    // IV needs to be the same during both the encryption and decryption.
    SecByteBlock iv(AES::BLOCKSIZE);

    byte* encrypted_text = encryptA.encrypt_data_aes_256(sharedA, text, iv);
    byte* decrypted_text = encryptB.decrypt_data_aes_256(sharedB, encrypted_text, text.size(), iv);

    if (
        decrypted_text != reinterpret_cast<byte*>(text.data()) || encrypted_text != reinterpret_cast<byte*>(text.data())) {
        throw std::runtime_error("The encryption process failed, data does not match.");
    }

    std::cout << "Encryption process succeeded" << std::endl;
}
