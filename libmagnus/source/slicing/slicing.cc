#include "slicing/slicing.hh"
#include <cmath>
#include <iostream>

namespace Magnus::LibMagnus::Slicing {

std::vector<std::string_view> slice_into_parts(std::string_view data)
{
    /*
    |-------------------|
    |    |    |    |    |
    |    |    |    |    |
    |    |    |    |    |
    |    |    |    |    |
    |___________________|

    first, divide into parts by:
        let f(x) = x % (ceil(sqrt(x)) + 2)
        (size_t)part_size = f(file_size)
        (float)number_of_parts = file_size / part_size

        Now, extract the decimal part from number_of_parts alongside the one's place;
        for example:
            123324.42 -> 123320 + 4.42
                 ____
    */
    double x = ceil(sqrt(data.size())) + 2;
    double part_size = remainder(x, data.size());
    double number_of_parts = data.size() / part_size;
    double last_packet_padding = 1 - modf(number_of_parts, &last_packet_padding);
    // HELL YEAH IT WORKS!
    // Now, add the padding to the end of the last packet.

    std::cout << "Part size: " << part_size << std::endl
              << "Number of parts: " << number_of_parts << std::endl
              << "Last packet's padding: " << last_packet_padding << std::endl
              << "Full file size: " << data.size() << std::endl
              << "Full file size reconstruction: " << (size_t)(part_size * number_of_parts) << std::endl;

    for (size_t i = 0; i < number_of_parts; i++)
    {

    }
    std::vector<std::string_view> sliced_file;
    return sliced_file;
}
}
