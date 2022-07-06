#include <magnus.hpp>

int main()
{
    std::string data;
    std::filesystem::path filepath_read = "../assets/image.jpg";
    auto mapped_read = Magnus::LibMagnus::Utils::mmap_file(filepath_read,
        Magnus::LibMagnus::Utils::MMAP_MODES::READ, data);

    auto zstd_compressor = Magnus::LibMagnus::Compression::ZSTD(data);
    zstd_compressor.compress_string();
    std::string& compressed_string = zstd_compressor.get_string();

    byte* data_bytes = (byte*)malloc(compressed_string.size() * sizeof(char));

    Magnus::LibMagnus::string_to_byte(compressed_string, data_bytes);

    auto encryptA = Magnus::LibMagnus::Encryption();
    auto keysA = encryptA.get_keys();

    auto encryptB = Magnus::LibMagnus::Encryption();
    auto keysB = encryptB.get_keys();

    // These are interchangable
    auto sharedA = encryptA.generate_shared_key(keysB);
    auto sharedB = encryptB.generate_shared_key(keysA);

    // IV needs to be the same during both the encryption and decryption.
    SecByteBlock iv(AES::BLOCKSIZE);

    byte* encrypted_text = (byte*)malloc(compressed_string.size() * sizeof(char));
    encryptA.encrypt_data_aes_256(sharedA, compressed_string, encrypted_text, iv);

    std::filesystem::path filepath_write_encrypted = "./image_mmap_encrypted.jpg";
    auto mapped_write_encrypted = Magnus::LibMagnus::Utils::mmap_file(filepath_write_encrypted,
        Magnus::LibMagnus::Utils::MMAP_MODES::RW);

    std::string encrypted_data_string(reinterpret_cast<char*>(encrypted_text), compressed_string.size());
    Magnus::LibMagnus::Utils::write_mmap(mapped_write_encrypted, encrypted_data_string);

    byte* decrypted_text = (byte*)malloc(compressed_string.size() * sizeof(char));
    encryptB.decrypt_data_aes_256(sharedB, encrypted_text, compressed_string.size() * sizeof(char), decrypted_text, iv);

    std::string_view decrypted_text_string_view(reinterpret_cast<char*>(decrypted_text), compressed_string.size());
    auto zstd_decompressor = Magnus::LibMagnus::Compression::ZSTD(decrypted_text_string_view);
    zstd_decompressor.decompress_string();
    std::string& decompressed_string = zstd_decompressor.get_string();

    std::filesystem::path filepath_write_decrypted = "./image_mmap_decrypted.jpg";
    auto mapped_write_decrypted = Magnus::LibMagnus::Utils::mmap_file(filepath_write_decrypted,
        Magnus::LibMagnus::Utils::MMAP_MODES::RW);

    Magnus::LibMagnus::Utils::write_mmap(mapped_write_decrypted, decompressed_string);

    for (int i = 0; i < decrypted_text_string_view.size() * sizeof(char); i++) {
        if (
            decrypted_text[i] != data_bytes[i]) {
            throw std::runtime_error("The encryption process failed, data does not match.");
        }
    }

    Magnus::LibMagnus::Utils::unmap(mapped_read);
    Magnus::LibMagnus::Utils::unmap(mapped_write_decrypted);
    Magnus::LibMagnus::Utils::unmap(mapped_write_encrypted);

    free(encrypted_text);
    free(decrypted_text);
    free(data_bytes);

    std::cout << "Encryption process succeeded" << std::endl;
}
