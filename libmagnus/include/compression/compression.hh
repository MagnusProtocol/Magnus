#pragma once
#include <iostream>

// Base class for all compression backends
class Compression {

public:
    virtual std::string_view get_string_view()
    {
        throw "[ERROR]: Not yet implemented.";
        return nullptr;
    };
    virtual std::string* get_string()
    {
        throw "[ERROR]: Not yet implemented.";
        return nullptr;
    };

    virtual void compress() { throw "[ERROR]: Not yet implemented."; }
    virtual void decompress() { throw "[ERROR]: Not yet implemented."; }
};