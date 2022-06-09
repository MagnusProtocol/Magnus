#include <cmath>
#include <magnus.h>


#include "sockets/socket.h"
#include "sys/sysinfo.h"
#include "sys/types.h"
#include <compression/lz4_wrapper.hh>
#include <iostream>

// int main()
// {
//     LibMagnus::Magnus new_magnus("/home/steve/Pictures/Wallpapers/Grass.jpg");
//     auto data = new_magnus.get_data();

//     // Compression
//     std::ofstream stream_compression("compressed.jpg.zst", std::ios::out | std::ios::binary);
//     auto lz4_compressor = LZ4(data);

//     lz4_compressor.compress();

//     stream_compression.write(
//         lz4_compressor.get_string()->data(),
//         lz4_compressor.get_string()->size());

//     // Decompression
//     std::ofstream stream_decompression("decompressed.jpg", std::ios::out | std::ios::binary);

//     auto compressed_string = lz4_compressor.get_string_view();
//     auto LZ4_decompressor = LZ4(compressed_string);

//     LZ4_decompressor.decompress();

//     stream_decompression.write(
//         LZ4_decompressor.get_string()->data(),
//         LZ4_decompressor.get_string()->size());
// }

int main()
{
    LibMagnus::Socket socket = LibMagnus::Socket();


    return 0;
}
