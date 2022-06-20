cmake_minimum_required(VERSION 3.14)
project("Magnus")

# Set C++ options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Get all C++ files
file(GLOB MAGNUS_SOURCES
    magnus/source/*.cpp
)

file(GLOB LIBMAGNUS_SOURCES
    libmagnus/source/*.cpp
    libmagnus/source/compression/*.cpp
    libmagnus/source/hashing/*.cpp
    libmagnus/source/slicing/*.cpp
    libmagnus/source/utils/*.cpp
    libmagnus/source/encryption/*.cpp
)
# Don't build the zstd static libs
set(ZSTD_BUILD_STATIC OFF)
# Don't build the zstd cli
set(ZSTD_BUILD_PROGRAMS OFF)
# add zstd
add_subdirectory(deps/zstd/build/cmake)

set(LZ4_BUNDLED_MODE OFF)
set(BUILD_STATIC_LIBS OFF)
set(LZ4_BUILD_CLI OFF)
# add lz4
add_subdirectory(deps/lz4/build/cmake)

# add blake3
add_subdirectory(deps/BLAKE3/c)

# add spdlog
add_subdirectory(deps/spdlog)

# add cryptopp
set(BUILD_TESTING OFF)
set(BUILD_STATIC OFF)
add_subdirectory(deps/cryptopp-cmake)
# Libmagnus (backend/protocol)

# Add all the sources
add_library(libmagnus ${LIBMAGNUS_SOURCES})
# Add all the includes
target_include_directories(libmagnus PUBLIC 
    libmagnus/include
    deps/zstd/lib
)
# Link all the needed libraries
target_link_libraries(libmagnus PUBLIC
    blake3-shared
    libzstd_shared
    lz4_shared
    cryptopp-shared
    spdlog
)

# Precompile reused headers
target_precompile_headers(libmagnus
  PUBLIC
    libmagnus/include/magnus.hpp
    deps/spdlog/include/spdlog/spdlog.h
  PRIVATE
    <iostream>
    <filesystem>
    <vector>
    <string>
    <string_view>
    <unordered_map>
)

# Enable some compiler/linker options.
target_compile_options(libmagnus PRIVATE -fsanitize=address
    -Werror -Wall -Wextra
    -flto -fstack-protector-all
    -fasynchronous-unwind-tables -DADDRESS_SANITIZER
)
target_link_options(libmagnus PRIVATE -fsanitize=address)

# Magnus (frontend/cli)
add_executable(magnus ${MAGNUS_SOURCES})
target_include_directories(magnus PRIVATE magnus/include)
target_link_libraries(magnus PRIVATE libmagnus)
