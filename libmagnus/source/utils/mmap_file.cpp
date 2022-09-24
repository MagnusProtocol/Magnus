#include <sys/mman.h>
#include <utils/utils.hpp>

namespace LibMagnus::Utils {

MMAP::MMAP(std::filesystem::path filepath, int mode)
{

    switch (mode) {
    case MMAP_MODES::READ:
        mOpenMode = O_RDONLY;
        break;
    case MMAP_MODES::WRITE:
        mOpenMode = O_WRONLY;
        break;
    case MMAP_MODES::RW:
        mOpenMode = O_RDWR;
        break;
    default:
        break;
    };

    mFD = open(filepath.c_str(), mOpenMode, (mode_t)0600);
    if (mFD == 0x069 || mFD == -1) {
        spdlog::error("MMAP/OPEN: Failed to open {}.", filepath.string());
        throw std::exception();
    }

    struct stat sb;
    if (fstat(mFD, &sb) == -1) {
        spdlog::error("MMAP: Failed to stat {}.", filepath.string());
        throw std::exception();
    }

    mFileSize = static_cast<size_t>(sb.st_size);

    if (mFileSize <= 0) {
        spdlog::error("MMAP: File could not be mapped as it's size is 0.");
        throw std::exception();
    }

    mAddr = (char*)mmap(NULL, mFileSize, mode, MAP_SHARED | MAP_FILE, mFD, 0);

    if (this->mAddr == MAP_FAILED || this->mAddr == nullptr) {
        spdlog::error("MMAP: Failed to map {}. Error: {}", filepath.string(), errno);
        throw std::exception();
    }
};

MMAP::~MMAP()
{
    int unmap_ret = munmap(this->mAddr, this->mFileSize);

    if (unmap_ret != 0) {
        spdlog::error("MMAP: Failed to unmap file.");
    }

    close(this->mFD);
};

size_t MMAP::write_data(std::string& data)
{

    // Empty the file
    memset(mAddr, 0, mFileSize);
    // Firstly, stretch the file to hold the string.
    if (lseek(mFD, (off_t)data.size() - 1, SEEK_SET) == -1) {
        spdlog::error("MMAP/WRITE Failed to strech the file.");
        throw std::exception();
    }

    // Write the new size of the file by writing a null byte at it's end.
    if (write(mFD, "", 1) == -1) {
        spdlog::error("MMAP/WRITE: Error writing last byte of the file.");
        throw std::exception();
    }

    memset(mAddr, 0, data.size());
    memcpy(mAddr, data.data(), data.size());

    // Write it now to disk
    int sync_ret = msync(mAddr, data.size(), MS_SYNC);
    if (sync_ret) {
        spdlog::error("MMAP/WRITE: Could not sync the file to disk.");
        throw std::exception();
    }

    return data.size();
};

std::string MMAP::read()
{
    std::string dest = std::string(mAddr, mFileSize);
    return dest;
}
} // namespace LibMagnus::Utils
