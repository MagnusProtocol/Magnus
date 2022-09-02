#include "sockets/tcp.hpp"
#include "utils.hpp"
#include <fstream>
#include <iostream>
#include <magnus.hpp>
#include <sockets/client.hpp>

int main()
{

	LibMagnus::TCPServer server = LibMagnus::TCPServer("127.0.0.1");
	server.Start();

	auto file = Magnus::LibMagnus::Utils::MMAP("somefile.txt", Magnus::LibMagnus::Utils::RW);
	file.write_data(server.Buffer);
	return 0;
}
