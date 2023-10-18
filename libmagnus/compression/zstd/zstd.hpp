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
	auto compress(const std::string_view input) -> std::string;

	/**
	 * @brief: Decompress a string
	 * @param input: Reference to a string that is to be decompressed
	 * @returns Decompressed string
	 */
	auto decompress(const std::string_view input) -> std::string;

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
	auto compress(std::vector<std::filesystem::path> paths) -> int;

	/**
	 * @brief: Decompresses multiple files
	 * @param path: std::vector of std::filesystem::path to the files that
	 * are to be decompressed
	 * @returns Result of the operation (int)
	 */
	auto decompress(std::vector<std::filesystem::path> paths) -> int;

private:
	static constexpr int comp_level = 4;

	ZSTD_CCtx* ZSTDCContext;
	ZSTD_DCtx* ZSTDDContext;

	/**
	 * @ref: https://github.com/facebook/zstd/blob/dev/examples/common.h
	 */
	template <typename... Args>
	inline void check(bool cond, const Args&... args) {
		if (!cond) {
			std::fprintf(stderr, "%s:%d CHECK(%d) failed: ", __FILE__, __LINE__,
						 cond);
			(std::cerr << ... << args);
			std::cerr << '\n';
			std::fprintf(stderr, "\n");
			std::exit(1);
		}
	}

	inline void check_zstd(size_t result) {
		check(!ZSTD_isError(result),
			  "Error in Zstd operation: ", ZSTD_getErrorName(result));
	}
};
