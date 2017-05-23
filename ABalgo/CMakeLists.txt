cmake_minimum_required(VERSION 2.8.3)
project(hello)

set (CMAKE_CXX_STANDARD 11)

set(SRC src/algorithm.cpp src/value.cpp src/main.cpp)

## Declare a C++ executable
add_executable(hello ${SRC})

