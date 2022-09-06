#include "sockets/tcp.hpp"
#include "utils/utils.hpp"
#include <compression/zstd_wrapper.hpp>
#include <fstream>
#include <iostream>
#include <magnus.hpp>
#include <sockets/client.hpp>

int main()
{
    LibMagnus::TCPServer server = LibMagnus::TCPServer("127.0.0.1");
    server.Start();

    std::string compressed_data = "";
    for (int i = 0; i < server.BufferStack.size(); i++)
    {
        compressed_data.append(server.BufferStack[i]);
    }

    std::cout << compressed_data << std::endl;

    auto file_write = LibMagnus::Utils::MMAP("./image.jpg.zst", LibMagnus::Utils::RW);
    file_write.write_data(compressed_data);

    return 0;
}
