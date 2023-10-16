#pragma once

#include <lz4.h>
#include <string>
#include <compression/compression.hpp>

class LZ4Compressor  {
public:
        LZ4Compressor();
        ~LZ4Compressor();

        /**
         * @brief: Compress a string
         * @param input: Reference to a string that is to be compressed
         * @returns Compressed string
         */
        std::string compress(std::string& src);

        /**
         * @brief: Decompress a string
         * @param input: Reference to a string that is to be decompressed
         * @returns Decompressed string
         */
        std::string decompress(std::string& input, int src_size);


};

