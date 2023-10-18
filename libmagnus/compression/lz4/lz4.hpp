#pragma once

#include <compression/compression.hpp>

class LZ4Compressor : Compressor {
public:
	LZ4Compressor() = default;
	~LZ4Compressor() = default;

	/**
	 * @brief: Compress a string
	 * @param input: Reference to a string that is to be compressed
	 * @returns Compressed string
	 */
	auto compress(const std::string_view input) -> std::string;

	/**
	 * @brief: Decompress a string
	 * @param input: Reference to a string that is to be decompressed
	 * @returns Decompressed string
	 */
	auto decompress(const std::string_view input) -> std::string;

	/**
	 * @brief: Decompress a string
	 * @param input: Reference to a string that is to be decompressed
	 * @param src_size: Size of the input before compression
	 * @returns Decompressed string
	 */
	auto decompress(const std::string_view input, const int src_size)
		-> std::string;

	/**
	 * @brief: Compresses a file
	 * @param path: std::filesystem::path to the file that is to be
	 * compressed
	 * @returns Result of the operation (int)
	 */
	auto compress(std::filesystem::path& path) -> int;

	/**
	 * @brief: Decompress a file
	 * @param path: std::filesystem::path to the file that is to be
	 * decompressed
	 * @returns Result of the operation (int)
	 */
	auto decompress(std::filesystem::path& path) -> int;

	/**
	 * @brief: Compresses multiple files
	 * @param path: std::vector of std::filesystem::path to the files that
	 * are to be compressed
	 * @returns Result of the operation (int)
	 */
	auto compress(const std::vector<std::filesystem::path>& paths) -> int;

	/**
	 * @brief: Decompresses multiple files
	 * @param path: std::vector of std::filesystem::path to the files that
	 * are to be decompressed
	 * @returns Result of the operation (int)
	 */
	auto decompress(const std::vector<std::filesystem::path>& paths) -> int;
};
