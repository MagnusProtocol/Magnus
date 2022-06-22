#include "compression/zstd_wrapper.hpp"
#include <thread>

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

namespace Magnus::LibMagnus::Compression {
ZSTD::ZSTD(std::string_view input)
{
    _input = input;

    // Setup spdlog
    SET_LOGGER("ZSTD", _logger);

    _cctx = ZSTD_createCCtx();
    _dctx = ZSTD_createDCtx();

    CHECK(_cctx != NULL, "ZSTD_createCCtx() failed!");
    ZSTD_CCtx_setParameter(_cctx, ZSTD_c_compressionLevel, 3);
    ZSTD_CCtx_setParameter(_cctx, ZSTD_c_nbWorkers, std::thread::hardware_concurrency());
}

ZSTD::ZSTD(std::string& input)
{
    _input = input;

    // Setup spdlog
    SET_LOGGER("ZSTD", _logger);

    _cctx = ZSTD_createCCtx();
    _dctx = ZSTD_createDCtx();

    CHECK(_cctx != NULL, "ZSTD_createCCtx() failed!");
    ZSTD_CCtx_setParameter(_cctx, ZSTD_c_compressionLevel, 3);
    ZSTD_CCtx_setParameter(_cctx, ZSTD_c_nbWorkers, std::thread::hardware_concurrency());
}

ZSTD::ZSTD(std::filesystem::path filename, int mode)
{
    // Setup spdlog
    SET_LOGGER("ZSTD", _logger);

    _fin = fopen(filename.c_str(), "rb");

    _cctx = ZSTD_createCCtx();
    _dctx = ZSTD_createDCtx();

    CHECK(_cctx != NULL, "ZSTD_createCCtx() failed!");
    ZSTD_CCtx_setParameter(_cctx, ZSTD_c_compressionLevel, 3);
    ZSTD_CCtx_setParameter(_cctx, ZSTD_c_nbWorkers, std::thread::hardware_concurrency());

    if (mode == MODES::COMPRESS) {
        _buff_in_size = ZSTD_CStreamInSize();
        _buff_out_size = ZSTD_CStreamOutSize();
    } else {
        _buff_in_size = ZSTD_DStreamInSize();
        _buff_out_size = ZSTD_DStreamOutSize();
    }

    _buff_in = malloc(_buff_in_size);
    _buff_out = malloc(_buff_out_size);
}

ZSTD::~ZSTD()
{

    ZSTD_freeCCtx(_cctx);
    ZSTD_freeDCtx(_dctx);

    if (_buff_in != nullptr && _buff_out != nullptr) {
        free(_buff_in);
        free(_buff_out);
        fclose(_fin);
    }
}

/*
 * @brief: This is a getter method to get the compressed/decompressed as a std::string_view.
 * */
std::string_view ZSTD::get_string_view()
{
    if (_buffer.empty()) {
        _logger->error("ZSTD: There isn't any buffer to return.");
    }
    return std::string_view(_buffer);
}

/*
 * @brief: This is a getter method to get the compressed/decompressed std::string&.
 * */
// TODO: Fix strings
std::string& ZSTD::get_string()
{
    if (_buffer.empty()) {
        _logger->error("ZSTD: There isn't any buffer to return.");
    }
    return _buffer;
}
/*
 *  @brief: This method uses the string set by the ZSTD string constructor and decompresses it.
 * */
void ZSTD::compress_string()
{
    if (_input.empty() == true)
        _logger->error("Magnus: Please use the correct constructor.");

    const size_t buffer_size = ZSTD_compressBound(_input.size());
    _buffer.resize(buffer_size);

    size_t const c_size = ZSTD_compressCCtx(_cctx, _buffer.data(), buffer_size, _input.data(), _input.size(), 3);

    CHECK_ZSTD(c_size);
    _buffer.resize(c_size);
}

/*
 * @brief: This method uses the string set by the ZSTD string constructor and decompresses it.
 * */
void ZSTD::decompress_string()
{
    if (_input.empty() == true)
        _logger->error("Magnus: Please use the correct constructor.");

    unsigned long long const r_size = ZSTD_getFrameContentSize(_input.data(), _input.size());
    CHECK(r_size != ZSTD_CONTENTSIZE_ERROR, "ZSTD: Not compressed by zstd!");
    CHECK(r_size != ZSTD_CONTENTSIZE_UNKNOWN, "ZSTD: Original size unknown!");

    _buffer.resize(size_t(r_size));
    size_t const d_size = ZSTD_decompressDCtx(_dctx, _buffer.data(), r_size, _input.data(), _input.size());

    // When zstd knows the content size, it will error if it doesn't match.
    CHECK_ZSTD(d_size);
    CHECK(d_size == r_size, "Size does not match!");
}

/*
 * @brief: This method takes the file provided in the filesystem::path constructor and compresses it.
 * It stores the result in a string which can be accessed via the getter functions.
 * */
void ZSTD::compress_file()
{

    size_t const to_read = _buff_in_size;
    for (;;) {
        size_t read = fread(_buff_in, 1, to_read, _fin);

        int const last_chunk = (read < to_read);
        ZSTD_EndDirective const mode = last_chunk ? ZSTD_e_end : ZSTD_e_continue;

        ZSTD_inBuffer input = { _buff_in, read, 0 };

        int finished;
        do {

            ZSTD_outBuffer output = { _buff_out, _buff_out_size, 0 };
            size_t const remaining = ZSTD_compressStream2(_cctx, &output, &input, mode);

            CHECK_ZSTD(remaining);
            _buffer.append((char*)_buff_out, output.pos);

            finished = last_chunk ? (remaining == 0) : (input.pos == input.size);
        } while (!finished);

        CHECK(input.pos == input.size,
            "ZSTD: zstd only returns 0 when the input is completely consumed!");

        if (last_chunk) {
            break;
        }
    }
}

void ZSTD::decompress_file()
{
    /* This loop assumes that the input file is one or more concatenated zstd
     * streams. This example won't work if there is trailing non-zstd data at
     * the end, but streaming decompression in general handles this case.
     * ZSTD_decompressStream() returns 0 exactly when the frame is completed,
     * and doesn't consume input after the frame.
     */
    size_t const toRead = _buff_in_size;
    size_t read;
    // size_t lastRet = 0;
    // int isEmpty = 1;
    while ((read = fread(_buff_in, 1, toRead, _fin))) {
        // isEmpty = 0;
        ZSTD_inBuffer input = { _buff_in, read, 0 };
        /* Given a valid frame, zstd won't consume the last byte of the frame
         * until it has flushed all of the decompressed data of the frame.
         * Therefore, instead of checking if the return code is 0, we can
         * decompress just check if input.pos < input.size.
         */
        while (input.pos < input.size) {
            ZSTD_outBuffer output = { _buff_out, _buff_out_size, 0 };
            /* The return code is zero if the frame is complete, but there may
             * be multiple frames concatenated together. Zstd will automatically
             * reset the context when a frame is complete. Still, calling
             * ZSTD_DCtx_reset() can be useful to reset the context to a clean
             * state, for instance if the last decompression call returned an
             * error.
             */
            size_t const ret = ZSTD_decompressStream(_dctx, &output, &input);
            CHECK_ZSTD(ret);
            _buffer.append(static_cast<const char*>(_buff_out), output.pos);
            // lastRet = ret;
        }
    }
}

}
