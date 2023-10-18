#include <compression/zstd/zstd.hpp>

ZSTDCompressor::ZSTDCompressor()
	: ZSTDCContext(ZSTD_createCCtx()), ZSTDDContext(ZSTD_createDCtx()) {

	check(this->ZSTDCContext != nullptr, "ZSTD_createCCtx() failed!");
	check(this->ZSTDDContext != nullptr, "ZSTD_createDCtx() failed!");
}

ZSTDCompressor::~ZSTDCompressor() {
	ZSTD_freeCCtx(this->ZSTDCContext);
	ZSTD_freeDCtx(this->ZSTDDContext);
}

auto ZSTDCompressor::compress(const std::string_view input) -> std::string {
	const size_t comp_buff_size = ZSTD_compressBound(input.size());

	std::string comp_string(comp_buff_size, ' ');

	const size_t comp_size = ZSTD_compressCCtx(
		this->ZSTDCContext, comp_string.data(), comp_buff_size, input.data(),
		input.size(), ZSTDCompressor::comp_level);
	check_zstd(comp_size);

	comp_string.resize(comp_size);

	return comp_string;
}

auto ZSTDCompressor::decompress(const std::string_view input) -> std::string {

	const size_t comp_buff_size =
		ZSTD_getFrameContentSize(input.data(), input.size());

	if (ZSTD_CONTENTSIZE_ERROR == comp_buff_size) {
		throw std::length_error("ZSTD Content size error");
	}

	std::string decomp_string(comp_buff_size, ' ');

	const size_t decomp_size =
		ZSTD_decompressDCtx(this->ZSTDDContext, decomp_string.data(),
							comp_buff_size, input.data(), input.size());
	check_zstd(decomp_size);

	decomp_string.resize(decomp_size);

	return decomp_string;
}

auto ZSTDCompressor::compress(std::filesystem::path& path) -> int {
	auto outfile = path.string() + ".zst";

	const unique_file_t fin{std::fopen(path.string().c_str(), "rb")};
	const unique_file_t fout{std::fopen(outfile.c_str(), "wb")};

	const int nbThreads = 8;

	/**
	 * Create the input and output buffers.
	 */
	const size_t buff_in_size = ZSTD_CStreamInSize();
	std::vector<char> buff_in(buff_in_size, ' ');

	const size_t buff_out_size = ZSTD_CStreamOutSize();
	std::vector<char> buff_out(buff_out_size, ' ');

	/**
	 * Set the compression level, and enable the checksum.
	 */
	check_zstd(ZSTD_CCtx_setParameter(this->ZSTDCContext,
									  ZSTD_c_compressionLevel,
									  ZSTDCompressor::comp_level));
	check_zstd(
		ZSTD_CCtx_setParameter(this->ZSTDCContext, ZSTD_c_checksumFlag, 1));
	check_zstd(ZSTD_CCtx_setParameter(this->ZSTDCContext, ZSTD_c_nbWorkers,
									  nbThreads));

	/**
	 * This loop reads from the input file, compresses that entire chunk,
	 * and writes all output produced to the output file.
	 */
	size_t const to_read = buff_in_size;
	for (;;) {
		const size_t read = fread(buff_in.data(), 1, to_read, fin.get());

		/**
		 * Select the flush mode.
		 * If the read may not be finished (read == to_read) we use
		 * ZSTD_e_continue. If this is the last chunk, we use
		 * ZSTD_e_end. Zstd optimizes the case where the first flush
		 * mode is ZSTD_e_end, since it knows it is compressing the
		 * entire source in one pass.
		 */
		const bool last_chunk = (read < to_read);
		const ZSTD_EndDirective mode =
			last_chunk ? ZSTD_e_end : ZSTD_e_continue;
		/**
		 * Set the input buffer to what we just read.
		 * We compress until the input buffer is empty, each time
		 * flushing the output.
		 */
		ZSTD_inBuffer input{buff_in.data(), read, 0};
		bool finished = false;
		size_t written = 0;

		while (!finished) {
			/**
			 * Compress into the output buffer and write all of the
			 * output to the file so we can reuse the buffer next
			 * iteration.
			 */
			ZSTD_outBuffer output{buff_out.data(), buff_out_size, 0};
			size_t const remaining =
				ZSTD_compressStream2(this->ZSTDCContext, &output, &input, mode);
			check_zstd(remaining);

			written = fwrite(buff_out.data(), 1, output.pos, fout.get());

			assert(written >= output.pos);
			/**
			 * If we're on the last chunk we're finished when zstd
			 * returns 0, which means its consumed all the input AND
			 * finished the frame. Otherwise, we're finished when
			 * we've consumed all the input.
			 */
			finished =
				last_chunk ? (remaining == 0) : (input.pos == input.size);
		};

		check(input.pos == input.size,
			  "Impossible: zstd only returns 0 when the input is "
			  "completely consumed!");

		if (last_chunk) {
			break;
		}
	}

	return 0;
}

