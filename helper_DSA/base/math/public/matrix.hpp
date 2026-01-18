#pragma once
#include <memory>
#include "tensor.hpp"

namespace math {

class Matrix {
 public:
    Matrix(size_t rows, size_t cols);

    size_t rows() const;
    size_t cols() const;

    float*       data();  // Phase 1: CPU only
    const float* data() const;

    // INTERNAL bridge (backend/runtime use)
    Tensor&       tensor();
    const Tensor& tensor() const;

 private:
    struct Impl;
    std::shared_ptr<Impl> impl;
};

Matrix multi(const Matrix& A, const Matrix& B);
Matrix add(const Matrix& A, const Matrix& B);

struct Matrix::Impl {
    Tensor tensor;
    Impl(size_t r, size_t c) : tensor({r, c}, DType::F32) {}
};

}  // namespace math