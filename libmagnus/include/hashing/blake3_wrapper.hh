#pragma once
#include "hashing/hashing.hh"
#include <blake3.h>
#include <vector>

namespace Magnus::LibMagnus::Hashing {
class BLAKE3 : public HashingBase {

private:
    std::string_view _input;
    std::vector<uint8_t> _buffer;
    blake3_hasher _hasher;
    std::string uint8_vector_to_hex_string(const std::vector<uint8_t>& v);

public:
    BLAKE3(std::string_view input);
    ~BLAKE3();

    std::string hash();
};
};
