cmake_minimum_required(VERSION 3.22)

# Client
add_executable(client client.cpp)
target_link_libraries(client PRIVATE magnus)

# Server
add_executable(server server.cpp)
target_link_libraries(server PRIVATE magnus)
