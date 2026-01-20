#include "hw_impl/cpu/implement.hpp"
#include <cmath>
#include <cassert>
#include <stdexcept>

#define IS_CONTIGUOUS_3OP A.is_contiguous() && B.is_contiguous() && C.is_contiguous()
#define FAST_PATH_ELEMWISE(OP)                                    \
    do {                                                          \
        if (IS_CONTIGUOUS_3OP) {                                  \
            size_t n = A.shape.rows * A.shape.cols;               \
            for (size_t i = 0; i < n; ++i) c[i] = OP(a[i], b[i]); \
            return;                                               \
        }                                                         \
    } while (0)

#define STRIDE_ACCESS(base, r, c, sr, sc) base[(r) * (sr) + (c) * (sc)]

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
    assert(A.shape.rows == B.shape.rows);
    assert(A.shape.cols == B.shape.cols);
    assert(A.shape.rows == C.shape.rows);
    assert(A.shape.cols == C.shape.cols);

    const float* a = A.f32();
    const float* b = B.f32();
    float*       c = C.f32();

    FAST_PATH_ELEMWISE([](float x, float y) { return x + y; });

    // fallback strided
    auto ar = A.shape.stride_r;
    auto ac = A.shape.stride_c;
    auto br = B.shape.stride_r;
    auto bc = B.shape.stride_c;
    auto cr = C.shape.stride_r;
    auto cc = C.shape.stride_c;

    for (size_t i = 0; i < A.shape.rows; ++i) {
        for (size_t j = 0; j < A.shape.cols; ++j) {
            STRIDE_ACCESS(c, i, j, cr, cc) = STRIDE_ACCESS(a, i, j, ar, ac) + STRIDE_ACCESS(b, i, j, br, bc);
        }
    }
}

void CPU::sub(const Tensor& A, const Tensor& B, Tensor& C) {
    assert(A.shape.rows == B.shape.rows);
    assert(A.shape.cols == B.shape.cols);
    assert(A.shape.rows == C.shape.rows);
    assert(A.shape.cols == C.shape.cols);

    const float* a = A.f32();
    const float* b = B.f32();
    float*       c = C.f32();

    FAST_PATH_ELEMWISE([](float x, float y) { return x + y; });

    // fallback strided

    auto ar = A.shape.stride_r;
    auto ac = A.shape.stride_c;
    auto br = B.shape.stride_r;
    auto bc = B.shape.stride_c;
    auto cr = C.shape.stride_r;
    auto cc = C.shape.stride_c;

    for (size_t i = 0; i < A.shape.rows; ++i) {
        for (size_t j = 0; j < A.shape.cols; ++j) {
            STRIDE_ACCESS(c, i, j, cr, cc) = STRIDE_ACCESS(a, i, j, ar, ac) - STRIDE_ACCESS(b, i, j, br, bc);
        }
    }
}

void CPU::multi(const Tensor& A, const Tensor& B, Tensor& C) {
    assert(A.shape.rows == B.shape.rows);
    assert(A.shape.cols == B.shape.cols);
    assert(A.shape.rows == C.shape.rows);
    assert(A.shape.cols == C.shape.cols);

    const float* a = A.f32();
    const float* b = B.f32();
    float*       c = C.f32();

    FAST_PATH_ELEMWISE([](float x, float y) { return x + y; });

    // fallback strided
    auto ar = A.shape.stride_r;
    auto ac = A.shape.stride_c;
    auto br = B.shape.stride_r;
    auto bc = B.shape.stride_c;
    auto cr = C.shape.stride_r;
    auto cc = C.shape.stride_c;

    for (size_t i = 0; i < A.shape.rows; ++i) {
        for (size_t j = 0; j < A.shape.cols; ++j) {
            STRIDE_ACCESS(c, i, j, cr, cc) = STRIDE_ACCESS(a, i, j, ar, ac) * STRIDE_ACCESS(b, i, j, br, bc);
        }
    }
}

void CPU::div(const Tensor& A, const Tensor& B, Tensor& C) {
    assert(A.shape.rows == B.shape.rows);
    assert(A.shape.cols == B.shape.cols);
    assert(A.shape.rows == C.shape.rows);
    assert(A.shape.cols == C.shape.cols);

    const float* a = A.f32();
    const float* b = B.f32();
    float*       c = C.f32();

    FAST_PATH_ELEMWISE([](float x, float y) { return x + y; });

    // fallback strided
    auto ar = A.shape.stride_r;
    auto ac = A.shape.stride_c;
    auto br = B.shape.stride_r;
    auto bc = B.shape.stride_c;
    auto cr = C.shape.stride_r;
    auto cc = C.shape.stride_c;

    for (size_t i = 0; i < A.shape.rows; ++i) {
        for (size_t j = 0; j < A.shape.cols; ++j) {
            STRIDE_ACCESS(c, i, j, cr, cc) = STRIDE_ACCESS(a, i, j, ar, ac) / STRIDE_ACCESS(b, i, j, br, bc);
        }
    }
}

/* =========================
   Matrix multiplication
   ========================= */

void matmul_native(const Tensor& A, const Tensor& B, Tensor& C) {
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

void CPU::matmul(const Tensor& A, const Tensor& B, Tensor& C) {
    if (A.shape.cols != B.shape.rows) throw std::runtime_error("matmul: shape mismatch");

    if (A.dtype != DType::F32 || B.dtype != DType::F32 || C.dtype != DType::F32)
        throw std::runtime_error("matmul: only F32 supported");

    if (C.shape.rows != A.shape.rows || C.shape.cols != B.shape.cols)
        throw std::runtime_error("matmul: output shape mismatch");

    const float* a = A.f32();
    const float* b = B.f32();
    float*       c = C.f32();

    if (IS_CONTIGUOUS_3OP) {
        matmul_native(A, B, C);
        return;
    }

    const ptrdiff_t ar = A.shape.stride_r;
    const ptrdiff_t ac = A.shape.stride_c;

    const ptrdiff_t br = B.shape.stride_r;
    const ptrdiff_t bc = B.shape.stride_c;

    const ptrdiff_t cr = C.shape.stride_r;
    const ptrdiff_t cc = C.shape.stride_c;
    size_t          M  = A.shape.rows;
    size_t          K  = A.shape.cols;
    size_t          N  = B.shape.cols;

    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            float sum = 0.0f;

            for (size_t k = 0; k < K; ++k) {
                sum += a[i * ar + k * ac] * b[k * br + j * bc];
            }

            c[i * cr + j * cc] = sum;
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