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

class MMAP {
private:
    int mOpenMode;
    char* mAddr;
    int mFD;
    size_t mFileSize;

public:
    MMAP(std::filesystem::path filepath, int mode);
    ~MMAP();

    /*
     * @brief: Read the contents of the file into a string.
     */
    std::string read();

    /*
     * @brief: Write to mmap'ed files.
     * @param data: The data that is to be written to the file, here, std::string_view.
     */
    size_t write_data(std::string& data);
};

} // namespace Magnus::LibMagnus::Utils
