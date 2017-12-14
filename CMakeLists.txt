cmake_minimum_required(VERSION 2.8.3)
project(pente)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
set(CMAKE_BUILD_TYPE DEBUG)

find_package(SFML 2 REQUIRED COMPONENTS graphics window system)
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

set(SRC source/main.cpp
        source/pente.cpp
        source/player.cpp
        source/algorithm.cpp
        source/timer.cpp
        source/value.cpp
        )


## Declare a C++ executable
add_executable(pente ${SRC})
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(pente ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
