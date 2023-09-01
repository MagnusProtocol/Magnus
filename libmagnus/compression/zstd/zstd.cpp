#include <compression/zstd/zstd.hpp>
#include <iostream>
#include <zstd.h>
// TODO: Switch ZSTD File functions to C++ STDLIB-IO
#include <cstdio>

/**
 * @brief: Constructor for the ZSTDCompressor class.
 *         It currently creates the ZSTD Context
 */
ZSTDCompressor::ZSTDCompressor() {
        this->comp_level = 4;
        this->ZSTDCContext = ZSTD_createCCtx();
        CHECK(this->ZSTDCContext != NULL, "ZSTD_createCCtx() failed!");

        this->ZSTDDContext = ZSTD_createDCtx();
        CHECK(ZSTDDContext != NULL, "ZSTD_createDCtx() failed!");
#if defined(ZSTD_STATIC_LINKING_ONLY)
        size_t thread_comp =
            ZSTD_CCtx_refThreadPool(this->ZSTDCContext, this->ZSTDCPool);
        CHECK(thread_comp == 0, "ZSTD_CCtx_refThreadPool failed!");

        size_t thread_decomp =
            ZSTD_CCtx_refThreadPool(this->ZSTDDContext, this->ZSTDDPool);
        CHECK(thread_decomp == 0, "ZSTD_CCtx_refThreadPool failed!");
#endif
}

/**
 * @brief: Destructor for the ZSTDCompressor class.
 *         It currently cleans up the ZSTD Context
 */
ZSTDCompressor::~ZSTDCompressor() {
        ZSTD_freeCCtx(this->ZSTDCContext);
        ZSTD_freeDCtx(this->ZSTDDContext);
}

/**
 * @brief: Compress a string
 * @param input: Reference to a string that is to be compressed
 * @returns Compressed string
 */
std::string ZSTDCompressor::compress(std::string& input) {
        std::string comp_string;

        size_t comp_buff_size = ZSTD_compressBound(input.size());

        comp_string.resize(comp_buff_size);

        auto dstp =
            const_cast<void*>(static_cast<const void*>(comp_string.c_str()));
        auto srcp = static_cast<const void*>(input.c_str());

        size_t const comp_size =
            ZSTD_compressCCtx(this->ZSTDCContext, dstp, comp_buff_size, srcp,
                              input.size(), this->comp_level);
        CHECK_ZSTD(comp_size);

        comp_string.resize(comp_size);

        return comp_string;
}

/**
 * @brief: Decompress a string
 * @param input: Reference to a string that is to be decompressed
 * @returns Decompressed string
 */
std::string ZSTDCompressor::decompress(std::string& input) {
        std::string decomp_string;

        size_t const comp_buff_size =
            ZSTD_getFrameContentSize(input.c_str(), input.size());

        if (ZSTD_CONTENTSIZE_ERROR == comp_buff_size) {
                // TODO: error
        }

        decomp_string.resize(comp_buff_size);

        auto dstp =
            const_cast<void*>(static_cast<const void*>(decomp_string.c_str()));
        auto srcp = static_cast<const void*>(input.c_str());

        size_t const decomp_size = ZSTD_decompressDCtx(
            this->ZSTDDContext, dstp, comp_buff_size, srcp, input.size());
        CHECK_ZSTD(decomp_size);

        decomp_string.resize(decomp_size);

        return decomp_string;
}

/**
 * @brief: Compresses a file
 * @param path: std::filesystem::path to the file that is to be compressed
 * @returns Result of the operation (int)
 */
