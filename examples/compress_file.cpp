#include <fstream>
#include <magnus.hpp>

// Magnus compression wrapper file example
int main()
{
    std::filesystem::path filepath_image = "../assets/image.jpg";
    auto zstd_compressor = Magnus::LibMagnus::Compression::ZSTD(filepath_image,
        Magnus::LibMagnus::Compression::COMPRESS);
    zstd_compressor.compress_file();

    std::ofstream compression_stream("./compressed.jpg.zst", std::ios::out | std::ios::binary);
    compression_stream.write(zstd_compressor.get_string_view().data(),
        zstd_compressor.get_string_view().size());

    std::filesystem::path filepath_compressed_image = "./compressed.jpg.zst";
    auto zstd_decompressor = Magnus::LibMagnus::Compression::ZSTD(filepath_compressed_image,
        Magnus::LibMagnus::Compression::DECOMPRESS);
    zstd_decompressor.decompress_file();

    std::ofstream decompression_stream("decompressed.jpg", std::ios::out | std::ios::binary);
    decompression_stream.write(zstd_decompressor.get_string_view().data(),
        zstd_decompressor.get_string_view().size());
}
