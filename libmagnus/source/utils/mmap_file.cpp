#include <algorithm>
#include <errno.h>
#include <iostream>
#include <utils.hpp>

namespace Magnus::LibMagnus::Utils {

/*
 * @brief: Helper function used to map files using mmap. UNIX ONLY.
 * @param filepath: The path to the file you want to map.
 * @param mode: Mode in which you want to open forementioned file. Only use MMAP_MODES.
 * @returns: std::tuple, first value is the mmap pointer, second value is file length.
 */
std::tuple<void*, size_t, int> mmap_file(std::filesystem::path filepath,
    int mode)
{
    int fd = 0x069,
        open_mode = 0x0;

    switch (mode) {
    case MMAP_MODES::READ:
        open_mode = O_RDONLY;
        break;
    case MMAP_MODES::WRITE:
        open_mode = O_WRONLY;
        break;
    case MMAP_MODES::RW:
        open_mode = O_RDWR;
        break;
    default:
        break;
    };

    fd = open(filepath.c_str(), open_mode, (mode_t)0600);
    if (fd == 0x069 || fd == -1) {
        spdlog::error("MMAP/OPEN: Failed to open {}.", filepath.string());
        throw std::exception();
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        spdlog::error("MMAP: Failed to stat {}.", filepath.string());
        throw std::exception();
    }

    size_t file_size = sb.st_size;

    void* addr = mmap(NULL, file_size, mode, MAP_SHARED | MAP_FILE, fd, 0);
    if (addr == MAP_FAILED || addr == nullptr) {
        spdlog::error("MMAP: Failed to map {}. Error: {}", filepath.string(), errno);
        throw std::exception();
    }

    return std::make_tuple(addr, file_size, fd);
};

/*
 * @brief: Helper function used to map files using mmap. UNIX ONLY.
 * @param filepath: The path to the file you want to map.
 * @param mode: Mode in which you want to open forementioned file. Only use MMAP_MODES.
 * @param data: a string in which you want to store the data from the mmap'ed file.
 * @returns: std::tuple, first value is the mmap pointer, second value is file length.
 */
std::tuple<void*, size_t, int> mmap_file(std::filesystem::path filepath,
    int mode, std::string& data)
{
    auto file = mmap_file(filepath, mode);

    data = std::string(
        static_cast<const char*>(std::get<0>(file)),
        std::get<1>(file));

    return file;
}

std::tuple<void*, size_t, int> mmap_file(std::filesystem::path filepath,
    int mode, std::string_view data)
{
    auto file = mmap_file(filepath, mode);

    data = std::string_view(
        static_cast<const char*>(std::get<0>(file)),
        std::get<1>(file));

    return file;
}

/*
 * @brief: Helper function used to unmap files and close their file descriptor after they've been used.
 * it is EXTREMELY important to call this before the end of the program,
 * with the mapp'ed file as the parameter.
 *
 * @param file: a tuple of the pointer to the mapped file, it's size and the file descriptor.
 */
int unmap(std::tuple<void*, size_t, int> mapped_file)
{
    int unmap_ret = munmap(
        std::get<0>(mapped_file),
        std::get<1>(mapped_file));

    if (unmap_ret != 0) {
        spdlog::error("MMAP: Failed to unmap file.");
        throw std::exception();
    }

    close(std::get<2>(mapped_file));

    return unmap_ret;
}

/*
 * @brief: Read the contents of the file into a string.
 * @param mapped_file: the mapped file.
 * @param dest: the string to read the file into.
 */
void read_string(std::tuple<void*, size_t, int> mapped_file,
    std::string& dest)
{
    dest = std::string(
        static_cast<char*>(std::get<0>(mapped_file)),
        std::get<1>(mapped_file));
}

/*
 * @brief: Read the contents of the file into a string.
 * @param mapped_file: the mapped file.
 * @param dest: the string to read the file into.
 */
void read_string(std::tuple<void*, size_t, int> mapped_file,
    std::string_view& dest)
{
    dest = std::string_view(
        static_cast<char*>(std::get<0>(mapped_file)),
        std::get<1>(mapped_file));
}

/*
 * @brief: Write to mmap'ed files.
 * @param file: This should be the value returned by the provided mmap_file function.
 * @param data: The data that is to be written to the file, here, a const char*.
 * @param data_size: The length of the data being written to the file.
 */
// TODO: FIX MMAP WRITE
void write_data(std::tuple<void*, size_t, int> mapped_file,
    const char* data, size_t data_size)
{
    // Empty the file
    memset(std::get<0>(mapped_file), 0, std::get<1>(mapped_file));
    // Firstly, stretch the file to hold the string.
    if (lseek(std::get<2>(mapped_file),
            data_size - 1, SEEK_SET)

        == -1) {
        spdlog::error("MMAP/WRITE Failed to strech the file.");
        throw std::exception();
    }

    // Write the new size of the file by writing a null byte at it's end.
    if (write(std::get<2>(mapped_file), "", 1) == -1) {
        spdlog::error("MMAP/WRITE: Error writing last byte of the file.");
        throw std::exception();
    }

    memset(std::get<0>(mapped_file), 0, std::get<1>(mapped_file));
    memcpy(std::get<0>(mapped_file), data, data_size);

    // Write it now to disk
    if (msync(std::get<0>(mapped_file), data_size, MS_SYNC) == -1) {
        spdlog::error("MMAP/WRITE: Could not sync the file to disk.");
        throw std::exception();
    }
}

/*
 * @brief: Write to mmap'ed files.
 * @param file: This should be the value returned by the provided mmap_file function.
 * @param data: The data that is to be written to the file, here, std::string&;
 */
void write_mmap(std::tuple<void*, size_t, int>& mapped_file,
    std::string& data)
{
    write_data(mapped_file, data.data(), data.size());
}

/*
 * @brief: Write to mmap'ed files.
 * @param file: This should be the value returned by the provided mmap_file function.
 * @param data: The data that is to be written to the file, here, std::string_view.
 */
void write_mmap(std::tuple<void*, size_t, int>& mapped_file,
    std::string_view data)
{
    write_data(mapped_file, data.data(), data.size());
}
};
