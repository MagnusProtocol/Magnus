#include <magnus.hpp>

int main()
{

    // Text to encrypt
    std::string text = "Hello, world!";
    std::cout << text << std::endl;
    byte text_bytes[text.size()];
    Magnus::LibMagnus::string_to_byte(text, text_bytes);

    auto encryptA = Magnus::LibMagnus::Encryption();
    auto keysA = encryptA.get_keys();

    auto encryptB = Magnus::LibMagnus::Encryption();
    auto keysB = encryptB.get_keys();

    // These are interchangable
    auto sharedA = encryptA.generate_shared_key(keysB);
    auto sharedB = encryptB.generate_shared_key(keysA);

    // IV needs to be the same during both the encryption and decryption.
    SecByteBlock iv(AES::BLOCKSIZE);

    byte encrypted_text[text.size()];
    encryptA.encrypt_data_aes_256(sharedA, text, encrypted_text, iv);
    std::cout << encrypted_text << std::endl;
    byte decrypted_text[text.size()];
    encryptB.decrypt_data_aes_256(sharedB, encrypted_text, text.length(), decrypted_text, iv);

    std::cout << decrypted_text << std::endl;
    for (int i = 0; i < text.size(); i++) {
        if (
            decrypted_text[i] != text_bytes[i]) {
            throw std::runtime_error("The encryption process failed, data does not match.");
        }
    }
    std::cout << "Encryption process succeeded" << std::endl;
}
