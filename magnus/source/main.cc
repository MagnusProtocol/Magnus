#include <cmath>
#include <magnus.hh>

#include "sys/sysinfo.h"
#include "sys/types.h"
#include <iostream>
#include <zstd_wrapper.hh>

int main()
{
    LibMagnus::Magnus new_magnus("/home/steve/Pictures/Wallpapers/Grass.jpg");
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

    std::ofstream stream("hurt.flac.zst", std::ios::out | std::ios::binary);

    auto zstd_compressor = ZSTD();
    zstd_compressor.compress(data);
    stream.write(
        zstd_compressor.get_string()->data(),
        zstd_compressor.get_string()->size());
}