#pragma once
#include <iostream>

// Base class for all compression backends
class Compression {

public:
    virtual std::string_view get_string_view()
    {
        std::cerr << "[ERROR]: Not yet implemented." << std::endl;
        return nullptr;
    };
    virtual std::string* get_string()
    {
        std::cerr << "[ERROR]: Not yet implemented." << std::endl;
        return nullptr;
    };

    virtual void compress() { std::cerr << "[ERROR]: Not yet implemented." << std::endl; }
    virtual void decompress() { std::cerr << "[ERROR]: Not yet implemented." << std::endl; }
};