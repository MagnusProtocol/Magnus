cmake_minimum_required(VERSION 3.14)
project("Magnus")

# Set C++ options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# Get all C++ files
file(GLOB MAGNUS_SOURCES
    magnus/source/*.cc
)

file(GLOB LIBMAGNUS_SOURCES
    libmagnus/source/*.cc
)

# Libmagnus (backend/protocol)
add_library(libmagnus ${LIBMAGNUS_SOURCES})
target_include_directories(libmagnus PUBLIC libmagnus/include)

# Magnus (frontend/cli)
add_executable(magnus ${MAGNUS_SOURCES})
target_include_directories(magnus PRIVATE magnus/include)
target_link_libraries(magnus PRIVATE libmagnus)
