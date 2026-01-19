#include "hw_impl/cpu/implement.hpp"
#include <cmath>
#include <cassert>

namespace math {

void CPU::execute(const Op& op) {
    switch (op.type) {
        case OpType::Add:
            add(*op.A, *op.B, *op.Out);
            break;

        case OpType::Sub:
            sub(*op.A, *op.B, *op.Out);
            break;

        case OpType::Multi:
            multi(*op.A, *op.B, *op.Out);
            break;

        case OpType::Div:
            div(*op.A, *op.B, *op.Out);
            break;

        case OpType::MatMul:
            matmul(*op.A, *op.B, *op.Out);
            break;

        case OpType::Scale:
            scale(*op.A, *op.Out, op.alpha);
            break;

        case OpType::Exp:
            exp(*op.A, *op.Out);
            break;

        case OpType::Log:
            log(*op.A, *op.Out);
            break;

        case OpType::Sqrt:
            sqrt(*op.A, *op.Out);
            break;

        default:
            throw std::runtime_error("OpType not supported on CPU");
    }
}

/* =========================
   Element-wise ops
   ========================= */

void CPU::add(const Tensor& A, const Tensor& B, Tensor& C) {
    assert(A.numberOfElement() == B.numberOfElement());
    assert(A.numberOfElement() == C.numberOfElement());

    const float* a = A.f32();
    const float* b = B.f32();
    float*       c = C.f32();

    for (size_t i = 0; i < A.numberOfElement(); ++i) c[i] = a[i] + b[i];
}

void CPU::sub(const Tensor& A, const Tensor& B, Tensor& C) {
    assert(A.numberOfElement() == B.numberOfElement());
    assert(A.numberOfElement() == C.numberOfElement());

    const float* a = A.f32();
    const float* b = B.f32();
    float*       c = C.f32();

    for (size_t i = 0; i < A.numberOfElement(); ++i) c[i] = a[i] - b[i];
}

void CPU::multi(const Tensor& A, const Tensor& B, Tensor& C) {
    assert(A.numberOfElement() == B.numberOfElement());
    assert(A.numberOfElement() == C.numberOfElement());

    const float* a = A.f32();
    const float* b = B.f32();
    float*       c = C.f32();

    for (size_t i = 0; i < A.numberOfElement(); ++i) c[i] = a[i] * b[i];
}

void CPU::div(const Tensor& A, const Tensor& B, Tensor& C) {
    assert(A.numberOfElement() == B.numberOfElement());
    assert(A.numberOfElement() == C.numberOfElement());

    const float* a = A.f32();
    const float* b = B.f32();
    float*       c = C.f32();

    for (size_t i = 0; i < A.numberOfElement(); ++i) c[i] = a[i] / b[i];
}

/* =========================
   Matrix multiplication
   ========================= */

void CPU::matmul(const Tensor& A, const Tensor& B, Tensor& C) {
    assert(A.shape.cols == B.shape.rows);
    assert(C.shape.rows == A.shape.rows);
    assert(C.shape.cols == B.shape.cols);

    const float* a = A.f32();
    const float* b = B.f32();
    float*       c = C.f32();

    size_t M = A.shape.rows;
    size_t K = A.shape.cols;
    size_t N = B.shape.cols;

    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            float sum = 0.f;
            for (size_t k = 0; k < K; ++k) {
                sum += a[i * K + k] * b[k * N + j];
            }
            c[i * N + j] = sum;
        }
    }
}

/* =========================
   Scalar ops
   ========================= */

void CPU::scale(const Tensor& A, Tensor& C, float alpha) {
    assert(A.numberOfElement() == C.numberOfElement());

    const float* a = A.f32();
    float*       c = C.f32();

    for (size_t i = 0; i < A.numberOfElement(); ++i) c[i] = alpha * a[i];
}

/* =========================
   Unary math ops
   ========================= */

void CPU::exp(const Tensor& A, Tensor& C) {
    assert(A.numberOfElement() == C.numberOfElement());

    const float* a = A.f32();
    float*       c = C.f32();

    for (size_t i = 0; i < A.numberOfElement(); ++i) c[i] = std::exp(a[i]);
}

void CPU::log(const Tensor& A, Tensor& C) {
    assert(A.numberOfElement() == C.numberOfElement());

    const float* a = A.f32();
    float*       c = C.f32();

    for (size_t i = 0; i < A.numberOfElement(); ++i) c[i] = std::log(a[i]);
}

void CPU::sqrt(const Tensor& A, Tensor& C) {
    assert(A.numberOfElement() == C.numberOfElement());

    const float* a = A.f32();
    float*       c = C.f32();

    for (size_t i = 0; i < A.numberOfElement(); ++i) c[i] = std::sqrt(a[i]);
}

}  // namespace math