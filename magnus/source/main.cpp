#include <fstream>
#include <magnus.hpp>
int main()
{
    /*
    auto encryptA = Magnus::LibMagnus::Encryption();
    auto keysA = encryptA.get_keys();

    auto encryptB = Magnus::LibMagnus::Encryption();
    auto keysB = encryptB.get_keys();

    // These are interchangable
    auto sharedA = encryptA.generate_shared_key(keysB);
    auto sharedB = encryptB.generate_shared_key(keysA);
    // IV needs to be the same during both the encryption and decryption.
    SecByteBlock iv(AES::BLOCKSIZE);

    // Step 1: Read + compress the file
    std::filesystem::path filepath_read = "../assets/image.jpg";
    auto zstd_compressor = Magnus::LibMagnus::Compression::ZSTD(filepath_read, Magnus::LibMagnus::Utils::MMAP_MODES::READ);
    zstd_compressor.compress_file();
    auto data = zstd_compressor.get_string();

    // Step 2: encrypt the file's data
    byte* encrypted_text = encryptA.encrypt_data_aes_256(sharedA, data, iv);

    // Step 3: write the encrypted + compressed data
    std::filesystem::path filepath_write = "./image_mmap_encrypted.jpg.mag";
    auto mapped_file_write = Magnus::LibMagnus::Utils::mmap_file(filepath_write,
        Magnus::LibMagnus::Utils::MMAP_MODES::RW);
    std::string_view write_encrypted(reinterpret_cast<char*>(encrypted_text), zstd_compressor.get_string().size());
    Magnus::LibMagnus::Utils::write_mmap(mapped_file_write, write_encrypted);

    // Step 4: Read the encrypted + compressed data from the disk
    std::string read_encrypted_data;
    auto mapped_file_read_encrypted = Magnus::LibMagnus::Utils::mmap_file(filepath_write,
        Magnus::LibMagnus::Utils::MMAP_MODES::READ, read_encrypted_data);

    // Step 5: Decrypt the data
    byte* decrypted_text = encryptB.decrypt_data_aes_256(sharedB, read_encrypted_data, iv);

    // Step 6: Decompress the data
    auto zstd_decompressor = Magnus::LibMagnus::Compression::ZSTD(read_encrypted_data);
    zstd_decompressor.decompress_string();

    // Step 7: Write the data back to the disk
    std::filesystem::path filepath_write_decrypted_data = "./image_mmap_decrypted.jpg";
    auto mapped_file_write_decrypted = Magnus::LibMagnus::Utils::mmap_file(filepath_write_decrypted_data,
        Magnus::LibMagnus::Utils::MMAP_MODES::RW);
    std::string_view write_decrypted(reinterpret_cast<char*>(decrypted_text), zstd_decompressor.get_string().size());
    Magnus::LibMagnus::Utils::write_mmap(mapped_file_write_decrypted, write_decrypted);

    Magnus::LibMagnus::Utils::unmap(mapped_file_write);
    Magnus::LibMagnus::Utils::unmap(mapped_file_write_decrypted);
    Magnus::LibMagnus::Utils::unmap(mapped_file_read_encrypted);
    std::cout << "Encryption process succeeded" << std::endl;
    */

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
