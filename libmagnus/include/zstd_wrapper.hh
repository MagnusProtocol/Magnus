#include <iostream>
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

class ZSTD {
private:
    std::string* _buffer;

public:
    ZSTD();
    ~ZSTD();

    void compress(std::string_view& input);
    std::string_view get_string_view();
    std::string* get_string();
};
