#include "public/matrix.hpp"
#include "hw_impl/cpu/implement.hpp"

namespace math {

static CPU cpu;

Matrix add(const Matrix& A, const Matrix& B) {
    Matrix C(A.rows(), A.cols());

    Op op{OpType::Add, &A.tensor(), &B.tensor(), &C.tensor()};

    cpu.execute(op);
    return C;
}

Matrix sub(const Matrix& A, const Matrix& B) {
    Matrix C(A.rows(), A.cols());

    Op op{OpType::Sub, &A.tensor(), &B.tensor(), &C.tensor()};

    cpu.execute(op);
    return C;
}

Matrix multi(const Matrix& A, const Matrix& B) {
    Matrix C(A.rows(), A.cols());

    Op op{OpType::Multi, &A.tensor(), &B.tensor(), &C.tensor()};

    cpu.execute(op);
    return C;
}

Matrix div(const Matrix& A, const Matrix& B) {
    Matrix C(A.rows(), A.cols());

    Op op{OpType::Div, &A.tensor(), &B.tensor(), &C.tensor()};

    cpu.execute(op);
    return C;
}

Matrix matmul(const Matrix& A, const Matrix& B) {
    Matrix C(A.rows(), B.cols());

    Op op{OpType::MatMul, &A.tensor(), &B.tensor(), &C.tensor()};

    cpu.execute(op);
    return C;
}

Matrix scale(const Matrix& A, float alpha) {
    Matrix C(A.rows(), A.cols());

    Op op{
        OpType::Scale, &A.tensor(),
        nullptr,  // not use B
        &C.tensor(),
        alpha  // scalar
    };

    cpu.execute(op);
    return C;
}

Matrix exp(const Matrix& A) {
    Matrix C(A.rows(), A.cols());

    Op op{OpType::Exp, &A.tensor(), nullptr, &C.tensor()};

    cpu.execute(op);
    return C;
}

Matrix log(const Matrix& A) {
    Matrix C(A.rows(), A.cols());

    Op op{OpType::Log, &A.tensor(), nullptr, &C.tensor()};

    cpu.execute(op);
    return C;
}

Matrix sqrt(const Matrix& A) {
    Matrix C(A.rows(), A.cols());

    Op op{OpType::Sqrt, &A.tensor(), nullptr, &C.tensor()};

    cpu.execute(op);
    return C;
}

Tensor&       Matrix::tensor() { return impl->tensor; }
const Tensor& Matrix::tensor() const { return impl->tensor; }

Matrix::Matrix(size_t r, size_t c) : impl(std::make_shared<Impl>(r, c)) {}

size_t Matrix::rows() const { return impl->tensor.shape.rows; }
size_t Matrix::cols() const { return impl->tensor.shape.cols; }

float*       Matrix::data() { return impl->tensor.f32(); }
const float* Matrix::data() const { return impl->tensor.f32(); }

}  // namespace math
