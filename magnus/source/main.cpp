#include <fstream>
#include <magnus.hpp>
int main()
{

    // Text to encrypt
    std::string data;
    std::filesystem::path filepath_read = "../assets/image.jpg";
    auto mapped_file_read = Magnus::LibMagnus::Utils::mmap_file(filepath_read, Magnus::LibMagnus::Utils::MMAP_MODES::READ, data);

    auto zstd_compressor = Magnus::LibMagnus::Compression::ZSTD(data);
    zstd_compressor.compress_string();

    auto encryptA = Magnus::LibMagnus::Encryption();
    auto keysA = encryptA.get_keys();

    auto encryptB = Magnus::LibMagnus::Encryption();
    auto keysB = encryptB.get_keys();

    // These are interchangable
    auto sharedA = encryptA.generate_shared_key(keysB);
    auto sharedB = encryptB.generate_shared_key(keysA);

    // IV needs to be the same during both the encryption and decryption.
    SecByteBlock iv(AES::BLOCKSIZE);

    byte* encrypted_text = encryptA.encrypt_data_aes_256(sharedA, zstd_compressor.get_string(), iv);

    // Write the data that was read back to image_mmap.jpg
    std::filesystem::path filepath_write = "./image_mmap_encrypted.jpg";
    auto mapped_file_write = Magnus::LibMagnus::Utils::mmap_file(filepath_write, Magnus::LibMagnus::Utils::MMAP_MODES::RW);
    std::string_view to_write(reinterpret_cast<char*>(encrypted_text), zstd_compressor.get_string().size());
    Magnus::LibMagnus::Utils::write_mmap(mapped_file_write, to_write);
    Magnus::LibMagnus::Utils::unmap(mapped_file_read);
    Magnus::LibMagnus::Utils::unmap(mapped_file_write);

    std::string read_encrypted_data;
    auto mapped_file_read_encrypted = Magnus::LibMagnus::Utils::mmap_file(filepath_write, Magnus::LibMagnus::Utils::MMAP_MODES::READ, read_encrypted_data);

    byte* decrypted_text = encryptB.decrypt_data_aes_256(sharedB, read_encrypted_data, iv);

    std::filesystem::path filepath_write_decrypted_data = "./image_mmap_decrypted.jpg";
    auto mapped_file_write_decrypted = Magnus::LibMagnus::Utils::mmap_file(filepath_write_decrypted_data, Magnus::LibMagnus::Utils::MMAP_MODES::RW);
    std::string_view write_decrypted(reinterpret_cast<char*>(decrypted_text), zstd_compressor.get_string().size());
    Magnus::LibMagnus::Utils::write_mmap(mapped_file_write_decrypted, write_decrypted);

    if (
        decrypted_text != reinterpret_cast<byte*>(zstd_compressor.get_string().data()) || encrypted_text != reinterpret_cast<byte*>(zstd_compressor.get_string().data())) {
        throw std::runtime_error("The encryption process failed, data does not match.");
    }

    Magnus::LibMagnus::Utils::unmap(mapped_file_read_encrypted);
    Magnus::LibMagnus::Utils::unmap(mapped_file_write_decrypted);

    std::cout << "Encryption process succeeded" << std::endl;
}
