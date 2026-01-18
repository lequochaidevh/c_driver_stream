#include "hw_impl/cpu/implement.hpp"
#include <cassert>

namespace math {

void CPU::execute(const Op& op) {
    if (op.type == OpType::Add) {
        size_t n = op.Out->shape.rows * op.Out->shape.cols;

        const float* a   = op.A->f32();
        const float* b   = op.B->f32();
        float*       out = op.Out->f32();

        for (size_t i = 0; i < n; i++) out[i] = a[i] + b[i];
    }

    if (op.type == OpType::Multi) {
        auto M = op.A->shape.rows;
        auto K = op.A->shape.cols;
        auto N = op.B->shape.cols;

        const float* A = op.A->f32();
        const float* B = op.B->f32();
        float*       C = op.Out->f32();

        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) {
                float sum = 0.f;
                for (size_t k = 0; k < K; k++) sum += A[i * K + k] * B[k * N + j];
                C[i * N + j] = sum;
            }
        }
    }
}

}  // namespace math