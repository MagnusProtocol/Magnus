#include "compression.hh"
#include <zstd.h>
#include <filesystem>

#define CHECK(cond, ...)                     \
    do {                                     \
        if (!(cond)) {                       \
            fprintf(stderr,                  \
                "%s:%d CHECK(%s) failed: ",  \
                __FILE__,                    \
                __LINE__,                    \
                #cond);                      \
            fprintf(stderr, "" __VA_ARGS__); \
            fprintf(stderr, "\n");           \
            exit(1);                         \
        }                                    \
    } while (0)

#define CHECK_ZSTD(fn)                                                         \
    do {                                                                       \
        size_t const err = (fn);                                               \
        CHECK(!ZSTD_isError(err), "[ERROR] ZSTD: %s", ZSTD_getErrorName(err)); \
    } while (0)

namespace Magnus::Compression {

enum MODES {
COMPRESS = 0x123,
DECOMPRESS = 0x234,
};

class ZSTD : public CompressionBase {
private:
    // String buffer for storing the comrpessed data
    std::string* _buffer;
    // String view for taking in a string to compress
    std::string_view _input;
    // Path to the file to be compressed
    std::filesystem::path _filename;

    // Compression context
    ZSTD_CCtx* _cctx;
    // Decompression context
    ZSTD_DCtx* _dctx;

    // Input & output buffers for files
    void* _buff_in;
    void* _buff_out;

    // Sizes for both the buffers
    size_t _buff_in_size;
    size_t _buff_out_size;

    // File pointer in case we need it
    FILE* _fin;

public:
    ZSTD(std::string_view& input);
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
