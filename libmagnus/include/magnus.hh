#pragma once

#include <fstream>
#include <iostream>

namespace LibMagnus {
class Magnus {
private:
    int _fd;
    void* _addr;
    uintmax_t _length;
    std::string_view _data;

    void map_file();

public:
    Magnus(std::string filepath);
    ~Magnus();
    std::string_view get_data();
};
}
