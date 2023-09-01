#pragma once

#include <compression/compression.hpp>
#include <zstd.h>

class ZSTDCompressor : Compressor {
public:
        ZSTDCompressor();
        ~ZSTDCompressor();

        /**
         * @brief: Compress a string
         * @param input: Reference to a string that is to be compressed
         * @returns Compressed string
         */
        std::string compress(std::string& input);

        /**
         * @brief: Decompress a string
         * @param input: Reference to a string that is to be decompressed
         * @returns Decompressed string
         */
        std::string decompress(std::string& input);

        /**
         * @brief: Compresses a file
         * @param path: std::filesystem::path to the file that is to be
         * compressed
         * @returns Result of the operation (int)
         */
        int compress(std::filesystem::path& path);

        /**
         * @brief: Decompress a file
         * @param path: std::filesystem::path to the file that is to be
         * decompressed
         * @returns Result of the operation (int)
         */
        int decompress(std::filesystem::path& path);

        /**
         * @brief: Compresses multiple files
         * @param path: std::vector of std::filesystem::path to the files that
         * are to be compressed
         * @returns Result of the operation (int)
         */
        int compress(std::vector<std::filesystem::path> paths);

        /**
         * @brief: Decompresses multiple files
         * @param path: std::vector of std::filesystem::path to the files that
         * are to be decompressed
         * @returns Result of the operation (int)
         */
        int decompress(std::vector<std::filesystem::path> paths);

private:
        int comp_level;

        // Temp helper func
        size_t fread_or_die(void* buffer, size_t sizeToRead, FILE* file) {
                size_t const readSize = fread(buffer, 1, sizeToRead, file);
                if (readSize == sizeToRead)
                        return readSize; /** good */
                if (feof(file))
                        return readSize; /** good, reached end of file */
                /** error */
                perror("fread");
                exit(-1);
        }
        size_t fwrite_or_die(const void* buffer, size_t sizeToWrite,
                             FILE* file) {
                size_t const writtenSize = fwrite(buffer, 1, sizeToWrite, file);
                if (writtenSize == sizeToWrite)
                        return sizeToWrite; /** good */
                /** error */
                perror("fwrite");
                exit(-1);
        }

        ZSTD_CCtx* ZSTDCContext;
        ZSTD_DCtx* ZSTDDContext;

#if defined(ZSTD_STATIC_LINKING_ONLY)
        ZSTD_threadPool* ZSTDCPool;
        ZSTD_threadPool* ZSTDDPool;
#endif
};

/**
 * @ref: https://github.com/facebook/zstd/blob/dev/examples/common.h
 */

/**! CHECK
 * Check that the condition holds. If it doesn't print a message and die.
 */
#define CHECK(cond, ...)                                                       \
        do {                                                                   \
                if (!(cond)) {                                                 \
                        fprintf(stderr, "%s:%d CHECK(%s) failed: ", __FILE__,  \
                                __LINE__, #cond);                              \
                        fprintf(stderr, "" __VA_ARGS__);                       \
                        fprintf(stderr, "\n");                                 \
                        exit(1);                                               \
                }                                                              \
        } while (0)

/**! CHECK_ZSTD
 * Check the zstd error code and die if an error occurred after printing a
 * message.
 */
#define CHECK_ZSTD(fn)                                                         \
        do {                                                                   \
                size_t const err = (fn);                                       \
                CHECK(!ZSTD_isError(err), "%s", ZSTD_getErrorName(err));       \
        } while (0)
