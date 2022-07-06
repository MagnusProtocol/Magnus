#pragma once

#include <string_view>
#include <vector>

namespace Magnus::LibMagnus::Slicing {

/*
 * @brief: The function takes in a string view and slices it into parts
 *         based on their size.
 * @param: takes in std::string_view
 * @returns: A vector of string_views, sliced into ample parts.
 * */
std::vector<std::string_view> slice_into_parts(std::string_view data);

/*
 * @brief: This function pads a string with the given number of bytes.
 * */
std::string_view pad_string(std::string_view data, double padding);
}
