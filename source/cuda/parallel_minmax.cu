#include "../../include/cuda/parallel_minmax.h"

#include <stdio.h>

#include <cuda.h>
#include <cuda_runtime_api.h>

const int NUMBER_OF_BLOCKS = 1;
const int THREADS_PER_BLOCK = 1;

__global__ void cuda_recursive(int depth)
{
    int id = blockIdx.x * blockDim.x + threadIdx.x;
    if(id == 0)
    {
        printf("Print with depth: %i\n", depth);
    }
    if(depth == 0)
    {
        return;
    }

    cuda_recursive<<<NUMBER_OF_BLOCKS, THREADS_PER_BLOCK>>>(depth - 1);
    cudaDeviceSynchronize();
    __syncthreads();
}

__global__ void test()
{
    int id = blockIdx.x * blockDim.x + threadIdx.x;
    if(id == 0)
    {
        printf("Inside kernel has!");
    }
}

void recursive(int depth)
{
    cuda_recursive<<<NUMBER_OF_BLOCKS, THREADS_PER_BLOCK>>>(depth);
    cudaDeviceSynchronize();
}