int ZSTDCompressor::compress(std::filesystem::path& path) {
        auto outfile = path.string() + ".zst";

        FILE* const fin = fopen(path.c_str(), "rb");
        FILE* const fout = fopen(outfile.c_str(), "wb");

        int nbThreads = 8;

        /**
         * Create the input and output buffers.
         */
        size_t const buff_in_size = ZSTD_CStreamInSize();
        void* const buff_in = malloc(buff_in_size);
        memset(buff_in, 0, buff_in_size);

        size_t const buff_out_size = ZSTD_CStreamOutSize();
        void* const buff_out = malloc(buff_out_size);
        memset(buff_out, 0, buff_out_size);

#if defined(ZSTD_STATIC_LINKING_ONLY)
        size_t r = ZSTD_CCtx_refThreadPool(this->ZSTDCContext, this->ZSTDCPool);
        CHECK(r == 0, "ZSTD_CCtx_refThreadPool failed!");
#endif

        /**
         * Set the compression level, and enable the checksum.
         */
        CHECK_ZSTD(ZSTD_CCtx_setParameter(
            this->ZSTDCContext, ZSTD_c_compressionLevel, this->comp_level));
        CHECK_ZSTD(
            ZSTD_CCtx_setParameter(this->ZSTDCContext, ZSTD_c_checksumFlag, 1));
        CHECK_ZSTD(ZSTD_CCtx_setParameter(this->ZSTDCContext, ZSTD_c_nbWorkers,
                                          nbThreads));

        /**
         * This loop reads from the input file, compresses that entire chunk,
         * and writes all output produced to the output file.
         */
        size_t const to_read = buff_in_size;
        for (;;) {
                size_t read = fread(buff_in, 1, to_read, fin);

                /**
                 * Select the flush mode.
                 * If the read may not be finished (read == to_read) we use
                 * ZSTD_e_continue. If this is the last chunk, we use
                 * ZSTD_e_end. Zstd optimizes the case where the first flush
                 * mode is ZSTD_e_end, since it knows it is compressing the
                 * entire source in one pass.
                 */
                int const last_chunk = (read < to_read);
                ZSTD_EndDirective const mode =
                    last_chunk ? ZSTD_e_end : ZSTD_e_continue;
                /**
                 * Set the input buffer to what we just read.
                 * We compress until the input buffer is empty, each time
                 * flushing the output.
                 */
                ZSTD_inBuffer input = {buff_in, read, 0};
                int finished;
                do {
                        /**
                         * Compress into the output buffer and write all of the
                         * output to the file so we can reuse the buffer next
                         * iteration.
                         */
                        ZSTD_outBuffer output = {buff_out, buff_out_size, 0};
                        size_t const remaining = ZSTD_compressStream2(
                            this->ZSTDCContext, &output, &input, mode);
                        CHECK_ZSTD(remaining);

                        fwrite(buff_out, 1, output.pos, fout);
                        /**
                         * If we're on the last chunk we're finished when zstd
                         * returns 0, which means its consumed all the input AND
                         * finished the frame. Otherwise, we're finished when
                         * we've consumed all the input.
                         */
                        finished = last_chunk ? (remaining == 0)
                                              : (input.pos == input.size);
                } while (!finished);
                CHECK(input.pos == input.size,
                      "Impossible: zstd only returns 0 when the input is "
                      "completely consumed!");

                if (last_chunk) {
                        break;
                }
        }

        fclose(fin);
        fclose(fout);
        free(buff_in);
        free(buff_out);

        return 0;
}

/**
 * @brief: Decompress a file
 * @param path: std::filesystem::path to the file that is to be decompressed
 * @returns Result of the operation (int)
 */
int ZSTDCompressor::decompress(std::filesystem::path& path) {
        auto outfile = path;
        outfile.replace_extension("decomp");

        FILE* const fin = fopen(path.c_str(), "rb");
        FILE* const fout = fopen(outfile.c_str(), "wb");

        /**
         * Create IO Buffers
         */
        size_t const buff_in_size = ZSTD_DStreamInSize();
        void* const buff_in = malloc(buff_in_size);
        memset(buff_in, 0, buff_in_size);

        size_t const buff_out_size = ZSTD_DStreamOutSize();
        void* const buff_out = malloc(buff_out_size);
        memset(buff_out, 0, buff_out_size);

        /** This loop assumes that the input file is one or more concatenated
         * zstd streams. This example won't work if there is trailing non-zstd
         * data at the end, but streaming decompression in general handles this
         * case. ZSTD_decompressStream() returns 0 exactly when the frame is
         * completed, and doesn't consume input after the frame.
         */
        size_t const to_read = buff_in_size;
        size_t read;
        size_t last_ret = 0;
        int is_empty = 1;
        while ((read = fread(buff_in, 1, to_read, fin))) {
                is_empty = 0;
                ZSTD_inBuffer input = {buff_in, read, 0};
                /** Given a valid frame, zstd won't consume the last byte of the
                 * frame until it has flushed all of the decompressed data of
                 * the frame. Therefore, instead of checking if the return code
                 * is 0, we can decompress just check if input.pos < input.size.
                 */
                while (input.pos < input.size) {
                        ZSTD_outBuffer output = {buff_out, buff_out_size, 0};
                        /** The return code is zero if the frame is complete,
                         * but there may be multiple frames concatenated
                         * together. Zstd will automatically reset the context
                         * when a frame is complete. Still, calling
                         * ZSTD_DCtx_reset() can be useful to reset the context
                         * to a clean state, for instance if the last
                         * decompression call returned an error.
                         */
                        size_t const ret = ZSTD_decompressStream(
                            this->ZSTDDContext, &output, &input);
                        CHECK_ZSTD(ret);
                        fwrite(buff_out, 1, output.pos, fout);
                        last_ret = ret;
                }
        }

        if (is_empty) {
                fprintf(stderr, "input is empty\n");
                exit(1);
        }

        if (last_ret != 0) {
                /** The last return value from ZSTD_decompressStream did not end
                 * on a frame, but we reached the end of the file! We assume
                 * this is an error, and the input was truncated.
                 */
                fprintf(stderr, "EOF before end of stream: %zu\n", last_ret);
                exit(1);
        }

        fclose(fin);
        fclose(fout);
        free(buff_in);
        free(buff_out);

        return 0;
}

/**
 * @brief: Compresses multiple files
 * @param path: std::vector of std::filesystem::path to the files that are to be
 * compressed
 * @returns Result of the operation (int)
 */
int ZSTDCompressor::compress(std::vector<std::filesystem::path> paths) {
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
int ZSTDCompressor::decompress(std::vector<std::filesystem::path> paths) {
        auto res = 0;
        for (auto path : paths) {
                res = decompress(path);
        }
        return res;
}
