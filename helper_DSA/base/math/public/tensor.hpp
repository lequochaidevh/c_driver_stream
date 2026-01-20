#pragma once
#include <vector>
#include <memory>
#include "dtype.hpp"
#include <iostream>

namespace math {

struct Shape {
    size_t    rows;
    size_t    cols;
    ptrdiff_t stride_r;  // ++ when +1 rows
    ptrdiff_t stride_c;  // ++ when +1 cols
};

struct Storage {
    std::shared_ptr<uint8_t[]> data;
    size_t                     bytes;
};

class Tensor {
 public:
    Shape   shape;
    DType   dtype;
    Storage storage;
    size_t  offset = 0;
    size_t  numberOfElement() const { return shape.rows * shape.cols; }

    Tensor(size_t r, size_t c, DType t) : dtype(t) {
        shape.rows     = r;
        shape.cols     = c;
        shape.stride_c = 1;
        shape.stride_r = c;

        size_t n      = r * c;
        storage.bytes = n * dtype_size(t);
        storage.data.reset(new uint8_t[storage.bytes]);
    }

    float* f32() { return reinterpret_cast<float*>(storage.data.get() + offset); }

    // const access
    const float* f32() const { return reinterpret_cast<const float*>(storage.data.get()) + offset; }

    // non-const access
    float& at(size_t r, size_t c) { return f32()[r * shape.stride_r + c * shape.stride_c]; }

    const inline float& at(size_t r, size_t c) const { return f32()[r * shape.stride_r + c * shape.stride_c]; }

    void transpose() {
        std::swap(shape.rows, shape.cols);
        std::swap(shape.stride_r, shape.stride_c);
    }

    void slice(size_t r0, size_t c0, size_t r, size_t c) {
        offset += r0 * shape.stride_r + c0 * shape.stride_c;
        shape.rows = r;
        shape.cols = c;
    }

    // GPU / SIMD → check to packed again
    bool is_contiguous() const { return shape.stride_c == 1 && shape.stride_r == shape.cols; }
};

}  // namespace math