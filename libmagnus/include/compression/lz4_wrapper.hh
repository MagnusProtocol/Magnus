#pragma once
#include "compression.hh"

class LZ4 : public Compression {
private:
    std::string* _buffer;
    std::string_view _input;

public:
    LZ4(std::string_view& input);
    ~LZ4();

    std::string_view get_string_view();
    std::string* get_string();

    void compress();
    void decompress();
};
