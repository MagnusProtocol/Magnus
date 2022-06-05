cmake_minimum_required(VERSION 3.14)
project("Magnus")

# Set C++ options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MACOSX_RPATH 1)

# Get all C++ files
file(GLOB MAGNUS_SOURCES
    magnus/source/*.cc
)

file(GLOB LIBMAGNUS_SOURCES
    libmagnus/source/*.cc
    libmagnus/source/compression/*.cc
    libmagnus/source/hashing/*.cc
    libmagnus/source/slicing/*.cc
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

# Libmagnus (backend/protocol)
add_library(libmagnus ${LIBMAGNUS_SOURCES})
target_include_directories(libmagnus PUBLIC 
    libmagnus/include
    deps/zstd/lib
)
target_link_libraries(libmagnus PUBLIC blake3-shared libzstd_shared lz4_shared)


# Magnus (frontend/cli)
add_executable(magnus ${MAGNUS_SOURCES})
target_include_directories(magnus PRIVATE magnus/include)
target_link_libraries(magnus PRIVATE libmagnus)
