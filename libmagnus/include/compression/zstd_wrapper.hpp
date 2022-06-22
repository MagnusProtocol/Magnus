#pragma once
#include "compression.hpp"
#include <filesystem>
#include <zstd.h>

namespace Magnus::LibMagnus::Compression {

enum MODES {
    COMPRESS = 0x123,
    DECOMPRESS = 0x234,
};

class ZSTD : public CompressionBase {
private:
    // String buffer for storing the comrpessed data
    std::string _buffer;
    // String view for taking in a string to compress
    std::string_view _input;
    // Path to the file to be compressed
    std::filesystem::path _filename;

    // Compression context
    ZSTD_CCtx* _cctx;
    // Decompression context
    ZSTD_DCtx* _dctx;

    // Input & output buffers for files
    void* _buff_in = nullptr;
    void* _buff_out = nullptr;

    // Sizes for both the buffers
    size_t _buff_in_size;
    size_t _buff_out_size;

    // File pointer in case we need it
    FILE* _fin;

    // Spdlog logger, multithreaded stderr
    std::shared_ptr<spdlog::logger> _logger;

public:
    ZSTD(std::string_view input);
    ZSTD(std::string& input);
    ZSTD(std::filesystem::path filename, int mode);
    ~ZSTD();

    std::string_view get_string_view();
    std::string& get_string();

    void compress_string();
    void decompress_string();

    void compress_file();
    void decompress_file();
};
};
