#pragma once
#include <fcntl.h>
#include <filesystem>
#include <spdlog/spdlog.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "spdlog/details/log_msg.h"
#include "spdlog/details/null_mutex.h"

#include <memory>
#include <mutex>
#include <string>

#define SET_LOGGER(CLASS_NAME, LOGGER)                \
    if (spdlog::get(CLASS_NAME) == nullptr) {         \
        LOGGER = spdlog::stderr_color_mt(CLASS_NAME); \
    } else {                                          \
        LOGGER = spdlog::get(CLASS_NAME);             \
    }

namespace Magnus::LibMagnus::Utils {

enum MMAP_MODES {
    READ = PROT_READ,
    WRITE = PROT_WRITE,
    RW = PROT_READ | PROT_WRITE
};

/*
 * @brief: Helper function used to map files using mmap. UNIX ONLY.
 * @param filepath: The path to the file you want to map.
 * @param mode: Mode in which you want to open forementioned file. Only use MMAP_MODES.
 * @returns: std::tuple, first value is the mmap pointer, second value is file length and the third value is the file descriptor.
 */
std::tuple<void*, size_t, int> mmap_file(std::filesystem::path filepath, int mode);

/*
 * @brief: Helper function used to map files using mmap. UNIX ONLY.
 * @param filepath: The path to the file you want to map.
 * @param mode: Mode in which you want to open forementioned file. Only use MMAP_MODES.
 * @param data: a string in which you want to store the data from the mmap'ed file.
 * @returns: std::tuple, first value is the mmap pointer, second value is file length and the third value is the file descriptor.
 */
std::tuple<void*, size_t, int> mmap_file(std::filesystem::path filepath, int mode, std::string& data);

/*
 * @brief: Helper function used to map files using mmap. UNIX ONLY.
 * @param filepath: The path to the file you want to map.
 * @param mode: Mode in which you want to open forementioned file. Only use MMAP_MODES.
 * @param data: a string_view in which you want to store the data from the mmap'ed file.
 * @returns: std::tuple, first value is the mmap pointer, second value is file length and the third value is the file descriptor.
 */
std::tuple<void*, size_t, int> mmap_file(std::filesystem::path filepath, int mode, std::string& data);

/*
 * @brief: Helper function used to unmap files and close their file descriptor after they've been used.
 * it is EXTREMELY important to call this before the end of the program,
 * with the mapp'ed file as the parameter.
 *
 * @param file: a tuple of the pointer to the mapped file, it's size and the file descriptor.
 */
int unmap(std::tuple<void*, size_t, int> mapped_file);

/*
 * @brief: Write to mmap'ed files.
 * @param file: This should be the value returned by the provided mmap_file function.
 * @param data: The data that is to be written to the file, here, std::string&;
 */
void write_mmap(std::tuple<void*, size_t, int>& mapped_file, std::string& data);

/*
 * @brief: Write to mmap'ed files.
 * @param file: This should be the value returned by the provided mmap_file function.
 * @param data: The data that is to be written to the file, here, std::string_view.
 */
void write_mmap(std::tuple<void*, size_t, int>& mapped_file, std::string_view data);

/*
 * @brief: Write to mmap'ed files.
 * @param file: This should be the value returned by the provided mmap_file function.
 * @param data: The data that is to be written to the file, here, a const char*.
 * @param data_size: The length of the data being written to the file.
 */
void write_mmap(std::tuple<void*, size_t, int>& mapped_file, const char* data, size_t data_size);

/*
 * @brief: Read the contents of the file into a string.
 * @param mapped_file: the mapped file.
 * @param dest: the string to read the file into.
 */
void read_string(std::tuple<void*, size_t, int>& mapped_file, std::string& dest);

/*
 * @brief: Read the contents of the file into a string.
 * @param mapped_file: the mapped file.
 * @param dest: the string to read the file into.
 */
void read_string(std::tuple<void*, size_t, int>& mapped_file, std::string_view& dest);

}
