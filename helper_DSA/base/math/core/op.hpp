#pragma once
#include "public/matrix.hpp"

namespace math {

enum class OpType {
    // Element-wise
    Add,    // C = A + B
    Sub,    // C = A - B
    Multi,  // C = A ⊙ B
    Div,    // C = A / B

    // Matrix algebra
    MatMul,       // C = A × B
    Transpose,    // C = A^T
    Inverse,      // C = A^-1
    Determinant,  // det(A)

    // Reduction
    Sum,   // sum(A)
    Mean,  // mean(A)
    Max,
    Min,

    // Scalar ops
    Scale,  // C = αA

    // Activation / math
    Exp,
    Log,
    Sqrt,
};

struct Op {
    OpType        type;
    const Tensor* A;
    const Tensor* B;
    Tensor*       Out;
    float         alpha = 1.f;  // dùng cho Scale
};

}  // namespace math
