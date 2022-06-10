#pragma once
#include <iostream>

// Base class for all compression backends
class CompressionBase {

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

    virtual void compress_string() { throw std::runtime_error("[ERROR]: Not yet implemented."); }
    virtual void decompress_string() { throw std::runtime_error("[ERROR]: Not yet implemented."); }

    // TODO: Implement compression for dictionaries so that parts can be compressed.
    virtual void compress_file() { throw std::runtime_error("[ERROR]: Not yet implemented."); }
    virtual void decompress_file() { throw std::runtime_error("[ERROR]: Not yet implemented."); }
};
