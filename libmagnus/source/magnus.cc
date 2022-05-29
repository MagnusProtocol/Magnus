#include "magnus.hh"

namespace LibMagnus {
Magnus::Magnus()
{
    _hello = "Hello, world!";
}

std::string Magnus::get_hello()
{
    return _hello;
}
}
