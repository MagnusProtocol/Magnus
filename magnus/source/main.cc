#include <magnus.hh>

#include <iostream>

int main()
{
    LibMagnus::Magnus new_magnus;
    std::cout << new_magnus.get_hello() << std::endl;
}