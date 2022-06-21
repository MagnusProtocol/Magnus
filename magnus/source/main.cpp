#include <magnus.hpp>

int main()
{
    // Text to encrypt
    std::string data;
    std::filesystem::path filepath_read = "../assets/image.jpg";
    auto mapped_file_read = Magnus::LibMagnus::Utils::mmap_file(filepath_read, Magnus::LibMagnus::Utils::MMAP_MODES::READ, data);

    auto encryptA = Magnus::LibMagnus::Encryption();
    auto keysA = encryptA.get_keys();

    auto encryptB = Magnus::LibMagnus::Encryption();
    auto keysB = encryptB.get_keys();

    // These are interchangable
    auto sharedA = encryptA.generate_shared_key(keysB);
    auto sharedB = encryptB.generate_shared_key(keysA);

    // IV needs to be the same during both the encryption and decryption.
    SecByteBlock iv(AES::BLOCKSIZE);

    byte* encrypted_text = encryptA.encrypt_data_aes_256(sharedA, data, iv);

    // Write the data that was read back to image_mmap.jpg
    std::filesystem::path filepath_write = "./image_mmap.jpg";
    auto mapped_file_write = Magnus::LibMagnus::Utils::mmap_file(filepath_write, Magnus::LibMagnus::Utils::MMAP_MODES::RW);
    std::string_view to_write(reinterpret_cast<char*>(encrypted_text), data.size());
    Magnus::LibMagnus::Utils::write_mmap(mapped_file_write, to_write);

    byte* decrypted_text = encryptB.decrypt_data_aes_256(sharedB, data, iv);

    if (
        decrypted_text != reinterpret_cast<byte*>(data.data()) || encrypted_text != reinterpret_cast<byte*>(data.data())) {
        throw std::runtime_error("The encryption process failed, data does not match.");
    }

    Magnus::LibMagnus::Utils::unmap(mapped_file_read);
    Magnus::LibMagnus::Utils::unmap(mapped_file_write);
    std::cout << "Encryption process succeeded" << std::endl;
}
