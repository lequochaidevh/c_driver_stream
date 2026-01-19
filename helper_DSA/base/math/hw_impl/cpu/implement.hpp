#pragma once
#include "hw_impl/hardware_interface.hpp"

namespace math {

class CPU : public HardwareInterface {
 public:
    void execute(const Op& op) override;

 private:
    void add(const Tensor& A, const Tensor& B, Tensor& C);
    void sub(const Tensor& A, const Tensor& B, Tensor& C);
    void multi(const Tensor& A, const Tensor& B, Tensor& C);
    void div(const Tensor& A, const Tensor& B, Tensor& C);

    void matmul(const Tensor& A, const Tensor& B, Tensor& C);

    void scale(const Tensor& A, Tensor& C, float alpha);

    void exp(const Tensor& A, Tensor& C);
    void log(const Tensor& A, Tensor& C);
    void sqrt(const Tensor& A, Tensor& C);
};

}  // namespace math