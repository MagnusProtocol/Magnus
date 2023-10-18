#include "lz4.hpp"

#include <lz4.h>
#include <lz4frame.h>
#include <string>

#include <cassert>
#include <cstddef>
#include <cstring>

class not_implemented : public std::logic_error {
public:
	not_implemented() : std::logic_error("Function not yet implemented"){};
};

auto LZ4Compressor::compress(const std::string_view input) -> std::string {

	const int src_size = static_cast<int>(input.size());
	const auto max_dst_size = LZ4_compressBound(src_size);
	// Now build allocations for the data we'll be playing with.

	std::vector<char> dst(max_dst_size, ' ');
	const auto bytes_returned = static_cast<size_t>(
		LZ4_compress_fast(input.data(), dst.data(), src_size, max_dst_size, 1));

	return {dst.data(), bytes_returned};
}

auto LZ4Compressor::decompress(const std::string_view input) -> std::string {

	throw std::logic_error(
		"Cannot use LZ4 decompression without specifying the source size");

	return std::string(input);
}

auto LZ4Compressor::decompress(const std::string_view input, const int src_size)
	-> std::string {

	std::vector<char> regen_buffer(src_size, ' ');
	// The LZ4_decompress_safe function needs to know where the compressed data
	// is, how many bytes long it is, where the regen_buffer memory location is,
	// and how large regen_buffer (uncompressed) output will be. Again, save the
	// return_value.
	const auto decompressed_size = static_cast<size_t>(
		LZ4_decompress_safe(input.data(), regen_buffer.data(),
							static_cast<int>(input.size()), src_size));

	return {regen_buffer.data(), decompressed_size};
};

auto LZ4Compressor::compress(std::filesystem::path& path) -> int {
	{
		path.clear();
		throw not_implemented();
	}

	return -1;
}

auto LZ4Compressor::decompress(std::filesystem::path& path) -> int {
	{
		path.clear();
		throw not_implemented();
	}

	return -1;
}

auto LZ4Compressor::compress(const std::vector<std::filesystem::path>& paths)
	-> int {
	auto res = 0;
	for (auto path : paths) {
		res = compress(path);
	}
	return res;
}

auto LZ4Compressor::decompress(const std::vector<std::filesystem::path>& paths)
	-> int {
	auto res = 0;
	for (auto path : paths) {
		res = decompress(path);
	}
	return res;
}