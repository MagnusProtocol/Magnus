#include "lz4.hpp"
#include "lz4.h"
#include <cstring>
#include <iostream>
#include <lz4.c>
#include "lz4frame.c"
#include "lz4hc.c"
#include "xxhash.c"
#include "lz4file.c"
#include "lz4frame.h"

#define CHUNK_SIZE (16*1024)

LZ4Compressor::LZ4Compressor() {

}
LZ4Compressor::~LZ4Compressor() {

}
std::string LZ4Compressor::compress(std::string& src) {

        const int src_size = src.size();
        const size_t max_dst_size = LZ4_compressBound(src_size);
        // Now build allocations for the data we'll be playing with.
        char *dst = new char[max_dst_size];
        std::memset(dst, 0, max_dst_size);
        int bytes_returned = LZ4_compress_fast(src.c_str(), dst, src_size, max_dst_size, 1);

        std::string comp_str(dst, bytes_returned);
        delete[] dst;
        return comp_str;
}
/**
         * @brief: Decompress a string
         * @param input: Reference to a string that is to be decompressed
         * @returns Decompressed string
 */
std::string LZ4Compressor::decompress(std::string& input, int src_size){

        char* const regen_buffer = new char[src_size];
        // The LZ4_decompress_safe function needs to know where the compressed data is, how many bytes long it is,
        // where the regen_buffer memory location is, and how large regen_buffer (uncompressed) output will be.
        // Again, save the return_value.
        const int decompressed_size = LZ4_decompress_safe(input.c_str(), regen_buffer, input.size(),src_size );
        std::string decomp_str(regen_buffer, decompressed_size);
        delete[] regen_buffer;
        return decomp_str;
};

int LZ4Compressor::compress(std::filesystem::path& path){
        auto outfile = path.string() + ".lz4";

        FILE* const f_in = fopen(path.string().c_str(), "rb");
        FILE* const f_out = fopen(outfile.c_str(), "wb");

        assert(f_in != NULL); assert(f_out != NULL);

        LZ4F_errorCode_t ret = LZ4F_OK_NoError;
        size_t len;
        LZ4_writeFile_t* lz4fWrite;
        void* const buf = malloc(CHUNK_SIZE);
        if (!buf) {
                printf("error: memory allocation failed \n");
                return 1;
        }

        /* Of course, you can also use prefsPtr to
     * set the parameters of the compressed file
     * NULL is use default
         */
        ret = LZ4F_writeOpen(&lz4fWrite, f_out, NULL);
        if (LZ4F_isError(ret)) {
                printf("LZ4F_writeOpen error: %s\n", LZ4F_getErrorName(ret));
                free(buf);
                return 1;
        }

        while (1) {
                len = fread(buf, 1, CHUNK_SIZE, f_in);

                if (ferror(f_in)) {
                        printf("fread error\n");
                        goto out;
                }

                /* nothing to read */
                if (len == 0) {
                        break;
                }

                ret = LZ4F_write(lz4fWrite, buf, len);
                if (LZ4F_isError(ret)) {
                        printf("LZ4F_write: %s\n", LZ4F_getErrorName(ret));
                        goto out;
                }

        }

out:
        free(buf);
        if (LZ4F_isError(LZ4F_writeClose(lz4fWrite))) {
                printf("LZ4F_writeClose: %s\n", LZ4F_getErrorName(ret));
                return 1;
        }
        fflush(f_out);
        return 0;
}
int LZ4Compressor::decompress(std::filesystem::path& path) {
        auto outfile = path;
        outfile.replace_extension("decomp");

        FILE* const f_in = fopen(path.string().c_str(), "rb");
        FILE* const f_out = fopen(outfile.string().c_str(), "wb");
        assert(f_in != NULL);
        assert(f_out != NULL);
        std::cout << path;
        LZ4F_errorCode_t ret = LZ4F_OK_NoError;
        LZ4_readFile_t* lz4fRead;
        void* const buf = malloc(CHUNK_SIZE);
        if (!buf) {
                printf("error: memory allocation failed \n");
        }

        ret = LZ4F_readOpen(&lz4fRead, f_in);
        if (LZ4F_isError(ret)) {
                printf("LZ4F_readOpen error: %s\n", LZ4F_getErrorName(ret));
                free(buf);
                return 1;
        }

        while (1) {
                ret = LZ4F_read(lz4fRead, buf, CHUNK_SIZE);
                if (LZ4F_isError(ret)) {
                        printf("LZ4F_read error: %s\n", LZ4F_getErrorName(ret));
                        goto out;
                }

                /* nothing to read */
                if (ret == 0) {
                        break;
                }

                if (fwrite(buf, 1, ret, f_out) != ret) {
                        printf("write error!\n");
                        goto out;
                }
                fflush(f_out);
        }

out:
        free(buf);
        if (LZ4F_isError(LZ4F_readClose(lz4fRead))) {
                printf("LZ4F_readClose: %s\n", LZ4F_getErrorName(ret));
                return 1;
        }

        if (ret) {
                return 1;
        }

        return 0;
}
/**
 * @brief: Compresses multiple files
 * @param path: std::vector of std::filesystem::path to the files that are to be
 * compressed
 * @returns Result of the operation (int)
 */
int LZ4Compressor::compress(std::vector<std::filesystem::path> paths) {
        auto res = 0;
        for (auto path : paths) {
                res = compress(path);
        }
        return res;
}

/**
 * @brief: Decompresses multiple files
 * @param path: std::vector of std::filesystem::path to the files that are to be
 * decompressed
 * @returns Result of the operation (int)
 */
int LZ4Compressor::decompress(std::vector<std::filesystem::path> paths) {
        auto res = 0;
        for (auto path : paths) {
                res = decompress(path);
        }
        return res;
}
