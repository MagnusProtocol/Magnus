#include <cmath>
#include <magnus.hh>

#include "sys/sysinfo.h"
#include "sys/types.h"
#include <iostream>
#include <zstd_wrapper.hh>

int main()
{
    LibMagnus::Magnus new_magnus("/home/steve/Games/DOOM_ETERNAL/UP1003-CUSA13338_00-DOOM050000000000-A0100-V0100.part05.rar");
    auto data = new_magnus.get_data();

    size_t size = data.size();
    size_t x = size % (size_t)ceil(sqrt(size)) + 2;
    size_t y = (x - size % x);
    size_t packet_size = size / y;

    if (packet_size > 65500) {
        packet_size /= x;
    }

    std::cout << packet_size << std::endl
              << size << std::endl
              << size / packet_size << std::endl;

    if (packet_size == size)
        std::cout << "Yes" << std::endl;

    // Compression
    std::ofstream stream_compression("compressed.jpg.zst", std::ios::out | std::ios::binary);
    auto zstd_compressor = ZSTD(data);

    zstd_compressor.compress();

    stream_compression.write(
        zstd_compressor.get_string()->data(),
        zstd_compressor.get_string()->size());

    // Decompression
    std::ofstream stream_decompression("decompressed.jpg", std::ios::out | std::ios::binary);

    auto compressed_string = zstd_compressor.get_string_view();
    auto zstd_decompressor = ZSTD(compressed_string);

    zstd_decompressor.decompress();

    stream_decompression.write(
        zstd_decompressor.get_string()->data(),
        zstd_decompressor.get_string()->size());
}