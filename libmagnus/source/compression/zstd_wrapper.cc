#include "compression/zstd_wrapper.hh"

ZSTD::ZSTD(std::string_view& input)
{
    _input = input;
    _buffer = new std::string;
    _cctx = ZSTD_createCCtx();
    _dctx = ZSTD_createDCtx();
    CHECK(_cctx != NULL, "ZSTD_createCCtx() failed!");
}

ZSTD::~ZSTD()
{
    ZSTD_freeCCtx(_cctx);
    ZSTD_freeDCtx(_dctx);
    delete _buffer;
}

void ZSTD::compress()
{
    const size_t buffer_size = ZSTD_compressBound(_input.size());
    _buffer->resize(buffer_size);

    size_t const c_size = ZSTD_compressCCtx(_cctx, _buffer->data(), buffer_size, _input.data(), _input.size(), 2);
    CHECK_ZSTD(c_size);
    _buffer->resize(c_size);
}

void ZSTD::decompress()
{
    unsigned long long const r_size = ZSTD_getFrameContentSize(_input.data(), _input.size());
    CHECK(r_size != ZSTD_CONTENTSIZE_ERROR, "%s: not compressed by zstd!");
    CHECK(r_size != ZSTD_CONTENTSIZE_UNKNOWN, "%s: original size unknown!");

    _buffer->resize(size_t(r_size));
    size_t const d_size = ZSTD_decompressDCtx(_dctx, _buffer->data(), r_size, _input.data(), _input.size());
    CHECK_ZSTD(d_size);
    // When zstd knows the content size, it will error if it doesn't match.
    CHECK(d_size == r_size, "Size does not match!");
}

std::string_view ZSTD::get_string_view()
{
    if (!_buffer) {
        throw std::runtime_error("[ERROR] ZSTD: There isn't any buffer to return.");
    }
    return std::string_view(*_buffer);
}
std::string* ZSTD::get_string()
{
    if (!_buffer) {
        throw std::runtime_error("[ERROR] ZSTD: There isn't any buffer to return.");
    }
    return _buffer;
}
