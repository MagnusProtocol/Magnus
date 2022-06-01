#pragma once

#include <fstream>
#include <iostream>

namespace LibMagnus {
class Magnus {
private:
    int _fd = 0;
    void* _addr = nullptr;
    uintmax_t _length = 0;
    std::string_view _data = "";

    void map_file();

public:
    Magnus(std::string filepath);
    ~Magnus();
    std::string_view get_data();
};
}
