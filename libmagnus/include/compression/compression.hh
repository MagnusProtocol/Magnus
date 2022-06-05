#pragma once
#include <iostream>

// Base class for all compression backends
class Compression {

public:
    virtual std::string_view get_string_view()
    {
        throw std::runtime_error("[ERROR]: Not yet implemented.");
        return nullptr;
    };
    virtual std::string& get_string()
    {
        throw std::runtime_error("[ERROR]: Not yet implemented.");
    };

    virtual void compress() { throw std::runtime_error("[ERROR]: Not yet implemented."); }
    virtual void decompress() { throw std::runtime_error("[ERROR]: Not yet implemented."); }

    // TODO: Implement compression for dictionaries so that parts can be compressed.
    virtual void compress_dict() { throw std::runtime_error("[ERROR]: Not yet implemented."); }
    virtual void decompress_dict() { throw std::runtime_error("[ERROR]: Not yet implemented."); }
};