auto ZSTDCompressor::decompress(std::filesystem::path& path) -> int {
	auto outfile{path};
	outfile.replace_extension(std::filesystem::path("decomp"));

	const unique_file_t fin{std::fopen(path.string().c_str(), "rb")};
	const unique_file_t fout{std::fopen(outfile.string().c_str(), "wb")};

	/**
	 * Create IO Buffers
	 */
	const size_t buff_in_size = ZSTD_DStreamInSize();
	std::vector<char> buff_in(buff_in_size, ' ');

	const size_t buff_out_size = ZSTD_DStreamOutSize();
	std::vector<char> buff_out(buff_out_size, ' ');

	/** This loop assumes that the input file is one or more concatenated
	 * zstd streams. This example won't work if there is trailing non-zstd
	 * data at the end, but streaming decompression in general handles this
	 * case. ZSTD_decompressStream() returns 0 exactly when the frame is
	 * completed, and doesn't consume input after the frame.
	 */
	const size_t to_read = buff_in_size;
	size_t read = 0;
	size_t last_ret = 0;
	size_t written = 0;

	int is_empty = 1;
	while ((read = fread(buff_in.data(), 1, to_read, fin.get())) != 0) {
		is_empty = 0;
		ZSTD_inBuffer input{buff_in.data(), read, 0};
		/** Given a valid frame, zstd won't consume the last byte of the
		 * frame until it has flushed all of the decompressed data of
		 * the frame. Therefore, instead of checking if the return code
		 * is 0, we can decompress just check if input.pos < input.size.
		 */
		while (input.pos < input.size) {
			ZSTD_outBuffer output{buff_out.data(), buff_out_size, 0};
			/** The return code is zero if the frame is complete,
			 * but there may be multiple frames concatenated
			 * together. Zstd will automatically reset the context
			 * when a frame is complete. Still, calling
			 * ZSTD_DCtx_reset() can be useful to reset the context
			 * to a clean state, for instance if the last
			 * decompression call returned an error.
			 */
			const size_t ret =
				ZSTD_decompressStream(this->ZSTDDContext, &output, &input);
			check_zstd(ret);

			written = fwrite(buff_out.data(), 1, output.pos, fout.get());

			assert(written != read);

			last_ret = ret;
		}
	}

	if (is_empty != 0) {
		throw std::filesystem::filesystem_error("Input is empty",
												std::error_code());
	}

	if (last_ret != 0) {
		/** The last return value from ZSTD_decompressStream did not end
		 * on a frame, but we reached the end of the file! We assume
		 * this is an error, and the input was truncated.
		 */
		throw std::filesystem::filesystem_error("EOF before end of stream",
												std::error_code());
	}

	return 0;
}

auto ZSTDCompressor::compress(std::vector<std::filesystem::path> paths) -> int {
	auto res = 0;
	for (auto path : paths) {
		res = compress(path);
	}
	return res;
}

auto ZSTDCompressor::decompress(std::vector<std::filesystem::path> paths)
	-> int {
	auto res = 0;
	for (auto path : paths) {
		res = decompress(path);
	}
	return res;
}
