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

Matrix add(const Matrix& A, const Matrix& B);
Matrix sub(const Matrix& A, const Matrix& B);
Matrix multi(const Matrix& A, const Matrix& B);
Matrix div(const Matrix& A, const Matrix& B);

Matrix matmul(const Matrix& A, const Matrix& B);

Matrix scale(const Matrix& A, float alpha);

Matrix exp(const Matrix& A);
Matrix log(const Matrix& A);
Matrix sqrt(const Matrix& A);

struct Matrix::Impl {
    Tensor tensor;
    Impl(size_t r, size_t c) : tensor({r, c}, DType::F32) {}
};

struct MatrixView {
    float* data;
    int    rows, cols;
    int    stride_r;
    int    stride_c;
};

}  // namespace math