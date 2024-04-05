cmake_minimum_required(VERSION 3.14)
project(HuffmanCoding)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(queue_lib INTERFACE)
target_include_directories(queue_lib INTERFACE queue/)
add_executable(HuffmanCoding HuffmanCoding.cpp)
target_link_libraries(HuffmanCoding PRIVATE queue_lib)


