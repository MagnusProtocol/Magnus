#pragma once
#include "compression.hpp"
#include <filesystem>

namespace Magnus::LibMagnus::Compression {
class LZ4 : public CompressionBase {
private:
    std::string _buffer;
    std::string_view _input;

    // Spdlog logger, multithreaded stderr
    std::shared_ptr<spdlog::logger> _logger;

public:
    LZ4(std::string_view& input);
    LZ4(std::filesystem::path filename);
    ~LZ4();

    std::string_view get_string_view();
    std::string get_string();

    void compress_string();
    void decompress_string();

    void compress_file();
    void decompress_file();
};
} // namespace Magnus::LibMagnus::Compression
