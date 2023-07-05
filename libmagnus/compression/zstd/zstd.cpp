#include <compression/zstd/zstd.hpp>
#include <zstd.h>
#include <iostream>

/*
 * @brief: Compress a string
 * @param input: Reference to a string that is to be compressed
 * @returns Compressed string
 */
std::string ZSTDCompressor::compress(std::string& input) {
        std::string comp_string; 

        size_t comp_buff_size = ZSTD_compressBound(input.size());
        auto comp_level = 4;

        comp_string.resize(comp_buff_size);

        auto dstp = const_cast<void*>(static_cast<const void*>(comp_string.c_str()));
        auto srcp = static_cast<const void*>(input.c_str());
        
        size_t const comp_size = ZSTD_compress(dstp, comp_buff_size, srcp, input.size(), 1);
        CHECK_ZSTD(comp_size);
        
        comp_string.resize(comp_size);

        return comp_string;
}

/*
 * @brief: Decompress a string
 * @param input: Reference to a string that is to be decompressed
 * @returns Decompressed string
 */
std::string ZSTDCompressor::decompress(std::string& input) {
        std::string decomp_string;

        size_t const comp_buff_size = ZSTD_getFrameContentSize(input.c_str(), input.size());

        if (ZSTD_CONTENTSIZE_ERROR == comp_buff_size) {
                // TODO: error
        }
        
        decomp_string.resize(comp_buff_size);

        auto dstp = const_cast<void*>(static_cast<const void*>(decomp_string.c_str()));
        auto srcp = static_cast<const void*>(input.c_str());
        
        size_t const decomp_size = ZSTD_decompress(dstp, comp_buff_size, srcp, input.size());
        CHECK_ZSTD(decomp_size);
        
        decomp_string.resize(decomp_size);
        
        return decomp_string;
}

/*
 * @brief: Compresses a file
 * @param path: std::filesystem::path to the file that is to be compressed
 * @returns Result of the operation (int)
 */
int ZSTDCompressor::compress(std::filesystem::path& path) {
}

/*
 * @brief: Decompress a file
 * @param path: std::filesystem::path to the file that is to be decompressed
 * @returns Result of the operation (int)
 */
int ZSTDCompressor::decompress(std::filesystem::path& path) {}

/*
 * @brief: Compresses multiple files
 * @param path: std::vector of std::filesystem::path to the files that are to be compressed
 * @returns Result of the operation (int)
 */
int ZSTDCompressor::compress(std::vector<std::filesystem::path> paths) {}

/*
 * @brief: Decompresses multiple files
 * @param path: std::vector of std::filesystem::path to the files that are to be decompressed
 * @returns Result of the operation (int)
 */
int ZSTDCompressor::decompress(std::vector<std::filesystem::path> paths) {}
