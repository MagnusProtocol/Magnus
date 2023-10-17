cmake_minimum_required(VERSION 3.22)
project("Magnus")

# Set C++ options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# GTest
set(GTEST_GIT_TAG v1.13.0)
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG ${GTEST_GIT_TAG}
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
message("-- Fetched GTest version: ${GTEST_GIT_TAG}")

# Get all C++ files
file(GLOB_RECURSE LIBMAGNUS_SOURCES
        libmagnus/*.cpp
        libmagnus/sockets/*.cpp
        libmagnus/compression/*.cpp
        libmagnus/hashing/*.cpp
        libmagnus/encryption/*.cpp
        libmagnus/keygen/*.cpp
)

find_package(OpenSSL REQUIRED)
if (OPENSSL_FOUND)
    message("-- OpenSSL Version: ${OPENSSL_VERSION}")
    if (NOT(OPENSSL_CRYPTO_LIBRARY) OR NOT(OPENSSL_SSL_LIBRARIES))
        message(FATAL_ERROR "-- Could not find OpenSSL::Crypto OR OpenSSL::SSL module(s)")
    endif()
else()
    message(FATAL_ERROR "-- Could not find OpenSSL")
endif()

# Don't build the zstd static libs & cli
set(ZSTD_BUILD_STATIC OFF)
set(ZSTD_BUILD_PROGRAMS OFF)
add_subdirectory(deps/zstd/build/cmake)

# LZ4 config
set(LZ4_BUNDLED_MODE OFF)
set(BUILD_STATIC_LIBS ON)
set(LZ4_BUILD_CLI OFF)

# add lz4
add_subdirectory(deps/lz4/build/cmake)


# Libmagnus (backend/protocol)
add_library(magnus ${LIBMAGNUS_SOURCES})

# Add all the includes
target_include_directories(magnus PUBLIC
        libmagnus/
        deps/zstd/lib
)

# Link all the needed libraries
target_link_libraries(magnus PUBLIC
        libzstd_shared
        lz4_static
        OpenSSL::Crypto
        OpenSSL::SSL
)

# Enable some compiler/linker options.
target_compile_options(magnus PRIVATE
        -Wall -fsanitize=address -Wextra
)
target_link_options(magnus PRIVATE -fsanitize=address)

# Get all C++ files
file(GLOB_RECURSE TESTS_SOURCES
        tests/*.cpp
)
# Tests
add_executable(Tests ${TESTS_SOURCES})

target_link_libraries(Tests
        GTest::gtest_main
        magnus
)

include(GoogleTest)
gtest_discover_tests(Tests)


# Client
add_executable(client magnus/client.cpp)
target_link_libraries(client PRIVATE magnus)

# Server
add_executable(server magnus/server.cpp)
target_link_libraries(server PRIVATE magnus)