#pragma once

#include <filesystem>
#include <string>

class Compressor {
public:
        /**
         * @brief: Compress a string
         * @param input: Reference to a string that is to be compressed
         * @returns Compressed string
         */
        virtual std::string compress(std::string& input) = 0;

        /**
         * @brief: Decompress a string
         * @param input: Reference to a string that is to be decompressed
         * @returns Decompressed string
         */
        virtual std::string decompress(std::string& input) = 0;

        /**
         * @brief: Compresses a file
         * @param path: std::filesystem::path to the file that is to be
         * compressed
         * @returns Result of the operation (int)
         */
        virtual int compress(std::filesystem::path& path) = 0;

        /**
         * @brief: Decompress a file
         * @param path: std::filesystem::path to the file that is to be
         * decompressed
         * @returns Result of the operation (int)
         */
        virtual int decompress(std::filesystem::path& path) = 0;

        /**
         * @brief: Compresses multiple files
         * @param path: std::vector of std::filesystem::path to the files that
         * are to be compressed
         * @returns Result of the operation (int)
         */
        virtual int compress(std::vector<std::filesystem::path> paths) = 0;

        /**
         * @brief: Decompresses multiple files
         * @param path: std::vector of std::filesystem::path to the files that
         * are to be decompressed
         * @returns Result of the operation (int)
         */
        virtual int decompress(std::vector<std::filesystem::path> paths) = 0;

        /**
         * @brief: Insertion operator overload for compression
         * @param os: std::ostream to write to
         * @ref: https://en.cppreference.com/w/cpp/language/operators
         */
        // virtual std::ostream& operator<<(std::ostream& os, const T& obj);

        /**
         * @brief: Extraction operator overload for decompression
         * @simple:
         * @param os: std::ostream to read from
         * @ref: https://en.cppreference.com/w/cpp/language/operators
         * @usage:
         * // Let's presume we have to compress a file
         * // variable `istream` is the file to compress
         * // variable `ostream` is the file to write the compressed data to
         * // variable `compressor` is an instance of some Compressor class
         * derivative
         * // The API will make it such that:
         *
         * istream << compressor << ostream;
         *
         */
        // virtual std::istream& operator>>(std::istream& is, T& obj);
};
