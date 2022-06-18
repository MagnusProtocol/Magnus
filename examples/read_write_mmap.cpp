#include "utils.hpp"
#include <fstream>
#include <magnus.hpp>

// Magnus utility API: MMAP files.
using namespace Magnus;
int main()
{
    std::filesystem::path filepath_read = "../assets/image.jpg";
    std::string data;
    // Read the contents of image.jpg to data.
    auto mapped_file_read = Magnus::LibMagnus::Utils::mmap_file(filepath_read, Magnus::LibMagnus::Utils::MMAP_MODES::READ, data);

    // Write the data that was read back to image_mmap.jpg
    std::filesystem::path filepath_write = "./image_mmap.jpg";
    auto mapped_file_write = Magnus::LibMagnus::Utils::mmap_file(filepath_write, Magnus::LibMagnus::Utils::MMAP_MODES::RW);
    Magnus::LibMagnus::Utils::write_mmap(mapped_file_write, data);

    Magnus::LibMagnus::Utils::unmap(mapped_file_read);
    Magnus::LibMagnus::Utils::unmap(mapped_file_write);
}
