#pragma once

#include <string_view>
#include <vector>

namespace Slicing {

std::vector<std::string_view> slice_into_parts(std::string_view data);
}