#include "compression/lz4_wrapper.hpp"
#include <lz4.h>

namespace Magnus::LibMagnus::Compression {
LZ4::LZ4(std::string_view& input)
{
    _input = input;
}

LZ4::~LZ4() { }

std::string_view LZ4::get_string_view()
{
    if (_buffer.empty()) {
        _logger->error("LZ4: There isn't any buffer to return.");
    }
    return std::string_view(_buffer);
}

std::string& LZ4::get_string()
{
    if (_buffer.empty()) {
        _logger->error("LZ4: There isn't any buffer to return.");
    }
    return _buffer;
}

void LZ4::compress_string()
{
    const size_t buffer_size = LZ4_compressBound(_input.size());
    _buffer.resize(buffer_size);

    size_t const c_size = LZ4_compress_default(_input.data(), _buffer.data(),
        _input.size(), buffer_size);
    _buffer.resize(c_size);
    // Check return_value to determine what happened.
    if (c_size <= 0)
        _logger->error("LZ4: Failed to compress file.");

    // Debug: print ratio
    // std::cout << (float)c_size / _input.size() << std::endl;
};

void LZ4::decompress_string()
{
    const size_t buffer_size = LZ4_compressBound(_input.size());
    _buffer.resize(buffer_size);
    const int decompressed_size = LZ4_decompress_safe(_input.data(), _buffer.data(),
        _input.size(), buffer_size);
    if (decompressed_size < 0)
        _logger->error("LZ4: Failed to decompress file.");
}

void LZ4::compress_file() { }

void LZ4::decompress_file() { }

};
