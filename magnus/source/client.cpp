#include "sockets/tcp.hpp"
#include <fstream>
#include <iostream>
#include <magnus.hpp>
#include <sockets/client.hpp>
#include <thread>

int main(int argc, char** argv)
{
    LibMagnus::Client client = LibMagnus::Client("127.0.0.1", 3000);


    std::filesystem::path filepath_read = argv[1];
    
    if (!std::filesystem::exists(filepath_read)) {
        std::cout << "File does not exist." << std::endl;
    }

    auto mapped_read = Magnus::LibMagnus::Utils::MMAP(filepath_read,
        Magnus::LibMagnus::Utils::MMAP_MODES::READ);

    std::string_view data = mapped_read.read();

    client.Send(data);

    return 0;
}
