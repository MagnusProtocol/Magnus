#include "lz4.hpp"
#include "lz4.h"
#include <cstring>
#include <iostream>
#include <lz4.c>
#include <lz4file.h>

#define BUFFER_SIZE 2048

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

int LZ4Compressor::filecompress(FILE* inp, FILE* out){
//        LZ4_writeFile_t;
//        return 0;
}
