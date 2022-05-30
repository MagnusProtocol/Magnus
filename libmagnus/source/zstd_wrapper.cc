#include <zstd_wrapper.hh>

ZSTD::ZSTD()
{
    _buffer = new std::string;
}

ZSTD::~ZSTD()
{
    delete _buffer;
}

void ZSTD::compress_single(std::string_view& input)
{
    size_t const buffer_size = ZSTD_compressBound(input.size());
    _buffer->resize(buffer_size);

    size_t const c_size = ZSTD_compress(_buffer->data(), buffer_size, input.data(), input.size(), 2);
    CHECK_ZSTD(c_size);
    _buffer->resize(c_size);
}

void ZSTD::decompress_single(std::string_view& input)
{
    unsigned long long const rSize = ZSTD_getFrameContentSize(input.data(), input.size());
    CHECK(rSize != ZSTD_CONTENTSIZE_ERROR, "%s: not compressed by zstd!");
    CHECK(rSize != ZSTD_CONTENTSIZE_UNKNOWN, "%s: original size unknown!");

    _buffer->resize(size_t(rSize));
    size_t const dSize = ZSTD_decompress(_buffer->data(), rSize, input.data(), input.size());
    CHECK_ZSTD(dSize);
    // When zstd knows the content size, it will error if it doesn't match.
    CHECK(dSize == rSize, "Size does not match!");
}

std::string_view ZSTD::get_string_view()
{
    if (!_buffer) {
        std::cerr << "[ERROR]: There isn't any buffer to return." << std::endl;
    }
    return std::string_view(*_buffer);
}
std::string* ZSTD::get_string()
{
    if (!_buffer) {
        std::cerr << "[ERROR]: There isn't any buffer to return." << std::endl;
    }
    return _buffer;
}
