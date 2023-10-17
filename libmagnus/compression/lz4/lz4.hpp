#pragma once

#include <compression/compression.hpp>

class LZ4Compressor {
public:
	LZ4Compressor();
	~LZ4Compressor();

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
	std::string decompress(std::string& input, int src_size);

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
};
