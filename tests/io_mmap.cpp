#include <fstream>
#include <gtest/gtest.h>
#include <magnus.hpp>

// Demonstrate some basic assertions.
TEST(ReadTest, MMAPClass)
{
    std::string filepath = "../Readme.md";
    // Read a file using mmap.
    auto mapped_read = Magnus::LibMagnus::Utils::MMAP(filepath,
        Magnus::LibMagnus::Utils::MMAP_MODES::READ);
    std::string data_mapped = mapped_read.read();

    // Read a file using fstream.
    std::ofstream ostr(filepath, std::ios::in);
    std::ostringstream sstr;
    sstr << ostr.rdbuf();
    std::string data_ofstream = sstr.str();

    // Expect equality.
    EXPECT_EQ(data_mapped, data_ofstream);
}
