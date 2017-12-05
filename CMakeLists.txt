cmake_minimum_required(VERSION 2.8.3)
project(pente)

set(CMAKE_CXX_STANDARD 11)
set(CUDA_SEPARABLE_COMPILATION ON)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 REQUIRED COMPONENTS graphics window system)
find_package(CUDA REQUIRED)

set(
        CUDA_NVCC_FLAGS
        ${CUDA_NVCC_FLAGS}
        -O3 -gencode arch=compute_61,code=sm_61
)

set(SRC source/main.cpp
        source/pente.cpp
        source/player.cpp
        source/additional_funcions.cpp
        source/algorithm.cpp
        source/timer.cpp
        source/value.cpp)

set(CUDA_SRC source/cuda/parallel_minmax.cu)

include_directories(${SFML_INCLUDE_DIR}
        kernels
        utility
        cudadevrt)

cuda_add_library(cuda_min_max ${CUDA_SRC})


## Declare a C++ executable
add_executable(pente ${SRC})

target_link_libraries(pente cuda_min_max ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
