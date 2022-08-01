#include "sockets/tcp.hpp"
#include <fstream>
#include <iostream>
#include <magnus.hpp>
#include <sockets/client.hpp>
#include <thread>

using namespace LibMagnus;

int main()
{
    Client client = Client("127.0.0.1", 3000);

    client.Send("Hello, world !");

    std::thread a([&client]() {
        client.Send("Hello, world !");
    });

    a.join();

    return 0;
}
