#include "sockets/tcp.hpp"
#include <fstream>
#include <iostream>
#include <magnus.hpp>

using namespace LibMagnus;

int main()
{
    TCPServer server = TCPServer("127.0.0.1");

    server.Start();

    return 0;
}
