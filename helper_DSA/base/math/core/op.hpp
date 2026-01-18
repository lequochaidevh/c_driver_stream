#pragma once
#include "public/matrix.hpp"

namespace math {

enum class OpType {
    Multi,
    Add,
};

struct Op {
    OpType        type;
    const Tensor* A;
    const Tensor* B;
    Tensor*       Out;
};

}  // namespace math
