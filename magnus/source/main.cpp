#include <fstream>
#include <magnus.hpp>
#include "sockets/tcp.h"
#include <iostream>

using namespace LibMagnus;

int main()
{
    TCPServer server = TCPServer("127.0.0.1");

    server.Start();

    return 0;
}

