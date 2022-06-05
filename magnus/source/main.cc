#include <compression/zstd_wrapper.hh>
#include <hashing/blake3_wrapper.hh>
#include <magnus.hh>
#include <slicing/slicing.hh>
int main()
{
    // Read the file and get it's contents.
    LibMagnus::Magnus new_magnus("../assets/image.jpg");
    auto data = new_magnus.get_data();

    // Compression
    std::ofstream stream_compression("compressed.jpg.zstd", std::ios::out | std::ios::binary);
    auto ZSTD_compressor = ZSTD(data);
    ZSTD_compressor.compress();
    stream_compression.write(
        ZSTD_compressor.get_string().data(),
        ZSTD_compressor.get_string().size());

    // Decompression
    std::ofstream stream_decompression("decompressed.jpg", std::ios::out | std::ios::binary);
    auto compressed_string = ZSTD_compressor.get_string_view();
    auto ZSTD_decompressor = ZSTD(compressed_string);
    ZSTD_decompressor.decompress();
    stream_decompression.write(
        ZSTD_decompressor.get_string().data(),
        ZSTD_decompressor.get_string().size());

    auto slices = Slicing::slice_into_parts(data);

    auto blake3_hash = BLAKE3(ZSTD_compressor.get_string_view());
    auto hash = blake3_hash.hash();
    std::cout << hash << std::endl;
}