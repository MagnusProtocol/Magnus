#pragma once
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class HashingBase {
public:
    HashingBase() {};
    ~HashingBase() {};
    virtual std::string_view get_string_view()
    {
        throw std::runtime_error("[ERROR]: Not yet implemented.");
        return nullptr;
    };

    virtual std::string* get_string()
    {
        throw std::runtime_error("[ERROR]: Not yet implemented.");
        return nullptr;
    };

    virtual std::string hash()
    {
        throw std::runtime_error("[ERROR]: Not yet implemented.");
        return nullptr;
    }
};
