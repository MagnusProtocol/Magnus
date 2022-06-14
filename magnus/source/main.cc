#include <compression/zstd_wrapper.hh>
#include <cstdio>
#include <hashing/blake3_wrapper.hh>
#include <magnus.hh>
#include <slicing/slicing.hh>
#include <chrono>

int main()
{
    std::filesystem::path filepath_image = "../assets/image.jpg";
    auto zstd_compressor = Magnus::Compression::ZSTD(filepath_image, Magnus::Compression::COMPRESS);
    zstd_compressor.compress_file();

    std::ofstream compression_stream("./compressed.jpg.zst", std::ios::out | std::ios::binary);
    compression_stream.write(zstd_compressor.get_string_view().data(),
                             zstd_compressor.get_string_view().size());

    std::filesystem::path filepath_compressed_image = "./compressed.jpg.zst";
    auto zstd_decompressor = Magnus::Compression::ZSTD(filepath_compressed_image, Magnus::Compression::DECOMPRESS);
    zstd_decompressor.decompress_file();

    std::ofstream decompression_stream("decompressed.jpg", std::ios::out | std::ios::binary);
    decompression_stream.write(zstd_decompressor.get_string_view().data(),
                             zstd_decompressor.get_string_view().size());

    auto blake3_hash = Magnus::Hashing::BLAKE3(zstd_compressor.get_string_view());
    auto hash = blake3_hash.hash();
    std::cout << hash << std::endl;


}
