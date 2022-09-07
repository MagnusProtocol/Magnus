#include "compression/zstd_wrapper.hpp"
#include <thread>

#define CHECK(cond, ...)                                                                           \
    do                                                                                             \
    {                                                                                              \
        if (!(cond))                                                                               \
        {                                                                                          \
            fprintf(stderr, "%s:%d CHECK(%s) failed: ", __FILE__, __LINE__, #cond);                \
            fprintf(stderr, "" __VA_ARGS__);                                                       \
            fprintf(stderr, "\n");                                                                 \
            exit(1);                                                                               \
        }                                                                                          \
    } while (0)

#define CHECK_ZSTD(fn)                                                                             \
    do                                                                                             \
    {                                                                                              \
        size_t const err = (fn);                                                                   \
        CHECK(!ZSTD_isError(err), "[ERROR] ZSTD: %s", ZSTD_getErrorName(err));                     \
    } while (0)

namespace Magnus::LibMagnus::Compression
{
// Common setup function for all the class constructors
void ZSTD::setup()
{
    // Setup spdlog
    SET_LOGGER("ZSTD", mLogger);

    mCctx = ZSTD_createCCtx();
    mDctx = ZSTD_createDCtx();

    CHECK(mCctx != NULL, "ZSTD_createCCtx() failed!");
    // ZSTD_CCtx_setParameter(mCctx, ZSTD_c_compressionLevel, 3);
    ZSTD_CCtx_setParameter(mCctx, ZSTD_c_nbWorkers, (int)std::thread::hardware_concurrency());
}

/*
 * @brief: Compress/Decompress a std::string_view; to obtain the result of this function, call the
 * applicable getter function.
 * @param input: the string_view to be compressed/decompressed
 * @param mode: To compress or to decompress, that is the question.
 */
ZSTD::ZSTD(std::string_view input, MODES mode)
{
    mInput = input;

    setup();

    if (mode == MODES::COMPRESS)
    {
        compress_string();
    }
    else
    {
        decompress_string();
    }
}

/*
 * @brief: Compress/Decompress a std::string_view; to obtain the result of this function, call the
 * applicable getter function.
 * @param input: the string to be compressed/decompressed
 * @param mode: To compress or to decompress, that is the question.
 */
ZSTD::ZSTD(std::string& input, MODES mode)
{
    mInput = input;

    setup();

    if (mode == MODES::COMPRESS)
    {
        compress_string();
    }
    else
    {
        decompress_string();
    }
}

/*
 * @brief: Compress/Decompress file; to obtain the result of this function, call the applicable
 * getter function.
 * @param filename: Path to the file on which the operation has to be performed
 * @param mode: To compress or to decompress, that is the question.
 */
ZSTD::ZSTD(std::filesystem::path filename, MODES mode)
{
    mFin = fopen(filename.c_str(), "rb");

    setup();

    if (mode == MODES::COMPRESS)
    {
        mBuffInSize = ZSTD_CStreamInSize();
        mBuffOutSize = ZSTD_CStreamOutSize();

        mBuffIn = malloc(mBuffInSize);
        mBuffOut = malloc(mBuffOutSize);

        compress_file();
    }
    else
    {
        mBuffInSize = ZSTD_DStreamInSize();
        mBuffOutSize = ZSTD_DStreamOutSize();

        mBuffIn = malloc(mBuffInSize);
        mBuffOut = malloc(mBuffOutSize);

        decompress_file();
    }
}

/*
 * @brief: Class destructor, does all the cleanup
 */
ZSTD::~ZSTD()
{

    ZSTD_freeCCtx(mCctx);
    ZSTD_freeDCtx(mDctx);

    if (mBuffIn != nullptr && mBuffOut != nullptr)
    {
        free(mBuffIn);
        free(mBuffOut);
        fclose(mFin);
    }
}

/*
 * @brief: Internal function to compress the string in the local variable _input.
 */
void ZSTD::compress_string()
{
    if (mInput.empty() == true)
    {
        mLogger->error("Magnus: Please use the correct constructor.");
        throw std::exception();
    }

    const size_t buffer_size = ZSTD_compressBound(mInput.size());
    mBuffer.resize(buffer_size);

    size_t const c_size =
        ZSTD_compressCCtx(mCctx, mBuffer.data(), buffer_size, mInput.data(), mInput.size(), 3);

    CHECK_ZSTD(c_size);
    mBuffer.resize(c_size);
}

/*
 * @brief: Internal function to compress a file which had been opened in the constructor.
 */
void ZSTD::compress_file()
{

    size_t const to_read = mBuffInSize;
    for (;;)
    {
        size_t read = fread(mBuffIn, 1, to_read, mFin);

        int const last_chunk = (read < to_read);
        ZSTD_EndDirective const mode = last_chunk ? ZSTD_e_end : ZSTD_e_continue;

        ZSTD_inBuffer input = {mBuffIn, read, 0};

        int finished;
        do
        {

            ZSTD_outBuffer output = {mBuffOut, mBuffOutSize, 0};
            size_t const remaining = ZSTD_compressStream2(mCctx, &output, &input, mode);

            CHECK_ZSTD(remaining);
            mBuffer.append((char*)mBuffOut, output.pos);

            finished = last_chunk ? (remaining == 0) : (input.pos == input.size);
        } while (!finished);

        CHECK(input.pos == input.size,
              "ZSTD: zstd only returns 0 when the input is completely consumed!");

        if (last_chunk)
        {
            break;
        }
    }
}

/*
 * @brief: Internal function to decompress the string in the local variable _input.
 */
void ZSTD::decompress_string()
{
    if (mInput.empty() == true)
    {
        mLogger->error("Magnus: Please use the correct constructor.");
        throw std::exception();
    }
    unsigned long long const r_size = ZSTD_getFrameContentSize(mInput.data(), mInput.size());
    CHECK(r_size != ZSTD_CONTENTSIZE_ERROR, "ZSTD: Not compressed by zstd!");
    CHECK(r_size != ZSTD_CONTENTSIZE_UNKNOWN, "ZSTD: Original size unknown!");

    mBuffer.resize(size_t(r_size));
    size_t const d_size = ZSTD_decompress(mBuffer.data(), r_size, mInput.data(), mInput.size());

    // When zstd knows the content size, it will error if it doesn't match.
    CHECK_ZSTD(d_size);
    CHECK(d_size == r_size, "Size does not match!");
}

/*
 * @brief: Internal function to decompress a file which had been opened in the constructor.
 */
void ZSTD::decompress_file()
{
    size_t const toRead = mBuffInSize;
    size_t read;

    while ((read = fread(mBuffIn, 1, toRead, mFin)))
    {

        ZSTD_inBuffer input = {mBuffIn, read, 0};

        while (input.pos < input.size)
        {
            ZSTD_outBuffer output = {mBuffOut, mBuffOutSize, 0};

            size_t const ret = ZSTD_decompressStream(mDctx, &output, &input);
            CHECK_ZSTD(ret);
            mBuffer.append(static_cast<const char*>(mBuffOut), output.pos);
        }
    }
}

/*
 * @brief: Getter function for the result of the operations
 * @returns: The result of the compression/decompression as a std::string_view
 */
std::string_view ZSTD::get_string_view()
{
    if (mBuffer.empty())
    {
        mLogger->error("ZSTD: There isn't any buffer to return.");
        throw std::exception();
    }
    return std::string_view(mBuffer);
}

/*
 * @brief: Getter function for the result of the operations
 * @returns: The result of the compression/decompression as a std::string
 */
std::string& ZSTD::get_string()
{
    if (mBuffer.empty())
    {
        mLogger->error("ZSTD: There isn't any buffer to return.");
        throw std::exception();
    }

    return mBuffer;
}
} // namespace Magnus::LibMagnus::Compression
