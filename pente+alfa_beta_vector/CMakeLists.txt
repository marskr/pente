cmake_minimum_required(VERSION 2.8.3)
project(pente)

set (CMAKE_CXX_STANDARD 11)

set(SRC src/main.cpp src/pente.cpp src/player.cpp src/algorithm.cpp src/value.cpp)

## Declare a C++ executable
add_executable(pente ${SRC})
