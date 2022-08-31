#include "sockets/tcp.hpp"
#include "utils/utils.hpp"
#include <compression/zstd_wrapper.hpp>
#include <fstream>
#include <iostream>
#include <magnus.hpp>
#include <sockets/client.hpp>

int main()
{

	// LibMagnus::TCPServer server = LibMagnus::TCPServer("127.0.0.1");
	// server.Start();

	auto file_read = LibMagnus::Utils::MMAP("../assets/image.jpg", LibMagnus::Utils::READ);

	std::string uncompressed_data = file_read.read();
	LibMagnus::Compression::ZSTD compressor(uncompressed_data, LibMagnus::Compression::COMPRESS);

	auto file_write = LibMagnus::Utils::MMAP("./image.jpg.zst", LibMagnus::Utils::RW);
	std::string compressed_data = compressor.get_string();
	file_write.write_data(compressed_data);
	return 0;
}
