#pragma once
#include <vector>
#include <memory>
#include "dtype.hpp"

namespace math {

struct Shape {
    size_t rows;
    size_t cols;
};

class Tensor {
 public:
    Shape                      shape;
    DType                      dtype;
    std::unique_ptr<uint8_t[]> data;

    Tensor(Shape s, DType t) : shape(s), dtype(t), data(new uint8_t[s.rows * s.cols * dtype_size(t)]) {}

    float* f32() { return reinterpret_cast<float*>(data.get()); }

    // const access
    const float* f32() const { return reinterpret_cast<const float*>(data.get()); }
};

}  // namespace math