#include "magnus.hpp"
#include <filesystem>
#include <string.h>

// For mmap
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace LibMagnus
{
Magnus::Magnus()
{
}

Magnus::~Magnus()
{
}
} // namespace LibMagnus
