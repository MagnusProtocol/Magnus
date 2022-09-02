#include "sockets/tcp.hpp"
#include <fstream>
#include <iostream>
#include <magnus.hpp>
#include <sockets/client.hpp>
#include <thread>

int main(int argc, char** argv)
{
	LibMagnus::Client client("127.0.0.1", 3000);

	std::filesystem::path filepath_read;
	if (argc > 0)
		filepath_read = argv[1];
	else
	{
		std::cout << "Please input an argument." << std::endl;
		std::terminate();
	}

	if (!std::filesystem::exists(filepath_read))
	{
		std::cout << "File does not exist." << std::endl;
	}

	auto mapped_read =
		Magnus::LibMagnus::Utils::MMAP(filepath_read, Magnus::LibMagnus::Utils::MMAP_MODES::READ);

	std::string data = mapped_read.read();

	if (data.size() > 1024)
	{
		std::cout << data.size() << std::endl;

		for (size_t i = 0; i < data.size(); i += 1024)
		{
			std::string substr = data.substr(i, i + 1024);
			client.Send(substr);
		}
	}

	return 0;
}
