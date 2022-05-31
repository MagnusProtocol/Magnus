#include "compression.hh"
#include <zstd.h>

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

#define CHECK_ZSTD(fn)                                           \
    do {                                                         \
        size_t const err = (fn);                                 \
        CHECK(!ZSTD_isError(err), "%s", ZSTD_getErrorName(err)); \
    } while (0)

class ZSTD : public Compression {
private:
    std::string* _buffer;
    std::string_view _input;
    // Compression context
    ZSTD_CCtx* _cctx;
    // Decompression context
    ZSTD_DCtx* _dctx;

public:
    ZSTD(std::string_view& input);
    ~ZSTD();

    std::string_view get_string_view();
    std::string* get_string();

    void compress();
    void decompress();
};
