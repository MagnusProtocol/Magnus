#include "sockets/tcp.hpp"
#include <fstream>
#include <iostream>
#include <magnus.hpp>
#include <sockets/client.hpp>
#include <thread>

int main(int argc, char** argv)
{
    LibMagnus::Client client("127.0.0.1", 3000);

    auto file_read = LibMagnus::Utils::MMAP("../assets/image.jpg", LibMagnus::Utils::READ);
    std::string uncompressed_data = file_read.read();

    LibMagnus::Compression::ZSTD compressor(uncompressed_data, LibMagnus::Compression::COMPRESS);
    std::string_view data = compressor.get_string_view();

    const int packet_size = 1024;

    if (data.size() > packet_size)
    {
        std::cout << data.size() << std::endl;

        for (size_t i = 0; i < data.size(); i += packet_size)
        {
            client.Send(data.substr(i, i + packet_size));
            std::cout << i / packet_size << std::endl;
        }
    }

    return 0;
}
