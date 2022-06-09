#include "compression/lz4_wrapper.hh"
#include <lz4.h>

LZ4::LZ4(std::string_view& input)
{
    _input = input;
    _buffer = new std::string;
}

LZ4::~LZ4()
{
    delete _buffer;
}

void LZ4::compress()
{
    const size_t buffer_size = LZ4_compressBound(_input.size());
    _buffer->resize(buffer_size);

    size_t const c_size = LZ4_compress_default(_input.data(), _buffer->data(), _input.size(), buffer_size);
    _buffer->resize(c_size);
    // Check return_value to determine what happened.
    if (c_size <= 0)
        std::cerr << "[ERROR] LZ4: Failed to compress file. ";

    // Debug: print ratio
    std::cout << (float)c_size / _input.size() << std::endl;
};

void LZ4::decompress()
{
    const size_t buffer_size = LZ4_compressBound(_input.size());
    _buffer->resize(buffer_size);
    const int decompressed_size = LZ4_decompress_safe(_input.data(), _buffer->data(), _input.size(), buffer_size);
    if (decompressed_size < 0)
        std::cerr << "[ERROR] LZ4: Failed to decompress file. ";
}

std::string_view LZ4::get_string_view()
{
    if (!_buffer) {
        std::cerr << "[ERROR] LZ4: There isn't any buffer to return." << std::endl;
    }
    return std::string_view(*_buffer);
}
std::string* LZ4::get_string()
{
    if (!_buffer) {
        std::cerr << "[ERROR] LZ4: There isn't any buffer to return." << std::endl;
    }
    return _buffer;
}
