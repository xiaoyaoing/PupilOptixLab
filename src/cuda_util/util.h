#pragma once

#include "vec_math.h"
#include <cuda.h>
#include <cuda_runtime.h>
#include <sstream>
#include <assert.h>
#include <iostream>

inline void CudaCheck(cudaError_t error, const char *call, const char *file, unsigned int line) {
    if (error != cudaSuccess) {
        std::wstringstream ss;
        ss << "CUDA call (" << call << " ) failed with error: '"
           << cudaGetErrorString(error) << "' (" << file << ":" << line << ")\n";
        std::wcerr << ss.str().c_str();
        assert(false);
    }
}
inline void CudaCheck(CUresult error, const char *call, const char *file, unsigned int line) {
    if (error != cudaSuccess) {
        std::wstringstream ss;
        ss << "CUDA call (" << call << " ) failed with error: '"
           << error << "' (" << file << ":" << line << ")\n";

        std::wcerr << ss.str().c_str();
        assert(false);
    }
}

#define CUDA_CHECK(call) CudaCheck(call, #call, __FILE__, __LINE__)
#define CUDA_FREE(var)                                           \
    do {                                                         \
        if (var)                                                 \
            CUDA_CHECK(cudaFree(reinterpret_cast<void *>(var))); \
    } while (false)

namespace util {
inline CUdeviceptr CudaMemcpy(void *src, size_t size) {
    CUdeviceptr device_memory = 0;
    CUDA_CHECK(cudaMalloc(reinterpret_cast<void **>(&device_memory), size));
    CUDA_CHECK(cudaMemcpy(reinterpret_cast<void **>(device_memory), src, size, cudaMemcpyHostToDevice));
    return device_memory;
}
}// namespace util