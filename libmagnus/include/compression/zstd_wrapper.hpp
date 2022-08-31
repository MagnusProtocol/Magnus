#pragma once
#include "compression.hpp"
#include <filesystem>
#include <zstd.h>

namespace LibMagnus::Compression
{

enum MODES
{
	COMPRESS = 0x123,
	DECOMPRESS = 0x234,
};

class ZSTD : public CompressionBase
{
	private:
	// String buffer for storing the comrpessed data
	std::string mBuffer;
	// String view for taking in a string to compress
	std::string_view mInput;
	// Path to the file to be compressed
	std::filesystem::path mFilename;

	// Compression context
	ZSTD_CCtx* mCctx;
	// Decompression context
	ZSTD_DCtx* mDctx;

	// Input & output buffers for files
	void* mBuffIn = nullptr;
	void* mBuffOut = nullptr;

	// Sizes for both the buffers
	size_t mBuffInSize;
	size_t mBuffOutSize;

	// File pointer in case we need it
	FILE* mFin;

	/*
	 * @brief: Internal function to compress a file which had been opened in the constructor.
	 */
	void compress_string();

	/*
	 * @brief: Internal function to decompress the string in the local variable _input.
	 */
	void decompress_string();

	/*
	 * @brief: Internal function to compress a file which had been opened in the constructor.
	 */
	void compress_file();

	/*
	 * @brief: Internal function to decompress a file which had been opened in the constructor.
	 */
	void decompress_file();

	void setup();

	// Spdlog logger, multithreaded stderr
	std::shared_ptr<spdlog::logger> mLogger;

	public:
	/*
	 * @brief: Compress/Decompress a std::string_view; to obtain the result of this function, call
	 * the applicable getter function.
	 * @param input: the string_view to be compressed/decompressed
	 * @param mode: To compress or to decompress, that is the question.
	 */
	ZSTD(std::string_view input, MODES mode);

	/*
	 * @brief: Compress/Decompress a std::string_view; to obtain the result of this function, call
	 * the applicable getter function.
	 * @param input: the string to be compressed/decompressed
	 * @param mode: To compress or to decompress, that is the question.
	 */
	ZSTD(std::string& input, MODES mode);

	/*
	 * @brief: Compress/Decompress file; to obtain the result of this function, call the applicable
	 * getter function.
	 * @param filename: Path to the file on which the operation has to be performed
	 * @param mode: To compress or to decompress, that is the question.
	 */
	ZSTD(std::filesystem::path filename, MODES mode);

	/*
	 * @brief: Class destructor, does all the cleanup
	 */
	~ZSTD();

	/*
	 * @brief: Getter function for the result of the operations
	 * @returns: The result of the compression/decompression as a std::string_view
	 */
	std::string_view get_string_view();

	/*
	 * @brief: Getter function for the result of the operations
	 * @returns: The result of the compression/decompression as a std::string
	 */
	std::string get_string();
};
}; // namespace LibMagnus::Compression
