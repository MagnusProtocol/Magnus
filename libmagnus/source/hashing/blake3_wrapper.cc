#include "hashing/blake3_wrapper.hh"
#include <array>
#include <blake3.h>

namespace Magnus::LibMagnus::Hashing {
BLAKE3::BLAKE3(std::string_view input)
    : _input(input)
{
    blake3_hasher_init(&_hasher);
}

BLAKE3::~BLAKE3()
{
}
std::string BLAKE3::uint8_vector_to_hex_string(const std::vector<uint8_t>& v)
{
    std::stringstream os;
    for (size_t i = 0; i < BLAKE3_OUT_LEN; i++) {
        std::ios_base::fmtflags f(os.flags()); // Init

        os << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)v[i];

        os.flags(f); // restore
    }
    return os.str();
}

std::string BLAKE3::hash()
{
    blake3_hasher_update(&_hasher, _input.data(), _input.size());

    _buffer.resize(BLAKE3_OUT_LEN);
    blake3_hasher_finalize(&_hasher, _buffer.data(), BLAKE3_OUT_LEN);

    return uint8_vector_to_hex_string(_buffer);
}
};
